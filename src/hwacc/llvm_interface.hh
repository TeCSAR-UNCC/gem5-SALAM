#ifndef __HWACC_LLVM_INTERFACE_HH__
#define __HWACC_LLVM_INTERFACE_HH__
//------------------------------------------//
#include "params/LLVMInterface.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/data_collection.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/llvm_types.hh"
#include "hwacc/LLVMRead/src/debug_flags.hh"
#include "hwacc/LLVMRead/src/utilization.hh"
#include "hwacc/LLVMRead/src/cycle_count.hh"
//------------------------------------------//
// #include <llvm/IR/Module.h>
// #include <llvm/IRReader/IRReader.h>
// #include <llvm/IR/LLVMContext.h>
// #include <llvm/Support/SourceMgr.h>
//------------------------------------------//
#include <list>
#include <queue>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <ratio>
//------------------------------------------//

class LLVMInterface : public ComputeUnit {
  private:
    std::string filename;
    bool lockstep;
    bool unlimitedFU;
    bool running;
    bool loadOpScheduled;
    bool storeOpScheduled;
    bool compOpScheduled;
    bool scheduled;
    bool occupancy_tracking = true;
    uint32_t scheduling_threshold;
    // Functional Units
    // -1 = Runtime Defined
    //  0 = Static LLVM Defined
    // >0 = User defined  
    int32_t counter_units;
    int32_t int_adder_units;
    int32_t int_multiply_units;
    int32_t int_shifter_units;
    int32_t int_bit_units;
    int32_t fp_sp_adder;
    int32_t fp_dp_adder;
    int32_t fp_sp_multiply;
    int32_t fp_sp_division; // Add config
    int32_t fp_dp_multiply;
    int32_t fp_dp_division; // Add config
    int32_t compare;
    int32_t gep;
    int32_t conversion;
    int32_t pipelined;
    int32_t fu_latency;
    int32_t clock_period;
    int process_delay;
    int cycle;
    int stalls;
    int loadInFlight;
    int storeInFlight;
    int compInFlight;
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
    double exponential = 1e9;
    double leak = 1.0;
    float runtime;
    FunctionalUnits _FunctionalUnits;
    FunctionalUnits _MaxFU;
    FunctionalUnits _MaxParsed;
    FunctionalUnitOccupancy _AverageOccupancy;
    FunctionalUnits _CycleOccupancy;
    FunctionalUnitOccupancy _RunningAverageOccupancy;
    std::list<FunctionalUnits> *_CycleOccList;
    CactiResults _SPM;
    CactiResults _Cache;
    std::chrono::duration<double> setupTime;
    std::chrono::duration<double> simTime;
    std::chrono::high_resolution_clock::time_point simStop;
    std::chrono::high_resolution_clock::time_point setupStop;
    std::chrono::high_resolution_clock::time_point timeStart;
    std::vector<InstructionBase*> reservation;
    std::vector<InstructionBase*> readQueue;
    std::vector<InstructionBase*> writeQueue;
    std::vector<InstructionBase*> computeQueue;
    std::list<BasicBlock*> *bbList;
    RegisterList *regList;
    BasicBlock *currBB;
    BasicBlock *prevBB;
    TypeList *typeList;    
    Hardware* hardware;
    Results *results;
  protected:
    InstructionBase* findParent(Register*);
    InstructionBase* findParent(std::string);
    InstructionBase* detectRAW(Register*);
    const std::string name() const { return comm->getName() + ".compute"; }
    virtual bool debug() { return comm->debug(); }
    Utilization* pwrUtil;
  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();
    void constructBBList();
    BasicBlock* findBB(std::string bbname);
    BasicBlock* findEntryBB();
    void startup();
    void initialize();
    void printPerformanceResults();
    void printOccupancyList();
    void finalize();
    void occupancy();
    void detectEdges();
    void initFU();
    void clearFU();
    void updateFU(int8_t FU);
    void maxFU(FunctionalUnits FU);
    void Occupancy(FunctionalUnits FU);
    void AverageOccupancy();
    bool limitedFU(int8_t FU);
    void updateParsedFU(int8_t FU);
    //void statisticsWithMemory();
    void scheduleBB(BasicBlock *bb);
    void readCommit(MemoryRequest * req);
    void writeCommit(MemoryRequest * req);
    bool unlimitedMode();
    void dumpQueues();
};

#endif //__HWACC_LLVM_INTERFACE_HH__
