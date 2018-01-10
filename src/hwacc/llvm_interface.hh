#ifndef __HWACC_LLVM_INTERFACE_HH__

#define __HWACC_LLVM_INTERFACE_HH__

#include "params/LLVMInterface.hh"
#include "sim/sim_object.hh"
#include "hwacc/io_acc.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/operations.hh"

class LLVMInterface : public SimObject {
  private:
    IOAcc *acc;
    bool running;
    bool finished;
    BasicBlockList *bbList;
    BasicBlock *currBB;
    BasicBlock *prevBB;
    ComputeNode *currCompNode;

    class TickEvent : public Event
    {
      private:
        LLVMInterface *llvm_int;

      public:
        TickEvent(LLVMInterface *_llvm_int) : Event(CPU_Tick_Pri), llvm_int(_llvm_int) {}
        void process() { llvm_int->tick(); }
        virtual const char *description() const { return "LLVMInterface tick"; }
    };

    TickEvent tickEvent;

  protected:

  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();

};

#endif //__HWACC_LLVM_INTERFACE_HH__
