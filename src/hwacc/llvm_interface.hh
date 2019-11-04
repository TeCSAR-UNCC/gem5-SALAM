#ifndef __HWACC_LLVM_INTERFACE_HH__
#define __HWACC_LLVM_INTERFACE_HH__
#include "params/LLVMInterface.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/data_collection.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/llvm_types.hh"
#include "hwacc/LLVMRead/src/debugFlags.hh"
#include "hwacc/LLVMRead/src/utilization.hh"
#include "hwacc/LLVMRead/src/macros.hh"
#include "hwacc/LLVMRead/src/cycle_count.hh"

#include <list>
#include <queue>

class LLVMInterface : public ComputeUnit {
  private:
    std::string filename;
    bool lockstep;
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
    bool scheduled;
    std::list<BasicBlock*> *bbList;
    RegisterList *regList;
    BasicBlock *currBB;
    BasicBlock *prevBB;
    TypeList *typeList;
    Results *results;
    std::vector<InstructionBase*> reservation;
    std::vector<InstructionBase*> readQueue;
    std::vector<InstructionBase*> writeQueue;
    std::vector<InstructionBase*> computeQueue;
    int process_delay;
    int cycle;
    int stalls;
    int loadOnly; //
    int storeOnly; //
    int compOnly; //
    int loadStore; //
    int loadComp;
    int loadStoreComp;
    int storeComp;
    int loadOnlyStall; 
    int storeOnlyStall; 
    int compOnlyStall; 
    int loadStoreStall; 
    int loadCompStall;
    int loadStoreCompStall;
    int storeCompStall;
    int execnodes;
    int clock_period;
    int global_loads;
    int global_stores;
    int memory_loads;
    int memory_stores;
    int dma_loads;
    int dma_stores;
    int read_ports;
    int write_ports;
    int cache_size;
    int spm_size;
    int cache_ports;
    int local_ports;
    int fu_clock_period;
    int read_bus_width;
    int write_bus_width;
    FunctionalUnits _FunctionalUnits;
    FunctionalUnits _MaxFU;
    FunctionalUnits _MaxParsed;
  protected:
    InstructionBase* findParent(Register*);
    InstructionBase* findParent(std::string);
    InstructionBase* detectRAW(Register*);
    Utilization* pwrUtil;
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
