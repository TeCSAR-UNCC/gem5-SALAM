#ifndef __HWACC_LLVM_INTERFACE_HH__
#define __HWACC_LLVM_INTERFACE_HH__
//------------------------------------------//
#include "params/LLVMInterface.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/LLVMRead/src/operand.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/function.hh"
#include "hwacc/LLVMRead/src/llvm_types.hh"
#include "hwacc/LLVMRead/src/debug_flags.hh"
#include "hwacc/LLVMRead/src/cycle_counts.hh"
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
#include <map>
#include <typeinfo>
#include <type_traits>
#include <queue>
#include <deque>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <ratio>
#include <memory>
#include <algorithm>
//------------------------------------------//

class LLVMInterface : public ComputeUnit {
  private:
    std::string filename;
    std::string topName;
    llvm::LLVMContext context;
    llvm::SMDiagnostic error;
    llvm::DataLayout * layout;
    uint32_t scheduling_threshold;
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
    bool lockstep;
    bool dbg;
    std::chrono::duration<float> setupTime;
    std::chrono::duration<float> simTime;
    std::chrono::duration<float> schedulingTime;
    std::chrono::duration<float> queueProcessTime;
    std::chrono::duration<float> computeTime;
    std::chrono::high_resolution_clock::time_point simStop;
    std::chrono::high_resolution_clock::time_point setupStop;
    std::chrono::high_resolution_clock::time_point timeStart;

    class ActiveFunction {
    private:
        LLVMInterface * owner;
        std::shared_ptr<SALAM::Function> func;
        std::shared_ptr<SALAM::Instruction> caller;
        std::list<std::shared_ptr<SALAM::Instruction>> reservation;
        std::map<uint64_t, std::shared_ptr<SALAM::Instruction>> readQueue;
        std::map<MemoryRequest *, uint64_t> readQueueMap;
        std::map<uint64_t, std::shared_ptr<SALAM::Instruction>> writeQueue;
        std::map<MemoryRequest *, uint64_t> writeQueueMap;
        std::map<uint64_t, std::shared_ptr<SALAM::Instruction>> computeQueue;
        std::shared_ptr<SALAM::BasicBlock> previousBB;
        uint32_t scheduling_threshold;
        bool returned = false;
        bool lockstep;

        inline bool uidActive(uint64_t id) {
          return computeUIDActive(id) || readUIDActive(id) || writeUIDActive(id);
        }

        std::map<Addr, std::shared_ptr<SALAM::Instruction>> activeWrites;
        inline void trackWrite(Addr writeAddr, std::shared_ptr<SALAM::Instruction> writeInst) {
          activeWrites.insert({writeAddr, writeInst});
        }
        inline void untrackWrite(uint64_t writeAddr) {
          auto it = activeWrites.find(writeAddr);
          if (it != activeWrites.end()) activeWrites.erase(it);
        }
        inline bool writeActive(uint64_t writeAddr) {
          return (activeWrites.find(writeAddr) != activeWrites.end());
        }

        inline std::shared_ptr<SALAM::Instruction> getActiveWrite(uint64_t writeAddr) {
          return activeWrites.find(writeAddr)->second;
        }

        inline bool writeUIDActive(uint64_t uid) {
          return (writeQueue.find(uid) != writeQueue.end());
        }
        inline bool readUIDActive(uint64_t uid) {
          return (readQueue.find(uid) != readQueue.end());
        }
        inline bool computeUIDActive(uint64_t uid) {
          return (computeQueue.find(uid) != computeQueue.end());
        }
    public:
        ActiveFunction(LLVMInterface * _owner, std::shared_ptr<SALAM::Function> _func,
                       std::shared_ptr<SALAM::Instruction> _caller):
                       owner(_owner), func(_func), caller(_caller),
                       previousBB(nullptr) {
                          scheduling_threshold = owner->getSchedulingThreshold();
                          lockstep = (owner->getLockstepStatus());
                       }
        void readCommit(MemoryRequest *req);
        void writeCommit(MemoryRequest *req);
        void findDynamicDeps(std::shared_ptr<SALAM::Instruction> inst);
        void scheduleBB(std::shared_ptr<SALAM::BasicBlock> bb);
        void processQueues();
        void launch();
        inline bool lockstepReady() {
          return !lockstep || (readQueue.empty() && writeQueue.empty() && computeQueue.empty());
        }
        inline bool canReturn() {
            return readQueue.empty() && writeQueue.empty() && computeQueue.empty() && reservation.front()->isReturn();
        }
        void launchRead(std::shared_ptr<SALAM::Instruction> readInst);
        void launchWrite(std::shared_ptr<SALAM::Instruction> writeInst);
        bool hasReturned() { return returned; }
    };

    std::list<ActiveFunction> activeFunctions;
    std::map<MemoryRequest *, ActiveFunction *> globalReadQueue;
    std::map<MemoryRequest *, ActiveFunction *> globalWriteQueue;

    std::vector<std::shared_ptr<SALAM::Function>> functions;
    std::vector<std::shared_ptr<SALAM::Value>> values;
    TypeList *typeList;
  protected:
    const std::string name() const { return comm->getName() + ".compute"; }
    virtual bool debug() { return comm->debug(); }
    // virtual bool debug() { return true; }
  public:
    LLVMInterface(LLVMInterfaceParams *p);
    void tick();
    void constructStaticGraph();
    void startup();
    void initialize();
    void finalize();
    void debug(uint64_t flags);
    bool getLockstepStatus() { return lockstep; }
    void readCommit(MemoryRequest *req);
    void writeCommit(MemoryRequest *req);
    void dumpModule(llvm::Module *m);
    void printPerformanceResults();
    void launchFunction(std::shared_ptr<SALAM::Function> callee,
                        std::shared_ptr<SALAM::Instruction> caller);
    void launchTopFunction();
    void endFunction(ActiveFunction * afunc);
    void launchRead(MemoryRequest * memReq, ActiveFunction * func);
    void launchWrite(MemoryRequest * memReq, ActiveFunction * func);
    std::shared_ptr<SALAM::Instruction> createInstruction(llvm::Instruction *inst,
                                                          uint64_t id);
    void dumpQueues();
    uint32_t getSchedulingThreshold() { return scheduling_threshold; }
    llvm::DataLayout * getDataLayout() { return layout; }
    void addSchedulingTime(std::chrono::duration<float> timeDelta) { schedulingTime = schedulingTime + timeDelta; }
    void addQueueTime(std::chrono::duration<float> timeDelta) { queueProcessTime = queueProcessTime + timeDelta; }
    void addComputeTime(std::chrono::duration<float> timeDelta) { computeTime = computeTime + timeDelta; }
};

#endif //__HWACC_LLVM_INTERFACE_HH__
