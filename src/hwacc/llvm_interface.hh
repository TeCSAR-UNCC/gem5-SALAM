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

struct FunctionalUnits {
  int32_t counter_units;
  int32_t int_adder_units;
  int32_t int_multiply_units;
  int32_t int_shifter_units;
  int32_t int_bit_units;
  int32_t fp_sp_adder;
  int32_t fp_dp_adder;
  int32_t fp_sp_multiply;
  int32_t fp_dp_multiply;  
  int32_t compare;
  int32_t gep;
  int32_t conversion;
  int32_t other;  
};

class LLVMInterface : public ComputeUnit {
  private:
    std::string filename;
    uint32_t scheduling_threshold;
    int32_t counter_units;
    int32_t int_adder_units;
    int32_t int_multiply_units;
    int32_t int_shifter_units;
    int32_t int_bit_units;
    int32_t fp_sp_adder;
    int32_t fp_dp_adder;
    int32_t fp_sp_multiply;
    int32_t fp_dp_multiply;
    int32_t compare;
    int32_t gep;
    int32_t conversion;
    int32_t pipelined;
    bool unlimitedFU;
    bool running;
    std::list<BasicBlock*> *bbList;
    RegisterList *regList;
    BasicBlock *currBB;
    BasicBlock *prevBB;
    TypeList *typeList;
    std::vector<InstructionBase*> reservation;
    std::vector<InstructionBase*> readQueue;
    std::vector<InstructionBase*> writeQueue;
    std::vector<InstructionBase*> computeQueue;
    int process_delay;
    int cycle;
    int stalls;
    int execnodes;
    FunctionalUnits _FunctionalUnits;
    FunctionalUnits _MaxFU;
    FunctionalUnits _MaxParsed;
  protected:
    InstructionBase* findParent(Register*);
    InstructionBase* findParent(std::string);
    InstructionBase* detectRAW(Register*);
  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();
    void constructBBList();
    BasicBlock* findBB(std::string bbname);
    void startup();
    void initialize();
    void statistics();
    void scheduleBB(BasicBlock *bb);
    void readCommit(MemoryRequest * req);
    void writeCommit(MemoryRequest * req);
    void detectEdges();
    void initFU();
    void clearFU();
    void updateFU(int8_t FU);
    void maxFU(FunctionalUnits FU);
    bool limitedFU(int8_t FU);
    void updateParsedFU(int8_t FU);
};

#endif //__HWACC_LLVM_INTERFACE_HH__
