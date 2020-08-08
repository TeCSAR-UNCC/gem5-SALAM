#include "function.hh"

using namespace SALAM;

SALAM::Function::Function(uint64_t id) : SALAM::Value(id) {
	bbList = new std::list<SALAM::BasicBlock*>();
}

void
SALAM::Function::initialize(llvm::Value * irval,
						   irvmap *vmap,
						   RegisterList * regs,
						   bool dbg,
						   bool isTop) {
	debug = dbg;
	top = isTop;

	//Parse irval for function params
	llvm::Function * func = llvm::dyn_cast<llvm::Function>(irval);
	if (!func) {
		panic("Invalid llvm::Value type used to initialize function. Failed cast to llvm::Function.");
	} else {
		//
	}

	Value::initialize(irval, vmap);
}