#ifndef __HWACC_LLVM_INTERFACE_HH__

#define __HWACC_LLVM_INTERFACE_HH__

#include "params/LLVMInterface.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/operations.hh"

#include <list>
#include <queue>

class LLVMInterface : public ComputeUnit {
  private:
    std::string filename;
    uint32_t numPE;
    bool running;
    std::list<BasicBlock*> *bbList;
    RegisterList *regList;
    BasicBlock *currBB;
    BasicBlock *prevBB;
    ComputeNode *currCompNode;
    std::list<ComputeNode*> *reservation;
    std::queue<ComputeNode*> *readQueue;
    std::queue<ComputeNode*> *writeQueue;
    std::list<ComputeNode*> *computeQueue;
    int process_delay;
    int cycle;
  protected:

  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();
    void constructBBList();
    BasicBlock * findBB(std::string bbname);
    void startup();
    void initialize();

    void scheduleBB(BasicBlock *bb);

    void readCommit(uint8_t * data);
    void writeCommit() { writeQueue->pop(); }
};

#endif //__HWACC_LLVM_INTERFACE_HH__
