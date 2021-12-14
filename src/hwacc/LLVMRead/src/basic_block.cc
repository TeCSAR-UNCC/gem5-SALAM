//------------------------------------------//
#include "basic_block.hh"
#include "llvm/IR/CFG.h"
#include "sim/sim_object.hh"
//------------------------------------------//

using namespace SALAM;

SALAM::BasicBlock::BasicBlock(uint64_t id, gem5::SimObject * owner, bool dbg) :
    SALAM::Value(id, owner, dbg) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbg = true;
    //     this->bb_dbg = new BasicBlock_Debugger();
    // }
}

SALAM::BasicBlock::~BasicBlock() 
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace Deleted: %s \n", __PRETTY_FUNCTION__);
    //// if (DTRACE(SALAM_Debug)) delete bb_dbg;
}

SALAM::BasicBlock::BasicBlock_Debugger::BasicBlock_Debugger()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

void
SALAM::BasicBlock::BasicBlock_Debugger::dumper(SALAM::BasicBlock *bb)
{
    // if (DTRACE(SALAM_Debug)) {
        // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    //     DPRINTF(SALAM_Debug, "%s \n\t\t\t %s %d  \n", 
    //         "************** Basic Block Dump **************",
    //         "UID: ", bb->getUID()
    //     );
    // }
}

void
SALAM::BasicBlock::initialize(llvm::Value * irval, irvmap *vmap, SALAM::valueListTy * valueList) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (dbg) DPRINTFS(LLVMParse, owner, "Initialize Values - BasicBlock::initialize\n");
    Value::initialize(irval, vmap);
	//Parse irval for BasicBlock params
	llvm::BasicBlock * bb = llvm::dyn_cast<llvm::BasicBlock>(irval);
	assert(bb); // panic("Invalid llvm::Value type used to initialize basic block. Failed cast to llvm::BasicBlock.");

    for (auto it = llvm::pred_begin(bb); it != pred_end(bb); ++it) {
        llvm::BasicBlock * predecessor = *it;
        std::shared_ptr<SALAM::BasicBlock> pred =
            std::dynamic_pointer_cast<SALAM::BasicBlock>(vmap->find(predecessor)->second);
        predecessors.push_back(pred);
    }

    if (dbg) DPRINTFS(LLVMParse, owner, "Initialize BasicBlocks\n");
	for (auto inst_iter = bb->begin(); inst_iter != bb->end(); inst_iter++) {
        llvm::Instruction &inst = *inst_iter;
        std::shared_ptr<SALAM::Value> instval = vmap->find(&inst)->second;
        assert(instval);
        std::shared_ptr<SALAM::Instruction> instruct = std::dynamic_pointer_cast<SALAM::Instruction>(instval);
        assert(instruct);
        instructions.push_back(instruct);
        instruct->initialize(&inst, vmap, valueList);
        if (dbg) DPRINTFS(LLVMParse, owner, "Instruction (UID: %d) Initialization Complete\n", instruct->getUID());
    }
}
