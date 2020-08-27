#ifndef __HWACC_LLVM_INTERFACE_HH__
#define __HWACC_LLVM_INTERFACE_HH__
//------------------------------------------//
#include "params/LLVMInterface.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/function.hh"
#include "hwacc/LLVMRead/src/llvm_types.hh"
#include "hwacc/LLVMRead/src/debug_flags.hh"
#include "hwacc/LLVMRead/src/utilization.hh"
#include "hwacc/LLVMRead/src/cycle_count.hh"
#include "hwacc/LLVMRead/src/ir_parse.hh"
//------------------------------------------//
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm-c/Core.h>
//------------------------------------------//
#include <list>
#include <queue>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <ratio>
#include <memory>
//------------------------------------------//

class LLVMInterface : public ComputeUnit {
  private:
    std::string filename;
    llvm::LLVMContext context;
    llvm::SMDiagnostic error;
    bool lockstep;
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
    bool unlimitedFU;
    bool running;
    bool loadOpScheduled;
    bool storeOpScheduled;
    bool compOpScheduled;
    std::chrono::duration<double> setupTime;
    std::chrono::duration<double> simTime;
    std::chrono::high_resolution_clock::time_point simStop;
    std::chrono::high_resolution_clock::time_point setupStop;
    std::chrono::high_resolution_clock::time_point timeStart;
    std::vector<SALAM::Instruction*> reservation;
    std::vector<SALAM::Instruction*> readQueue;
    std::vector<SALAM::Instruction*> writeQueue;
    std::vector<SALAM::Instruction*> computeQueue;
    // std::list<SALAM::BasicBlock*> *bbList;
    std::vector<std::shared_ptr<SALAM::Function>> functions;
    std::vector<std::shared_ptr<SALAM::Value>> values;
    RegisterList *regList;
    // SALAM::BasicBlock *currBB;
    // SALAM::BasicBlock *prevBB;
    TypeList *typeList;
    Hardware* hardware;
  protected:
    // InstructionBase* findParent(Register*);
    // InstructionBase* findParent(std::string);
    // InstructionBase* detectRAW(Register*);
    const std::string name() const { return comm->getName() + ".compute"; }
    virtual bool debug() { return comm->debug(); }
  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();
    void constructStaticGraph();
    // SALAM::BasicBlock* findBB(std::string bbname);
    // SALAM::BasicBlock* findEntryBB();
    void startup();
    void initialize();
    void printPerformanceResults();
    void finalize();
    void occupancy();
    //void statisticsWithMemory();
    // void scheduleBB(SALAM::BasicBlock *bb);
    void readCommit(MemoryRequest * req);
    void writeCommit(MemoryRequest * req);
    void dumpQueues();
    void dumpModule(llvm::Module *m);
    std::shared_ptr<SALAM::Instruction> createInstruction(llvm::Instruction * inst, uint64_t id);
};

#endif //__HWACC_LLVM_INTERFACE_HH__
