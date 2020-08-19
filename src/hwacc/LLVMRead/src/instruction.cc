#include "instruction.hh"

void
SALAM::Instruction::initialize(llvm::Value * irval, irvmap * irmap) {
	SALAM::Value::initialize(irval, irmap);
}