//------------------------------------------//
#include "basic_block.hh"
//------------------------------------------//

using namespace SALAM;

SALAM::BasicBlock::BasicBlock(uint64_t id) : SALAM::Value(id) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
SALAM::BasicBlock::initialize(llvm::Value * irval, irvmap *vmap, SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
	//Parse irval for BasicBlock params
	llvm::BasicBlock * bb = llvm::dyn_cast<llvm::BasicBlock>(irval);
	assert(bb); // panic("Invalid llvm::Value type used to initialize basic block. Failed cast to llvm::BasicBlock.");

    DPRINTF(LLVMInterface, "Initialize BasicBlocks\n");
	for (auto inst_iter = bb->begin(); inst_iter != bb->end(); inst_iter++) {
        llvm::Instruction &inst = *inst_iter;
        std::shared_ptr<SALAM::Value> instval = vmap->find(&inst)->second;
        assert(instval);
        std::shared_ptr<SALAM::Instruction> instruct = std::dynamic_pointer_cast<SALAM::Instruction>(instval);
        assert(instruct);
        instructions.push_back(instruct);
        instruct->instantiate(&inst, vmap, valueList);
        instruct->initialize(&inst, vmap, valueList);
        DPRINTF(LLVMInterface, "Instruction (UID: %d) Initialization Complete\n", instruct->getUID());
    }

    DPRINTF(LLVMInterface, "Initialize Values - BasicBlock::initialize\n");
	Value::initialize(irval, vmap);
}