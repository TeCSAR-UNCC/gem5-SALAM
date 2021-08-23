#ifndef __HWACC_LLVM_INSTRUCTION_HH__
#define __HWACC_LLVM_INSTRUCTION_HH__

#include <cstdlib>
#include <iostream>
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include "basic_block.hh"
#include "operand.hh"
//#include "cycle_counts.hh" MERGECHECK
#include "debug_flags.hh"
#include "value.hh"
#include "mem_request.hh"

namespace SALAM {

class BasicBlock; // Required Declaration

//---------------------------------------------------------------------------//
//--------- Instruction Base Class ------------------------------------------//
//---------------------------------------------------------------------------//


class Instruction : public Value
{
    private:
        //std::deque<std::shared_ptr<SALAM::Instruction>> dynamicDependencies;
        //std::deque<std::shared_ptr<SALAM::Instruction>> dynamicUsers;
        std::vector<std::shared_ptr<SALAM::Instruction>> dynamicDependencies;
        std::vector<std::shared_ptr<SALAM::Instruction>> dynamicUsers;
        uint64_t llvmOpCode;
        uint64_t cycleCount;
        uint64_t currentCycle;
        bool dbg = false;

    protected:
        valueListTy staticDependencies;
        // Operands
        //std::deque<SALAM::Operand> operands;
        std::vector<SALAM::Operand> operands;

        bool running = false;
        class Instruction_Debugger: public Debugger
        {
            public:
                Instruction_Debugger();
                ~Instruction_Debugger() = default;
                virtual void dumper(SALAM::Instruction *inst);
        };
        Instruction_Debugger* inst_dbg;
        bool launched = false;
        bool committed = false;
        bool isready = false;
    public:
        Instruction(uint64_t id); //
        Instruction(uint64_t id, uint64_t OpCode); //
        Instruction(uint64_t id, uint64_t OpCode, uint64_t cycles); //
        ~Instruction(); //
        bool operator == (const std::shared_ptr<SALAM::Instruction> inst) const { return this->getUID() == inst->getUID(); }
        bool operator != (const std::shared_ptr<SALAM::Instruction> inst) const { return !operator==(inst); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * valueList); //
        virtual std::shared_ptr<SALAM::BasicBlock> getTarget()  { return nullptr; }
        // void instantiate(llvm::Value * irval,
        //                 irvmap * irmap,
        //                 SALAM::valueListTy * valueList); //
        uint64_t getDependencyCount() { return dynamicDependencies.size(); }
        virtual uint64_t getCycleCount() { return cycleCount; }
        uint64_t getOpode() { return llvmOpCode; }
        uint64_t getCurrentCycle() { return currentCycle; }
        virtual valueListTy getStaticDependencies() const { return staticDependencies; }
        //std::deque<std::shared_ptr<SALAM::Instruction>> getDynamicDependencies() const { return dynamicDependencies; }
        std::vector<std::shared_ptr<SALAM::Instruction>> getDynamicDependencies() const { return dynamicDependencies; }
        std::shared_ptr<SALAM::Value> getStaticDependencies(int i) const { return staticDependencies.at(i); }
        std::shared_ptr<SALAM::Value> getDynamicDependencies(int i) const { return dynamicDependencies.at(i); }
        //virtual std::deque<uint64_t> runtimeInitialize();
        virtual std::vector<uint64_t> runtimeInitialize();
        void removeDynamicDependency(int i) { dynamicDependencies.erase(dynamicDependencies.begin()+i); }
        void addRuntimeDependency(std::shared_ptr<SALAM::Instruction> dep) { dynamicDependencies.push_back(dep); }
        void addRuntimeUser(std::shared_ptr<SALAM::Instruction> dep) { dynamicUsers.push_back(dep); }
        void signalUsers();
        bool isCommitted() { return committed; }
        bool debug() { return dbg; }
        void linkOperands(const SALAM::Operand &newOp);
        //std::deque<SALAM::Operand> * getOperands() { return &operands; }
        std::vector<SALAM::Operand> * getOperands() { return &operands; }
        virtual bool isReturn() { return false; }
        virtual bool isTerminator() { return false; }
        virtual bool isPhi() { return false; }
        virtual bool isCall() { return false; }
        virtual bool isBr() { return false; }
        virtual bool isLoad() { return false; }
        virtual bool isStore() {return false; }
        virtual bool isGEP() { return false; }
        virtual bool launch();
        virtual bool commit();
        virtual bool ready();
        virtual void compute() { }
        virtual void reset();
        virtual void setOperandValue(uint64_t uid);
        virtual void dump() { if (dbg) inst_dbg->dumper(this); }
        virtual bool isInstruction() { return true; }
        virtual bool isLoadingInternal() { return false; }
        std::shared_ptr<SALAM::Instruction> clone() const { return std::static_pointer_cast<SALAM::Instruction>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Instruction>(new SALAM::Instruction(*this)); }
        virtual MemoryRequest * createMemoryRequest() { return nullptr; }

