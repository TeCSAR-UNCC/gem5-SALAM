#ifndef __HWACC_LLVM_INTERFACE_HH__

#define __HWACC_LLVM_INTERFACE_HH__

#include "params/LLVMInterface.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/operations.hh"

class LLVMInterface : public ComputeUnit {
  private:
    std::string filename;
    bool running;
    bool finished;
    BasicBlockList *bbList;
    RegisterList *regList;
    BasicBlock *currBB;
    BasicBlock *prevBB;
    ComputeNode *currCompNode;
//    int clock_period;
    int process_delay;
    uint8_t *dataBuffer;
  protected:

  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();
    void constructBBList();
    void startup();
    void copyToBuffer(uint8_t *data, unsigned size);
    void initialize();
};

#endif //__HWACC_LLVM_INTERFACE_HH__
