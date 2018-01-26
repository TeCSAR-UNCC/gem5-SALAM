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
    int clock_delay;
    int process_delay;

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
    void constructBBList();
    void startup();
};

#endif //__HWACC_LLVM_INTERFACE_HH__
