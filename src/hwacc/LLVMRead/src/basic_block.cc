//------------------------------------------//
#include "basic_block.hh"
//------------------------------------------//

using namespace SALAM;

SALAM::BasicBlock::BasicBlock(uint64_t id) : SALAM::Value(id) {
    CLASSOUT("SALAM::BasicBlock::BasicBlock", id);
	//
}

void
SALAM::BasicBlock::initialize(llvm::Value * irval, irvmap *vmap, SALAM::valueListTy * valueList) {
    TRACEOUT("SALAM::BasicBlock::initialize");
	//Parse irval for BasicBlock params
	llvm::BasicBlock * bb = llvm::dyn_cast<llvm::BasicBlock>(irval);
	assert(bb); // panic("Invalid llvm::Value type used to initialize basic block. Failed cast to llvm::BasicBlock.");

    DEBUGOUT("Initialize BasicBlocks");
	for (auto inst_iter = bb->begin(); inst_iter != bb->end(); inst_iter++) {
        DEBUGITER("-");
        llvm::Instruction &inst = *inst_iter;
        std::shared_ptr<SALAM::Value> instval = vmap->find(&inst)->second;
        assert(instval);
        std::shared_ptr<SALAM::Instruction> instruct = std::dynamic_pointer_cast<SALAM::Instruction>(instval);
        assert(instruct);
        instructions.push_back(instruct);
        instruct->instantiate(&inst, vmap, valueList);
        instruct->initialize(&inst, vmap, valueList);
        DEBUGOUT("Instruction (UID: " << instruct->getUID() << ") Initialization Complete\n");
    }

    DEBUGOUT("Initialize Values - BasicBlock::initialize");
	Value::initialize(irval, vmap);
}