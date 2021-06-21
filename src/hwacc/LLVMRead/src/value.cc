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

// copy constructor
SALAM::Value::Value(const Value &copy_val)
{
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: [Copy Const]%s \n", __PRETTY_FUNCTION__);
  	uid = copy_val.uid;
  	returnReg = copy_val.returnReg;
	irtype = copy_val.irtype;
	size = copy_val.size;
}

// operator equals
SALAM::Value& 
SALAM::Value::operator = (Value &copy_val)
{
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: [= Overload] %s \n", __PRETTY_FUNCTION__);
	uid = copy_val.uid;
  	returnReg = copy_val.returnReg;
	irtype = copy_val.irtype;
	size = copy_val.size;
  	return *this;
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
        DPRINTF(SALAM_Debug, "%s\n\t\t %s%s\n\t\t %s%d\n\t\t %s%d%s%d%s \n", 
            "|-(Value Base) ",
            " | LLVM IR: ", value->getIRString(),
            " | UID: ", value->getUID(),
            " | Size: ", value->getSize(), " bits [", value->getSizeInBytes(), " bytes]"
        );
		if(value->getReg()) value->getReg()->dump();
		//DPRINTF(SALAM_Debug, "%s\n%s\n",
		//	"************** LLVM IR Dump **************",
		//	value->getIRString()
		//);
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
	if (size>0) addRegister(true);

	std::string tmpstr;
	llvm::raw_string_ostream ss(ir_string);
	ss << *irval;
	ir_string = ss.str();
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

#ifdef USE_AP_VALUES
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
#else
	void
	SALAM::Value::addAPIntRegister(const uint64_t & val) {
		if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
		assert(irtype->isIntegerTy());
		uint64_t bitmask = 0;
		assert((size <= 64) && "Only 64-bit and smaller values are supported when not using AP values.");
		bitmask = (bitmask - 1) >> (64 - size);
		returnReg = std::make_shared<APIntRegister>(val & bitmask);
	}
	void
	SALAM::Value::addAPFloatRegister(const uint64_t & val) {
		if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
		assert(irtype->isFloatingPointTy());
		uint64_t bitmask = 0;
		assert((size <= 64) && "Only 64-bit and smaller values are supported when not using AP values.");
		bitmask = (bitmask - 1) >> (64 - size);
		returnReg = std::make_shared<APFloatRegister>(val & bitmask);
	}
#endif

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

#ifdef USE_AP_VALUES
	void
	SALAM::Value::setRegisterValue(const llvm::APInt &data) {
		if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	    DPRINTF(Runtime, "| APInt Register\n");
		if (returnReg->isInt()) {
			llvm::APInt tmp = data;
			returnReg->writeIntData(&tmp);
		} else {
			DPRINTF(Runtime, "Unsupported type for register operation. Tried to place integer data in non-integer register.\n");
		}
	}
	void
	SALAM::Value::setRegisterValue(const llvm::APFloat &data) {
		if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	    DPRINTF(Runtime, "| APFloat Register\n");
		if (returnReg->isFP()) {
			llvm::APFloat tmp = data;
			returnReg->writeFloatData(&tmp);
		} else {
			DPRINTF(Runtime, "Unsupported type for register operation. Tried to place float data in non-float register.\n");
		}
	}
#endif

void
SALAM::Value::setRegisterValue(const uint64_t data) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (returnReg->isPtr()) {
    	DPRINTF(Runtime, "| Ptr Register\n");
		uint64_t tmp = data;
		returnReg->writePtrData(&tmp);
	} else {
	#ifdef USE_AP_VALUES
		DPRINTF(Runtime, "Unsupported type for register operation. Tried to place Ptr data in non-Ptr register.\n");
	#else
		if (returnReg->isInt()) {
			DPRINTF(Runtime, "| Int Register\n");
			uint64_t tmp = data;
			returnReg->writeIntData(&tmp, getSizeInBytes());
		} else {
			DPRINTF(Runtime, "| FP Register\n");
			uint64_t tmp = data;
			returnReg->writeFloatData(&tmp, getSizeInBytes());
		}
	#endif
	}
}
void
SALAM::Value::setRegisterValue(uint8_t * data) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	DPRINTF(Runtime, "| Set Register Data - ");
    switch (irtype->getTypeID()) {
    #ifdef USE_AP_VALUES
        case llvm::Type::FloatTyID:
        {
            DPRINTF(Runtime, "Float\n");
            float tmpData;
            std::memcpy(&tmpData, data, sizeof(float));
            llvm::APFloat * regData = returnReg->getFloatData();
            *regData = llvm::APFloat(tmpData);
            break;
        }
        case llvm::Type::DoubleTyID:
        {
            DPRINTF(Runtime, "Double\n");
            double tmpData;
            std::memcpy(&tmpData, data, sizeof(double));
            llvm::APFloat * regData = returnReg->getFloatData();
            *regData = llvm::APFloat(tmpData);
            break;
        }
        case llvm::Type::IntegerTyID:
        {
            DPRINTF(Runtime, "Integer Type | Size = %d\n", size);
            llvm::APInt * regData = returnReg->getIntData();
            if (size > 64) {
                size_t bigIntLen = ((size - 1) / 64) + 1;
                *regData = llvm::APInt(size, llvm::ArrayRef<uint64_t>((uint64_t *)data, bigIntLen));
            } else {
                *regData = llvm::APInt(size, *(uint64_t *)(data));
            }
            break;
        }
    #else
        case llvm::Type::FloatTyID:
        {
            DPRINTF(Runtime, "Float\n");
            float tmpData;
            returnReg->writeFloatData((uint64_t *)data, (size_t)4);
            break;
        }
        case llvm::Type::DoubleTyID:
        {
            DPRINTF(Runtime, "Double\n");
            returnReg->writeFloatData((uint64_t *)data, (size_t)8);
            break;
        }
        case llvm::Type::IntegerTyID:
        {
            DPRINTF(Runtime, "Integer Type | Size = %d\n", size);
            returnReg->writeIntData((uint64_t *)data, (size_t)getSizeInBytes());
            break;
        }
    #endif
        case llvm::Type::PointerTyID:
        {
            DPRINTF(Runtime, "Pointer\n");
            returnReg->writePtrData((uint64_t *)data);
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
SALAM::Value::setRegisterValue(bool data) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    DPRINTF(Runtime, "| Int Register\n");
	if (returnReg->isInt()) {
	#ifdef USE_AP_VALUES
		if (data == true) {
			setRegisterValue(llvm::APInt::getAllOnesValue(1));
		} else {
			setRegisterValue(llvm::APInt::getNullValue(1));
		}
	#else
		if (data == true) {
			setRegisterValue((uint64_t)1);
		} else {
			setRegisterValue((uint64_t)0);
		}
	#endif
	} else {
		DPRINTF(Runtime, "Unsupported type for register operation. Tried to place integer data in non-integer register.\n");
	}
}

void
SALAM::Value::setRegisterValue(std::shared_ptr<SALAM::Register> reg) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++setRegisterValue()\n");
	if (reg->isPtr()) {
		setRegisterValue(*(reg->getPtrData()));
	} else if (reg->isFP()) {
		setRegisterValue(*(reg->getFloatData()));
	} else {
		setRegisterValue(*(reg->getIntData()));
	}
    DPRINTF(Runtime, "||==setRegisterValue====\n");
}