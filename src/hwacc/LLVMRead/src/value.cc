#include "value.hh"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"

void
SALAM::Value::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	TRACEOUT("SALAM::Value::initialize");
	irtype = irval->getType();
	if (irtype->getTypeID() == llvm::Type::PointerTyID) {
		size = 64; //We assume a 64-bit memory address space
	} else {
		size = irtype->getScalarSizeInBits();
	}
	// Link Return Register

	DEBUGOUT("Initialize Return Register");
}

void
SALAM::Value::addRegister(bool istracked) {
	TRACEOUT("SALAM::Value::addRegister");
	if (irtype->isPointerTy()) {
		reg = new PointerRegister(istracked);
	} else if (irtype->isIntegerTy()) {
		reg = new APIntRegister(irtype, istracked);
	} else if (irtype->isFloatingPointTy()) {
		reg = new APFloatRegister(irtype, istracked);
	} else {
		assert(0); // Type is invalid for a register
	}
}
void
SALAM::Value::addAPIntRegister(const llvm::APInt & val) {
	TRACEOUT("SALAM::Value::addAPIntRegister");
	assert(irtype->isIntegerTy());
	reg = new APIntRegister(val);
}
void
SALAM::Value::addAPIntRegister(const llvm::APSInt & val) {
	TRACEOUT("SALAM::Value::addAPIntRegister");
	assert(irtype->isIntegerTy());
	reg = new APIntRegister(val);
}
void
SALAM::Value::addAPFloatRegister(const llvm::APFloat & val) {
	TRACEOUT("SALAM::Value::addAPFloatRegister");
	assert(irtype->isFloatingPointTy());
	reg = new APFloatRegister(val);
}
void
SALAM::Value::addPointerRegister(bool istracked, bool isnull) {
	TRACEOUT("SALAM::Value::addPointerRegister");
	assert(irtype->isPointerTy());
	reg = new PointerRegister(istracked, isnull);
}
void
SALAM::Value::addPointerRegister(uint64_t val, bool istracked, bool isnull) {
	TRACEOUT("SALAM::Value::addPointerRegister");
	assert(irtype->isPointerTy());
	reg = new PointerRegister(val, istracked, isnull);
}

