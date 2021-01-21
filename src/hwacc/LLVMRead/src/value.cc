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
SALAM::Value::Value_Debugger::dumper()
{
    if (DTRACE(SALAM_Debug)) {
        if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
        DPRINTF(SALAM_Debug, "%s \n", 
            "************** Value Dump **************"
        );
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
}

void
SALAM::Value::addRegister(bool istracked) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
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
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isIntegerTy());
	reg = new APIntRegister(val);
}
void
SALAM::Value::addAPIntRegister(const llvm::APSInt & val) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isIntegerTy());
	reg = new APIntRegister(val);
}
void
SALAM::Value::addAPFloatRegister(const llvm::APFloat & val) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isFloatingPointTy());
	reg = new APFloatRegister(val);
}
void
SALAM::Value::addPointerRegister(bool istracked, bool isnull) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isPointerTy());
	reg = new PointerRegister(istracked, isnull);
}
void
SALAM::Value::addPointerRegister(uint64_t val, bool istracked, bool isnull) {
	if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
	assert(irtype->isPointerTy());
	reg = new PointerRegister(val, istracked, isnull);
}