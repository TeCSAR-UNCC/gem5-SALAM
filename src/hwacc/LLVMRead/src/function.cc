#include "function.hh"

using namespace SALAM;

SALAM::Function::Function(uint64_t id) : SALAM::Value(id) {
	//
}

void
SALAM::Function::initialize(llvm::Value * irval,
						   irvmap *vmap,
						   RegisterList * regs,
						   bool isTop) {
	top = isTop;
	regList = regs;

	//Parse irval for function params
	llvm::Function * func = llvm::dyn_cast<llvm::Function>(irval);
	assert(func); //panic("Invalid llvm::Value type used to initialize function. Failed cast to llvm::Function.");

	// Fill arguments
	for (auto arg_iter = func->arg_begin(); arg_iter != func->arg_end(); arg_iter++) {
        llvm::Argument &arg = *arg_iter;
        std::shared_ptr<SALAM::Value> argval = vmap->find(&arg)->second;
        assert(argval);
        arguments.push_back(argval);
    }

    // Fill bbList
    for (auto bb_iter = func->begin(); bb_iter != func->end(); bb_iter++) {
        llvm::BasicBlock &bb = *bb_iter;
        std::shared_ptr<SALAM::Value> bbval = vmap->find(&bb)->second;
        assert(bbval);
        std::shared_ptr<SALAM::BasicBlock> bblock = std::dynamic_pointer_cast<SALAM::BasicBlock>(bbval);
        assert(bblock);
        bbList.push_back(bblock);
        bblock->initialize(&bb, vmap, regs);
    }

	Value::initialize(irval, vmap);
}