void
SALAM::Constant::initialize(llvm::Value * irval, SALAM::irvmap * irmap, SALAM::valueListTy * values) {
	TRACEOUT("SALAM::Constant::initialize");
	//Initialize SALAM::Value
	SALAM::Value::initialize(irval, irmap);
	// Parse the constant value
	llvm::ConstantData * cd = llvm::dyn_cast<llvm::ConstantData>(irval);
	llvm::ConstantExpr * ce = llvm::dyn_cast<llvm::ConstantExpr>(irval);
	if (cd) {
		// The constant is a llvm::ConstantData. Get it's value and store it in constValue
		if (irtype->isFloatingPointTy()) {
			llvm::ConstantFP * fp = llvm::dyn_cast<llvm::ConstantFP>(cd);
			addAPFloatRegister(fp->getValueAPF());
		} else if (irtype->isIntegerTy()) {
			llvm::ConstantInt * in = llvm::dyn_cast<llvm::ConstantInt>(cd);
			addAPIntRegister(in->getValue());
		} else if (irtype->isPointerTy()) {
			assert(llvm::dyn_cast<llvm::ConstantPointerNull>(cd));
			addPointerRegister(false, true);
		}
	} else if (ce) {
		// The constant is an expression. We need to parse the expression
		for (auto op : ce->operand_values()) {
			// Iterate over operands and add new values to our map and value list
			auto mapit = irmap->find(op);
    		if(mapit == irmap->end()) {
    			uint64_t id = values->back()->getUID() + 1;
    			std::shared_ptr<SALAM::Constant> con = std::make_shared<SALAM::Constant>(id);
		        values->push_back(con);
		        irmap->insert(SALAM::irvmaptype(op, con));
		        operands.push_back(con);
		        con->initialize(op, irmap, values);
    		} else {
    			std::shared_ptr<SALAM::Value> opval = mapit->second;
    			operands.push_back(opval);
    		}
		}

		// After LLVM 9.0 rounding mode was moved to "llvm/ADT/FloatingPointMode.h"
		// We are using LLVM 9.0 so it is instead found in "llvm/ADT/APFloat.h"
		auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;

		switch(ce->getOpcode()) {
			case llvm::Instruction::Trunc:
				{
					auto opdata = operands.front()->getReg()->getIntData();
					addAPIntRegister(opdata->trunc(size));
					break;
				}
	        case llvm::Instruction::ZExt:
	        	{
					auto opdata = operands.front()->getReg()->getIntData();
					opdata->setIsSigned(false);
					addAPIntRegister(opdata->extend(size));
					break;
				}
	        case llvm::Instruction::SExt:
	        	{
					auto opdata = operands.front()->getReg()->getIntData();
					opdata->setIsSigned(true);
					addAPIntRegister(opdata->extend(size));
					break;
				}
	        case llvm::Instruction::FPToUI:
	        	{
	        		llvm::APSInt tmp(size, true);
	        		bool exact;
	        		auto opdata = operands.front()->getReg()->getFloatData();
	        		auto err = opdata->convertToInteger(tmp,
	        										  rounding,
	        										  &exact);
	        		assert(err == llvm::APFloatBase::opStatus::opOK);
	        		addAPIntRegister(tmp);
	        		break;
	        	}
	        case llvm::Instruction::FPToSI:
	        	{
	        		llvm::APSInt tmp(size, false);
	        		bool exact;
	        		auto opdata = operands.front()->getReg()->getFloatData();
	        		auto err = opdata->convertToInteger(tmp,
	        										  rounding,
	        										  &exact);
	        		assert(err == llvm::APFloatBase::opStatus::opOK);
	        		addAPIntRegister(tmp);
	        		break;
	        	}
	        case llvm::Instruction::UIToFP:
	        	{
	        		auto opdata = operands.front()->getReg()->getIntData();
	        		llvm::APFloat tmp(irtype->getFltSemantics());
	        		auto err = tmp.convertFromAPInt(*opdata, false, rounding);
	        		assert(err == llvm::APFloatBase::opStatus::opOK);
	        		addAPFloatRegister(tmp);
	        		break;
	        	}
	        case llvm::Instruction::SIToFP:
	        	{
	        		auto opdata = operands.front()->getReg()->getIntData();
	        		llvm::APFloat tmp(irtype->getFltSemantics());
	        		auto err = tmp.convertFromAPInt(*opdata, false, rounding);
	        		assert(err == llvm::APFloatBase::opStatus::opOK);
	        		addAPFloatRegister(tmp);
	        		break;
	        	}
	        case llvm::Instruction::FPTrunc:
	        	{
	        		auto opdata = operands.front()->getReg()->getFloatData();
	        		llvm::APFloat tmp(*opdata);
	        		bool losesInfo;
	        		auto err = tmp.convert(irtype->getFltSemantics(), rounding, &losesInfo);
	        		assert(err == llvm::APFloatBase::opStatus::opOK);
	        		addAPFloatRegister(tmp);
	        		break;
	        	}
	        case llvm::Instruction::FPExt:
	        	{
	        		auto opdata = operands.front()->getReg()->getFloatData();
	        		llvm::APFloat tmp(*opdata);
	        		bool losesInfo;
	        		auto err = tmp.convert(irtype->getFltSemantics(), rounding, &losesInfo);
	        		assert(err == llvm::APFloatBase::opStatus::opOK);
	        		addAPFloatRegister(tmp);
	        		break;
	        	}
	        case llvm::Instruction::PtrToInt:
	        	{
	        		auto opdata = operands.front()->getReg()->getPtrData();
	        		addAPIntRegister(llvm::APInt(64, *opdata));
	        		break;
	        	}
	        case llvm::Instruction::IntToPtr:
	        	{
	        		auto opdata = operands.front()->getReg()->getIntData();
	        		assert(opdata->isUnsigned());
	        		int64_t tmp = opdata->getExtValue();
	        		addPointerRegister(*(uint64_t *)&tmp, false, false);
	        		break;
	        	}
	        default:
	        	assert(0); // We currently do not support this nested ConstantExpr
		}
	} else {
		assert(0); // The value is not a supported type of llvm::Constant
	}
}

void
SALAM::GlobalConstant::initialize(llvm::Value * irval, SALAM::irvmap * irmap, SALAM::valueListTy * values) {
	TRACEOUT("SALAM::GlobalConstant::initialize");
	// Parse the initializer of the value
	auto glb = llvm::dyn_cast<llvm::GlobalVariable>(irval);
	assert(glb);
	assert(glb->hasInitializer());
	// glb->getInitializer()->print(llvm::outs());

	//Initialize SALAM::ConstantData
	SALAM::Constant::initialize(glb->getInitializer(), irmap, values);
}

void
SALAM::Argument::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	TRACEOUT("SALAM::Argument::initialize");
	//Initialize SALAM::Value
	SALAM::Value::initialize(irval, irmap);
	addRegister();
}