        // Functions for getting data from operands
        uint64_t getPtrOperandValue(uint64_t op_num) { return *(operands.at(op_num).getPtrRegValue()); }
};

//---------------------------------------------------------------------------//
//--------- Terminator Instructions -----------------------------------------//
//---------------------------------------------------------------------------//

// SALAM-BadInstruction // --------------------------------------------------//
class BadInstruction : public Instruction {
    // Used to draw hard dependencies, ie: ret
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params

    protected:
    public:
        BadInstruction(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~BadInstruction() = default;
        std::shared_ptr<SALAM::BadInstruction> clone() const { return std::static_pointer_cast<SALAM::BadInstruction>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::BadInstruction>(new SALAM::BadInstruction(*this)); }
};


std::shared_ptr<SALAM::Instruction>
createBadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-Ret // -------------------------------------------------------------//

class Ret : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Ret(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Ret() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        bool isReturn() override { return true; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Ret> clone() const { return std::static_pointer_cast<SALAM::Ret>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Ret>(new SALAM::Ret(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createRetInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-Br // --------------------------------------------------------------//

class Br : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        std::shared_ptr<SALAM::Value> condition;
        std::shared_ptr<SALAM::BasicBlock> defaultDestination;
        std::shared_ptr<SALAM::BasicBlock> trueDestination;
        std::shared_ptr<SALAM::BasicBlock> falseDestination;
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;
        bool conditional = false;

    protected:
    public:
        // Branch Constructor
        Br(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Br() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        Br &isConditional(bool isConditional) { conditional = isConditional; return *this; }
        bool isConditional() { return conditional; }
        std::shared_ptr<SALAM::BasicBlock> getTarget() override;
        bool isTerminator() override { return true; }
        bool isBr() override { return true; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Br> clone() const { return std::static_pointer_cast<SALAM::Br>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Br>(new SALAM::Br(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createBrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-Switch // ----------------------------------------------------------//
typedef std::pair<std::shared_ptr<SALAM::Value>, std::shared_ptr<SALAM::BasicBlock>> caseArgs;
typedef std::vector< caseArgs> switchArgs;

class Switch : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        // [0] [Switch Var, Default Dest]
        // [1] [ Case Var, Case Dest ] .... [n]
        switchArgs cases;
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;
        std::shared_ptr<SALAM::BasicBlock> defaultDestination;
    protected:
    public:
        Switch(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Switch() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        // std::shared_ptr<SALAM::Value> defaultDest() { return arguments[0].second; }
        // std::shared_ptr<SALAM::Value> destination(int switchVar);
        std::shared_ptr<SALAM::BasicBlock> getTarget() override;
        bool isTerminator() override { return true; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Switch> clone() const { return std::static_pointer_cast<SALAM::Switch>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Switch>(new SALAM::Switch(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createSwitchInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- Binary Operator Instructions ------------------------------------//
//---------------------------------------------------------------------------//

// SALAM-Add // -------------------------------------------------------------//

class Add : public Instruction
{
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Add(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Add() = default;
        void initialize(llvm::Value *irval,
                        SALAM::irvmap *irmap,
                        SALAM::valueListTy *valueList) override;
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Add> clone() const { return std::static_pointer_cast<SALAM::Add>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Add>(new SALAM::Add(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-FAdd // ------------------------------------------------------------//

class FAdd : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FAdd(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FAdd() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FAdd> clone() const { return std::static_pointer_cast<SALAM::FAdd>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FAdd>(new SALAM::FAdd(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Sub // -------------------------------------------------------------//

class Sub : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Sub(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Sub() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Sub> clone() const { return std::static_pointer_cast<SALAM::Sub>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Sub>(new SALAM::Sub(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FSub // -------------------------------------------------------------//

class FSub : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FSub(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FSub() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FSub> clone() const { return std::static_pointer_cast<SALAM::FSub>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FSub>(new SALAM::FSub(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Mul // -------------------------------------------------------------//

class Mul : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::APIntRegister *op1, *op2;
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Mul(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Mul() = default;
        void initialize (llvm::Value * irval,
                        SALAM::irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Mul> clone() const { return std::static_pointer_cast<SALAM::Mul>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Mul>(new SALAM::Mul(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FMul // ------------------------------------------------------------//

class FMul : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FMul(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FMul() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FMul> clone() const { return std::static_pointer_cast<SALAM::FMul>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FMul>(new SALAM::FMul(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-UDiv // ------------------------------------------------------------//

class UDiv : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        UDiv(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~UDiv() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::UDiv> clone() const { return std::static_pointer_cast<SALAM::UDiv>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::UDiv>(new SALAM::UDiv(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createUDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SDiv // ------------------------------------------------------------//

class SDiv : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        SDiv(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SDiv() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::SDiv> clone() const { return std::static_pointer_cast<SALAM::SDiv>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::SDiv>(new SALAM::SDiv(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createSDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FDiv // ------------------------------------------------------------//

class FDiv : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FDiv(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FDiv() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FDiv> clone() const { return std::static_pointer_cast<SALAM::FDiv>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FDiv>(new SALAM::FDiv(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-URem // ------------------------------------------------------------//

class URem : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        URem(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~URem() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::URem> clone() const { return std::static_pointer_cast<SALAM::URem>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::URem>(new SALAM::URem(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createURemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SRem // ------------------------------------------------------------//

class SRem : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        SRem(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SRem() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::SRem> clone() const { return std::static_pointer_cast<SALAM::SRem>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::SRem>(new SALAM::SRem(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createSRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FRem // ------------------------------------------------------------//

class FRem : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FRem(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FRem() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FRem> clone() const { return std::static_pointer_cast<SALAM::FRem>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FRem>(new SALAM::FRem(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- Bitwise Binary Operator Instructions ----------------------------//
//---------------------------------------------------------------------------//

// SALAM-Shl // -------------------------------------------------------------//

class Shl : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Shl(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Shl() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Shl> clone() const { return std::static_pointer_cast<SALAM::Shl>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Shl>(new SALAM::Shl(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createShlInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-LShr // ------------------------------------------------------------//

class LShr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        LShr(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~LShr() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::LShr> clone() const { return std::static_pointer_cast<SALAM::LShr>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::LShr>(new SALAM::LShr(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createLShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-AShr // ------------------------------------------------------------//

class AShr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        AShr(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~AShr() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::AShr> clone() const { return std::static_pointer_cast<SALAM::AShr>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::AShr>(new SALAM::AShr(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createAShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-And // -------------------------------------------------------------//

class And : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        And(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~And() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::And> clone() const { return std::static_pointer_cast<SALAM::And>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::And>(new SALAM::And(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createAndInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Or // --------------------------------------------------------------//

class Or : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;
    protected:

    public:
        Or(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Or() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Or> clone() const { return std::static_pointer_cast<SALAM::Or>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Or>(new SALAM::Or(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createOrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Xor // -------------------------------------------------------------//

class Xor : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Xor(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Xor() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Xor> clone() const { return std::static_pointer_cast<SALAM::Xor>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Xor>(new SALAM::Xor(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createXorInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- Memory Instructions ---------------------------------------------//
//---------------------------------------------------------------------------//

// SALAM-Load // ------------------------------------------------------------//

class Load : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        uint64_t align;
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;
        bool loadingInternal = false;

    protected:
    public:
        Load(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Load() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        bool isLoad() override { return true; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void loadInternal();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        bool isLoadingInternal() { return loadingInternal; }
        std::shared_ptr<SALAM::Load> clone() const { return std::static_pointer_cast<SALAM::Load>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Load>(new SALAM::Load(*this)); }

        MemoryRequest * createMemoryRequest() override;
};

std::shared_ptr<SALAM::Instruction>
createLoadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Store // -----------------------------------------------------------//

class Store : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        uint64_t align;
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Store (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Store() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        bool isStore() override { return true; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Store> clone() const { return std::static_pointer_cast<SALAM::Store>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Store>(new SALAM::Store(*this)); }

        MemoryRequest * createMemoryRequest() override;
};

std::shared_ptr<SALAM::Instruction>
createStoreInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-GEP // -------------------------------------------------------------//

/*

In our storage, pointers are standard uint64_t, for comm interface convience
The GEP indecies will by APSInts, so cast to int64_t for calculating offset inside GEP, then recast to APSInt

*/

class GetElementPtr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        std::vector<llvm::Type *> indexTypes;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        //uint64_t currentCycle;
        llvm::Type * resultElementType;
        uint64_t resultElementSize;
        uint64_t resultElementSizeInBytes;
        llvm::DataLayout * layout;

    protected:

    public:
        GetElementPtr(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~GetElementPtr() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        GetElementPtr &setA() { std::cout << "a\n"; return *this; }
        GetElementPtr &setB() { std::cout << "b\n"; return *this; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void setDataLayout(llvm::DataLayout * dl) { layout = dl; }
        virtual bool isGEP() override { return true; }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::GetElementPtr> clone() const { return std::static_pointer_cast<SALAM::GetElementPtr>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::GetElementPtr>(new SALAM::GetElementPtr(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createGetElementPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- Other / Cast Instructions ---------------------------------------//
//---------------------------------------------------------------------------//

// SALAM-Trunc // -----------------------------------------------------------//

class Trunc : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        Trunc(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Trunc() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Trunc> clone() const { return std::static_pointer_cast<SALAM::Trunc>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Trunc>(new SALAM::Trunc(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-ZExt // ------------------------------------------------------------//

class ZExt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        ZExt(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~ZExt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::ZExt> clone() const { return std::static_pointer_cast<SALAM::ZExt>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::ZExt>(new SALAM::ZExt(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createZExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SExt // ------------------------------------------------------------//

class SExt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        SExt(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SExt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::SExt> clone() const { return std::static_pointer_cast<SALAM::SExt>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::SExt>(new SALAM::SExt(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createSExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FPToUI // ----------------------------------------------------------//
class FPToUI;
void initializeFPToUIInst(SALAM::FPToUI &salamInstruction);

class FPToUI : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FPToUI(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPToUI() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FPToUI> clone() const { return std::static_pointer_cast<SALAM::FPToUI>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FPToUI>(new SALAM::FPToUI(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFPToUIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FPToSI // ----------------------------------------------------------//

class FPToSI : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:

    public:
        FPToSI(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPToSI() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FPToSI> clone() const { return std::static_pointer_cast<SALAM::FPToSI>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FPToSI>(new SALAM::FPToSI(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFPToSIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-UIToFP // ----------------------------------------------------------//

class UIToFP : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        UIToFP(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~UIToFP() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::UIToFP> clone() const { return std::static_pointer_cast<SALAM::UIToFP>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::UIToFP>(new SALAM::UIToFP(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createUIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SIToFP // ----------------------------------------------------------//

class SIToFP : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        SIToFP(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SIToFP() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::SIToFP> clone() const { return std::static_pointer_cast<SALAM::SIToFP>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::SIToFP>(new SALAM::SIToFP(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createSIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FPTrunc // ---------------------------------------------------------//

class FPTrunc : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FPTrunc(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPTrunc() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FPTrunc> clone() const { return std::static_pointer_cast<SALAM::FPTrunc>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FPTrunc>(new SALAM::FPTrunc(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFPTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FPExt // -----------------------------------------------------------//

class FPExt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        FPExt(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPExt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FPExt> clone() const { return std::static_pointer_cast<SALAM::FPExt>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FPExt>(new SALAM::FPExt(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFPExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-PtrToInt // --------------------------------------------------------//

class PtrToInt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:

    public:
        PtrToInt (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~PtrToInt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::PtrToInt> clone() const { return std::static_pointer_cast<SALAM::PtrToInt>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::PtrToInt>(new SALAM::PtrToInt(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createPtrToIntInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-IntToPtr // --------------------------------------------------------//

class IntToPtr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        IntToPtr(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~IntToPtr() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::IntToPtr> clone() const { return std::static_pointer_cast<SALAM::IntToPtr>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::IntToPtr>(new SALAM::IntToPtr(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createIntToPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- Other / Comparison Instructions ---------------------------------//
//---------------------------------------------------------------------------//

// SALAM-ICmp // ------------------------------------------------------------//

class ICmp : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        uint64_t predicate;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:

    public:
        ICmp(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~ICmp() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::ICmp> clone() const { return std::static_pointer_cast<SALAM::ICmp>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::ICmp>(new SALAM::ICmp(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createICmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FCmp // ------------------------------------------------------------//

class FCmp : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        uint64_t predicate;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:

    public:
        FCmp(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FCmp() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::FCmp> clone() const { return std::static_pointer_cast<SALAM::FCmp>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::FCmp>(new SALAM::FCmp(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createFCmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- Other / Edge Instructions ---------------------------------------//
//---------------------------------------------------------------------------//

// SALAM-Phi // -------------------------------------------------------------//

typedef std::pair<std::shared_ptr<SALAM::BasicBlock>, std::shared_ptr<SALAM::Value>> phiArgTy;

//typedef std::pair<std::shared_ptr<SALAM::Value>, std::shared_ptr<SALAM::Value> > phiNode;
typedef std::map<std::shared_ptr<SALAM::BasicBlock>, std::shared_ptr<SALAM::Value>> phiArgsTy;

class Phi : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        std::shared_ptr<SALAM::BasicBlock> previousBB;
        phiArgsTy phiArgs; // [BasicBlock, Value]
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:

    public:
        Phi (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Phi() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        //virtual std::deque<uint64_t> runtimeInitialize() override;
        virtual std::vector<uint64_t> runtimeInitialize() override;
        bool isPhi() override { return true; }
        void setPrevBB(std::shared_ptr<SALAM::BasicBlock> prevBB);
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        virtual valueListTy getStaticDependencies() const override;
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Phi> clone() const { return std::static_pointer_cast<SALAM::Phi>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Phi>(new SALAM::Phi(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createPHIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Call // ------------------------------------------------------------//

class Call : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;
        std::shared_ptr<SALAM::Value> callee;
    protected:
    public:
        Call (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Call() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        bool isCall() override { return true; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Value> getCalleeValue() { return callee; }
        std::shared_ptr<SALAM::Call> clone() const { return std::static_pointer_cast<SALAM::Call>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Call>(new SALAM::Call(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createCallInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Select // ----------------------------------------------------------//

class Select : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // conditions.at[0] == base params
        std::shared_ptr<SALAM::Value> condition;
        std::shared_ptr<SALAM::Value> trueValue;
        std::shared_ptr<SALAM::Value> falseValue;
        SALAM::Debugger *dbgr;
        uint64_t currentCycle;

    protected:
    public:
        // ---- Constructor
        Select (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Select() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        // std::shared_ptr<SALAM::Value> evaluate();
        // bool isTerminator() override { return true; }
        uint64_t getCycleCount() { return conditions.at(0).at(2); }
        void compute();
        void dump() { if (dbgr->enabled()) { dumper(); inst_dbg->dumper(static_cast<SALAM::Instruction*>(this));}}
        void dumper();
        std::shared_ptr<SALAM::Select> clone() const { return std::static_pointer_cast<SALAM::Select>(createClone()); }
        virtual std::shared_ptr<SALAM::Value> createClone() const override { return std::shared_ptr<SALAM::Select>(new SALAM::Select(*this)); }
};

std::shared_ptr<SALAM::Instruction>
createSelectInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- End Instruction Classes -----------------------------------------//
//---------------------------------------------------------------------------//

}

#endif // __HWACC_LLVM_INSTRUCTION_HH__