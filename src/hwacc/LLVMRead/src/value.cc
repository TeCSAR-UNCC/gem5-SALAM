#include "value.hh"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"

SALAM::Value::Value(uint64_t id) {
	uid = id;
	size = 0;
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s | UID: %d \n", __PRETTY_FUNCTION__, this->uid);
    if (DTRACE(SALAM_Debug)) {
        this->dbg = true;
        this->value_dbg = new Value_Debugger();
    }
}

SALAM::Value::~Value() 
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace Deleted: %s | UID:%d \n", __PRETTY_FUNCTION__, this->uid);
    //if (DTRACE(SALAM_Debug)) delete value_dbg;
}

SALAM::Value::Value_Debugger::Value_Debugger()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

void
SALAM::Value::Value_Debugger::dumper(SALAM::Value *value)
{
    if (DTRACE(SALAM_Debug)) {
        if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
        DPRINTF(SALAM_Debug, "%s \n", 
            "************** Value Dump **************"
        );
		if(value->getReg()) value->getReg()->dump();
    }
}

void
SALAM::Value::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	irtype = irval->getType();
	if (irtype->getTypeID() == llvm::Type::PointerTyID) {
		size = 64; //We assume a 64-bit memory address space
	} else {
		size = irtype->getScalarSizeInBits();
	}
	// Link Return Register
	addRegister(true);
	//linkOperands();
}

void
SALAM::Value::addRegister(bool istracked) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	if (irtype->isPointerTy()) {
		returnReg = std::make_shared<PointerRegister>(istracked);
	} else if (irtype->isIntegerTy()) {
		returnReg = std::make_shared<APIntRegister>(irtype, istracked);
	} else if (irtype->isFloatingPointTy()) {
		returnReg = std::make_shared<APFloatRegister>(irtype, istracked);
	} else {
		//assert(0); // Type is invalid for a register
		returnReg = nullptr;
	}
}

void
SALAM::Value::addAPIntRegister(const llvm::APInt & val) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isIntegerTy());
	returnReg = std::make_shared<APIntRegister>(val);
}
void
SALAM::Value::addAPIntRegister(const llvm::APSInt & val) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isIntegerTy());
	returnReg = std::make_shared<APIntRegister>(val);
}
void
SALAM::Value::addAPFloatRegister(const llvm::APFloat & val) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isFloatingPointTy());
	returnReg = std::make_shared<APFloatRegister>(val);
}
void
SALAM::Value::addPointerRegister(bool istracked, bool isnull) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isPointerTy());
	returnReg = std::make_shared<PointerRegister>(istracked, isnull);
}
void
SALAM::Value::addPointerRegister(uint64_t val, bool istracked, bool isnull) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isPointerTy());
	returnReg = std::make_shared<PointerRegister>(val, istracked, isnull);
}

void
SALAM::Value::setRegisterValue(const llvm::APInt &data) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	if (returnReg->isInt()) {
		llvm::APInt * regData = returnReg->getIntData();
		*regData = data;
	} else {
		DPRINTF(Runtime, "Unsupported type for register operation. Tried to place integer data in non-integer register.\n");
	}
}
void
SALAM::Value::setRegisterValue(const llvm::APFloat &data) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	if (returnReg->isFP()) {
		llvm::APFloat * regData = returnReg->getFloatData();
		*regData = data;
	} else {
		DPRINTF(Runtime, "Unsupported type for register operation. Tried to place float data in non-float register.\n");
	}
}
void
SALAM::Value::setRegisterValue(const uint64_t data) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	if (returnReg->isPtr()) {
		uint64_t * regData = returnReg->getPtrData();
		*regData = data;
	} else {
		DPRINTF(Runtime, "Unsupported type for register operation. Tried to place Ptr data in non-Ptr register.\n");
	}
}
void
SALAM::Value::setRegisterValue(uint8_t * data) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	switch (irtype->getTypeID()) {
        case llvm::Type::FloatTyID:
        {
            float tmpData;
            std::memcpy(&tmpData, data, sizeof(float));
            llvm::APFloat * regData = returnReg->getFloatData();
            *regData = llvm::APFloat(tmpData);
            break;
        }
        case llvm::Type::DoubleTyID:
        {
            double tmpData;
            std::memcpy(&tmpData, data, sizeof(double));
            llvm::APFloat * regData = returnReg->getFloatData();
            *regData = llvm::APFloat(tmpData);
            break;
        }
        case llvm::Type::IntegerTyID:
        {
            llvm::APInt * regData = returnReg->getIntData();
            if (size > 64) {
                size_t bigIntLen = ((size - 1) / 64) + 1;
                *regData = llvm::APInt(size, llvm::ArrayRef<uint64_t>((uint64_t *)data, bigIntLen));
            } else {
                *regData = llvm::APInt(size, *(uint64_t *)(data));
            }
            break;
        }
        case llvm::Type::PointerTyID:
        {
            std::memcpy(returnReg->getPtrData(), data, 8);
            break;
        }
        default:
        {
            DPRINTF(Runtime, "Unsupported type for register operation\n");
            assert(0);
        }
    }
}

void
SALAM::Value::setRegisterValue(std::shared_ptr<SALAM::Register> reg) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	if (reg->isPtr()) {
		setRegisterValue(*(reg->getPtrData()));
	} else if (reg->isFP()) {
		setRegisterValue(*(reg->getFloatData()));
	} else {
		setRegisterValue(*(reg->getIntData()));
	}
}