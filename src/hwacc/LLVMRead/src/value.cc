#include "value.hh"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"

void
SALAM::Value::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	//if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	irtype = irval->getType();
	if (irtype->getTypeID() == llvm::Type::PointerTyID) {
		size = 64; //We assume a 64-bit memory address space
	} else {
		size = irtype->getScalarSizeInBits();
	}
	// Link Return Register

	//if (dbg) DPRINTF(Debug::LLVMInterface, "Initialize Return Register\n");
}

void
SALAM::Value::addRegister(bool istracked) {
	//if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
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
	//if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	assert(irtype->isIntegerTy());
	reg = new APIntRegister(val);
}
void
SALAM::Value::addAPIntRegister(const llvm::APSInt & val) {
	//if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	assert(irtype->isIntegerTy());
	reg = new APIntRegister(val);
}
void
SALAM::Value::addAPFloatRegister(const llvm::APFloat & val) {
	//if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	assert(irtype->isFloatingPointTy());
	reg = new APFloatRegister(val);
}
void
SALAM::Value::addPointerRegister(bool istracked, bool isnull) {
	//if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	assert(irtype->isPointerTy());
	reg = new PointerRegister(istracked, isnull);
}
void
SALAM::Value::addPointerRegister(uint64_t val, bool istracked, bool isnull) {
	//if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	assert(irtype->isPointerTy());
	reg = new PointerRegister(val, istracked, isnull);
}