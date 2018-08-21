#ifndef __HWACC_LLVM_INTERFACE_HH__
#define __HWACC_LLVM_INTERFACE_HH__
#include "params/LLVMInterface.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/llvm_types.hh"
#include "hwacc/LLVMRead/src/debugFlags.hh"
#include "hwacc/LLVMRead/src/utilization.hh"

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
    TypeList *typeList;
    std::list<ComputeNode*> *reservation;
    std::list<ComputeNode*> *readQueue;
    std::list<ComputeNode*> *writeQueue;
    std::list<ComputeNode*> *computeQueue;
    int process_delay;
    int cycle;
    int stalls;
    int execnodes;
    Utilization *pwrUtil;
    typedef std::map<std::string, int> opCodeCount;
    opCodeCount opCount;
  protected:

  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();
    void constructBBList();
    BasicBlock * findBB(std::string bbname);
    void startup();
    void initialize();
    void statistics();
    void scheduleBB(BasicBlock *bb);
    void readCommit(MemoryRequest * req);
    void writeCommit(MemoryRequest * req);
};

#endif //__HWACC_LLVM_INTERFACE_HH__
