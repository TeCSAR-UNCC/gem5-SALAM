#include "operand.hh"
#include "sim/sim_object.hh"


SALAM::Constant::Constant(uint64_t id, gem5::SimObject * owner, bool dbg) :
                          Value(id, owner, dbg)
{
}

void
SALAM::Constant::initialize(llvm::Value * irval,
                            SALAM::irvmap * irmap,
                            SALAM::valueListTy * values)
{
    //Initialize SALAM::Value
    SALAM::Value::initialize(irval, irmap);
    // Parse the constant value
    llvm::ConstantData * cd = llvm::dyn_cast<llvm::ConstantData>(irval);
    llvm::ConstantExpr * ce = llvm::dyn_cast<llvm::ConstantExpr>(irval);
    llvm::ConstantVector * cv = llvm::dyn_cast<llvm::ConstantVector>(irval);
    llvm::Type *irtype = irval->getType();
    if (cv) {
        /***********************************************************************
         * LLVM's handling of Vectors is a little complex. Vectors containing
         * other Constants are handled with the llvm::ConstantVector class. We
         * need to extract each element as a constant here. This is different
         * than the llvm::ConstantDataVector, which represents a vector
         * containing only simple constant data like ints, floats, and doubles.
         * We handle this as a subset of ConstantData.
         **********************************************************************/
        addRegister(irtype);
        auto vReg = std::dynamic_pointer_cast<SALAM::VectorRegister>(returnReg);
        auto vecType = llvm::dyn_cast<llvm::VectorType>(irtype);
        auto vectorCount = vecType->getElementCount();
        auto vectorSize = vectorCount.Min;
        for (auto it=0; it<vectorSize; it++) {
            llvm::Constant * con = cv->getAggregateElement(it);
            cd = llvm::dyn_cast<llvm::ConstantData>(con);
            auto contyid = con->getType()->getTypeID();
            switch (contyid)
            {
                case llvm::Type::IntegerTyID:
                {
                    llvm::ConstantInt * ci = llvm::dyn_cast<llvm::ConstantInt>(con);
                    auto bitwidth = ci->getBitWidth();
                    auto byteWidth = ((bitwidth - 1) >> 3) + 1;
                    auto zExtValue = ci->getZExtValue();
                    switch (byteWidth)
                    {
                        case 1:
                        {
                            vReg->setVectorElement<uint8_t>(zExtValue, it);
                            break;
                        }
                        case 2:
                        {
                            vReg->setVectorElement<uint16_t>(zExtValue, it);
                            break;
                        }
                        case 4:
                        {
                            vReg->setVectorElement<uint32_t>(zExtValue, it);
                            break;
                        }
                        case 8:
                        {
                            vReg->setVectorElement<uint64_t>(zExtValue, it);
                            break;
                        }
                        default:
                        {
                            assert(0 && "Unsupported integer size in vector");
                        }
                    }
                    break;
                }
                case llvm::Type::FloatTyID:
                {
                    llvm::ConstantFP * fp = llvm::dyn_cast<llvm::ConstantFP>(con);
                    auto apfp = fp->getValueAPF();
                    vReg->setVectorElement<float>(apfp.convertToFloat(), it);
                    break;
                }
                case llvm::Type::DoubleTyID:
                {
                    llvm::ConstantFP * fp = llvm::dyn_cast<llvm::ConstantFP>(con);
                    auto apfp = fp->getValueAPF();
                    vReg->setVectorElement<double>(apfp.convertToDouble(), it);
                    break;
                }
                default:
                {
                    assert(0 && "Unsupported type for Vector element in SALAM\n");
                }
            }
        }
    } else if (cd) {
        // The constant is a llvm::ConstantData.
        // Get it's value and store it in constValue
        if (irtype->isFloatingPointTy()) {
            llvm::ConstantFP * fp = llvm::dyn_cast<llvm::ConstantFP>(cd);
        #if USE_LLVM_AP_VALUES
            addAPFloatRegister(fp->getValueAPF());
        #else
            auto apfp = fp->getValueAPF();
            auto api = apfp.bitcastToAPInt();
            addAPFloatRegister(api.getLimitedValue());
        #endif
        } else if (irtype->isIntegerTy()) {
            llvm::ConstantInt * in = llvm::dyn_cast<llvm::ConstantInt>(cd);
        #if USE_LLVM_AP_VALUES
            addAPIntRegister(in->getValue());
        #else
            auto api = in->getValue();
            addAPIntRegister(api.getLimitedValue());
        #endif
        } else if (irtype->isPointerTy()) {
            assert(llvm::dyn_cast<llvm::ConstantPointerNull>(cd));
            addPointerRegister(false, true);
        } else if (irtype->isVectorTy()) {
            addRegister(irtype);
            auto vecType = llvm::dyn_cast<llvm::VectorType>(irtype);
            auto vectorCount = vecType->getElementCount();
            auto vectorSize = vectorCount.Min;
            auto elemType = vecType->getElementType();
            llvm::ConstantDataSequential * cds = llvm::dyn_cast<llvm::ConstantDataSequential>(cd);
            if (cds) {
                auto vReg = std::dynamic_pointer_cast<SALAM::VectorRegister>(returnReg);
            } else { 
                // zeroinitializer case
            }
        }
    } else if (ce) {
        // The constant is an expression. We need to parse the expression
        for (auto op : ce->operand_values()) {
            // Iterate over operands and add
            // new values to our map and value list
            auto mapit = irmap->find(op);
            if (mapit == irmap->end()) {
                uint64_t id = values->back()->getUID() + 1;
                std::shared_ptr<SALAM::Constant> con =
                    std::make_shared<SALAM::Constant>(id, owner, dbg);
                values->push_back(con);
                irmap->insert(SALAM::irvmaptype(op, con));
                operands.push_back(con);
                con->initialize(op, irmap, values);
            } else {
                std::shared_ptr<SALAM::Value> opval = mapit->second;
                operands.push_back(opval);
            }
        }

    #if USE_LLVM_AP_VALUES
        #if (LLVM_VERSION_MAJOR <= 9)
            auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
        #else
            auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
        #endif
    #endif

        switch(ce->getOpcode()) {
            case llvm::Instruction::Trunc:
            {
                auto opdata = operands.front()->getIntRegValue();
            #if USE_LLVM_AP_VALUES
                addAPIntRegister(opdata.trunc(size));
            #else
                addAPIntRegister(opdata);
            #endif
                break;
            }
            case llvm::Instruction::ZExt:
            {
                auto opdata = operands.front()->getIntRegValue();
            #if USE_LLVM_AP_VALUES
                opdata.setIsSigned(false);
                addAPIntRegister(opdata.extend(size));
            #else
                addAPIntRegister(opdata);
            #endif
                break;
            }
            case llvm::Instruction::SExt:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getIntRegValue();
                opdata.setIsSigned(true);
                addAPIntRegister(opdata.extend(size));
            #else
                int64_t tmp = operands.front()->getSIntRegValue();
                addAPIntRegister((uint64_t)tmp);
            #endif
                break;
            }
            case llvm::Instruction::FPToUI:
            {
            #if USE_LLVM_AP_VALUES
                llvm::APSInt tmp(size, true);
                bool exact;
                auto opdata = operands.front()->getFloatRegValue();
                auto err = opdata.convertToInteger(tmp,
                                                  rounding,
                                                  &exact);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPIntRegister(tmp);
            #else
                if (operands.front()->getSize() == 32) {
                    auto opdata = operands.front()->getFloatFromReg();
                    addAPIntRegister((uint64_t)opdata);
                } else {
                    auto opdata = operands.front()->getDoubleFromReg();
                    addAPIntRegister((uint64_t)opdata);
                }
            #endif
                break;
            }
            case llvm::Instruction::FPToSI:
            {
            #if USE_LLVM_AP_VALUES
                llvm::APSInt tmp(size, false);
                bool exact;
                auto opdata = operands.front()->getFloatRegValue();
                auto err = opdata.convertToInteger(tmp,
                                                  rounding,
                                                  &exact);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPIntRegister(tmp);
            #else
                if (operands.front()->getSize() == 32) {
                    auto opdata = operands.front()->getFloatFromReg();
                    addAPIntRegister((uint64_t)(int64_t)opdata);
                } else {
                    auto opdata = operands.front()->getDoubleFromReg();
                    addAPIntRegister((uint64_t)(int64_t)opdata);
                }
            #endif
                break;
            }
            case llvm::Instruction::UIToFP:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getIntRegValue();
                llvm::APFloat tmp(irtype->getFltSemantics());
                auto err = tmp.convertFromAPInt(opdata, false, rounding);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                auto opdata = operands.front()->getUIntRegValue();
                switch (size) {
                    case 32:
                    {
                        float tmp = (float)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    case 64:
                    {
                        double tmp = (double)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                        break;
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::SIToFP:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getIntRegValue();
                llvm::APFloat tmp(irtype->getFltSemantics());
                auto err = tmp.convertFromAPInt(opdata, false, rounding);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                auto opdata = operands.front()->getSIntRegValue();
                switch (size) {
                    case 32:
                    {
                        float tmp = (float)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    case 64:
                    {
                        double tmp = (double)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                        break;
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::FPTrunc:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getFloatRegValue();
                llvm::APFloat tmp(opdata);
                bool losesInfo;
                auto err = tmp.convert(irtype->getFltSemantics(),
                                       rounding, &losesInfo);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                switch (operands.front()->getSize()) {
                    case 64:
                    {
                        double opdata = operands.front()->getDoubleFromReg();
                        float tmp = (float)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::FPExt:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getFloatRegValue();
                llvm::APFloat tmp(opdata);
                bool losesInfo;
                auto err = tmp.convert(irtype->getFltSemantics(),
                                       rounding, &losesInfo);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                switch (operands.front()->getSize()) {
                    case 32:
                    {
                        float opdata = operands.front()->getFloatFromReg();
                        double tmp = (double)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::PtrToInt:
            {
                auto opdata = operands.front()->getReg()->getPtrData();
            #if USE_LLVM_AP_VALUES
                addAPIntRegister(llvm::APInt(64, opdata));
            #else
                addAPIntRegister(opdata);
            #endif
                break;
            }
            case llvm::Instruction::IntToPtr:
            {
                auto opdata = operands.front()->getIntRegValue();
            #if USE_LLVM_AP_VALUES
                assert(opdata.isUnsigned());
                int64_t tmp = opdata.getExtValue();
                addPointerRegister(*(uint64_t *)&tmp, false, false);
            #else
                addPointerRegister(opdata, false, false);
            #endif
                break;
            }
            default:
                assert(0); // We do not support this nested ConstantExpr
        }
    } else {
        assert(0); // The value is not a supported type of llvm::Constant
    }
}

SALAM::GlobalConstant::GlobalConstant(uint64_t id, gem5::SimObject * owner, bool dbg) :
                                      Constant(id, owner, dbg)
{
}

void
SALAM::GlobalConstant::initialize(llvm::Value * irval,
                                  SALAM::irvmap * irmap,
                                  SALAM::valueListTy * values)
{
    // Parse the initializer of the value
    auto glb = llvm::dyn_cast<llvm::GlobalVariable>(irval);
    assert(glb);
    assert(glb->hasInitializer());
    // glb->getInitializer()->print(llvm::outs());

    //Initialize SALAM::ConstantData
    SALAM::Constant::initialize(glb->getInitializer(), irmap, values);
}

SALAM::Argument::Argument(uint64_t id, gem5::SimObject * owner, bool dbg) :
                          Value(id, owner, dbg)
{
}

void
SALAM::Argument::initialize(llvm::Value * irval, SALAM::irvmap * irmap)
{
    //Initialize SALAM::Value
    SALAM::Value::initialize(irval, irmap);
    addRegister(irval->getType());
}

SALAM::Operand::Operand_Debugger::Operand_Debugger()
{

}

void
SALAM::Operand::Operand_Debugger::dumper(Operand * op)
{

}

// copy constructor
SALAM::Operand::Operand(const SALAM::Operand &copy_val):
           SALAM::Value(copy_val)
{
    lockedValue = copy_val.lockedValue;
    set = copy_val.set;
}

// copy constructor from base
SALAM::Operand::Operand(const SALAM::Value &copy_val):
           SALAM::Value(copy_val)
{ // Update here for values in the copied value base class
    initOperandReg();
}

SALAM::Operand::Operand(std::shared_ptr<SALAM::Value> copy_val):
            SALAM::Value(copy_val)
{
    initOperandReg();
}

// operator equals
SALAM::Operand&
SALAM::Operand::operator = (SALAM::Operand &copy_val)
{
    uid = copy_val.uid;
      returnReg = copy_val.returnReg;
    valueTy = copy_val.valueTy;
    size = copy_val.size;
    lockedValue = copy_val.lockedValue;
    set = copy_val.set;
    return *this;
}

void
SALAM::Operand::initOperandReg()
{
    bool istracked = false;
    if (returnReg->isPtr()) {
        if (dbg) DPRINTFS(Runtime, owner, "Operand Ptr Register Initialized\n");
        lockedValue = std::make_shared<PointerRegister>(istracked);
    } else if (returnReg->isInt()) {
        if (dbg) DPRINTFS(Runtime, owner, "Operand Int Register Initialized\n");
        lockedValue = std::make_shared<APIntRegister>(size, istracked);
    } else if (returnReg->isFP()) {
        if (dbg) DPRINTFS(Runtime, owner, "Operand FP Register Initialized\n");
        lockedValue =
            std::make_shared<APFloatRegister>(valueTy, istracked);
    } else if (returnReg->isVector()) {
        if (dbg) DPRINTFS(Runtime, owner, "Operand Vector Register Initialized\n");
        lockedValue = std::make_shared<VectorRegister>(valueTy, istracked);
    } else {
        if (dbg) DPRINTFS(Runtime, owner, "Invalid register type. Dumping Operand details\n");
        dump();
        assert(0); // Type is invalid for a register
    }
}

void
SALAM::Operand::initialize(llvm::Value * irval, SALAM::irvmap * irmap)
{
    SALAM::Value::initialize(irval, irmap);
}

void
SALAM::Operand::updateOperandRegister() {
    assert(lockedValue);
    if (lockedValue->isPtr()) {
        lockedValue->writePtrData(returnReg->getPtrData(true),
                                  getSizeInBytes());
    } else if (lockedValue->isInt()) {
        lockedValue->writeIntData(returnReg->getIntData(true));
    } else if (lockedValue->isFP()) {
        lockedValue->writeFloatData(returnReg->getFloatData(true));
    } else if (lockedValue->isVector()) {
        lockedValue->writeVectorData(returnReg->getVectorData(true));
    }
}
