#include "function.hh"

using namespace SALAM;

SALAM::Function::Function(uint64_t id) : SALAM::Value(id) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);	
}

void
SALAM::Function::initialize(llvm::Value * irval,
						   irvmap *vmap,
						   SALAM::valueListTy *valueList,
						   bool isTop) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	top = isTop;
	//Parse irval for function params
	llvm::Function * func = llvm::dyn_cast<llvm::Function>(irval);
	assert(func); //panic("Invalid llvm::Value type used to initialize function. Failed cast to llvm::Function.");

	// Fill arguments
    DPRINTF(LLVMInterface, "Initialize Function Arguments\n");
	for (auto arg_iter = func->arg_begin(); arg_iter != func->arg_end(); arg_iter++) {
        llvm::Argument &arg = *arg_iter;
        std::shared_ptr<SALAM::Value> argval = vmap->find(&arg)->second;
        assert(argval);
        std::shared_ptr<SALAM::Argument> argum = std::dynamic_pointer_cast<SALAM::Argument>(argval);
        assert(argum);
        arguments.push_back(argum);
        argum->initialize(&arg, vmap);
    }

    // Fill bbList
    DPRINTF(LLVMInterface, "Initialize BasicBlocks\n");
    for (auto bb_iter = func->begin(); bb_iter != func->end(); bb_iter++) {
        llvm::BasicBlock &bb = *bb_iter;
        std::shared_ptr<SALAM::Value> bbval = vmap->find(&bb)->second;
        assert(bbval);
        std::shared_ptr<SALAM::BasicBlock> bblock = std::dynamic_pointer_cast<SALAM::BasicBlock>(bbval);
        assert(bblock);
        bbList.push_back(bblock);
        bblock->initialize(&bb, vmap, valueList);
    }

    DPRINTF(LLVMInterface, "Initialize Values - Function::initialize\n");
	Value::initialize(irval, vmap);
}