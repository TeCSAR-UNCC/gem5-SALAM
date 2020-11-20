#ifndef __HWACC_LLVM_INSTRUCTION_HH__
#define __HWACC_LLVM_INSTRUCTION_HH__

#include <cstdlib>
#include <iostream>
#include "value.hh"
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include "operand.hh"
#include "cycle_count.hh"
//#include "basic_block.hh"
#include "debug_flags.hh"


namespace SALAM {

//---------------------------------------------------------------------------//
//--------- Instruction Base Class ------------------------------------------//
//---------------------------------------------------------------------------//

class Instruction : public Value, public Operand
{
    private:
        Value *returnRegister;
        valueListTy staticOperands;
        std::vector<Instruction *> dynamicDependencies;
        std::vector<Instruction *> dynamicUsers;
        std::vector<Operand *> opList;
        uint64_t llvmOpCode;
        uint64_t cycleCount;
        
    protected:
    public:
        Instruction(uint64_t id);
        Instruction(uint64_t id, uint64_t OpCode);
        Instruction(uint64_t id, uint64_t OpCode, uint64_t cycles);
        ~Instruction() = default;
        virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * valueList) { };
        void instantiate(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void debug();
        // Create initialize function for instruction - common item containers
        valueListTy getStaticOperands() const { return staticOperands; }
        std::shared_ptr<SALAM::Value> getStaticOperands(int i) const { return staticOperands.at(i); }
        void signalUsers();
        virtual void compute() { }
        virtual void fetchDependencyVal(Instruction * dep) {} //TODO: This will be changed to purely virtual
        virtual Instruction* clone() const { return new Instruction(*this); }
};

//---------------------------------------------------------------------------//
//--------- Terminator Instructions -----------------------------------------//
//---------------------------------------------------------------------------//

// SALAM-BadInstruction // --------------------------------------------------//
class BadInstruction : public Instruction {
    // Used to draw hard dependencies, ie: ret
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        BadInstruction(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~BadInstruction() = default;
        void compute()      override { }
        virtual BadInstruction* clone() const { return new BadInstruction(*this); }
};


std::shared_ptr<SALAM::Instruction>
createBadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-Ret // -------------------------------------------------------------//

class Ret : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        
    protected:

    public:
        Ret(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Ret() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
       // void getCycleCount() override { setCycleCount(cycles->ret_inst); }
        virtual Ret* clone() const { return new Ret(*this); }
};

std::shared_ptr<SALAM::Instruction>
createRetInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-Br // --------------------------------------------------------------//

class Br : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        std::shared_ptr<SALAM::Value> condition;
        std::shared_ptr<SALAM::Value> defaultDestination;
        std::shared_ptr<SALAM::Value> trueDestination;
        std::shared_ptr<SALAM::Value> falseDestination;
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
        std::shared_ptr<SALAM::Value> destination();
        void compute()      override;
        virtual Br* clone() const { return new Br(*this); }
};

std::shared_ptr<SALAM::Instruction>
createBrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-Switch // ----------------------------------------------------------//
typedef std::pair<std::shared_ptr<SALAM::Value>, std::shared_ptr<SALAM::Value>> caseArgs;
typedef std::vector< caseArgs> switchArgs;

class Switch : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // [0] [Switch Var, Default Dest]
        // [1] [ Case Var, Case Dest ] .... [n]
        switchArgs arguments;
    protected:

    public:
        Switch(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Switch() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        std::shared_ptr<SALAM::Value> defaultDest() { return arguments[0].second; }
        std::shared_ptr<SALAM::Value> destination(int switchVar);
        void compute()      override;
        virtual Switch* clone() const {  return new Switch(*this); }
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
    protected:

    public:
        Add(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Add() = default;
        void initialize(llvm::Value *irval,
                        SALAM::irvmap *irmap,
                        SALAM::valueListTy *valueList) override;
        void compute() override;
       // void getCycleCount() override { setCycleCount(cycles->add_inst); }
        virtual Add* clone() const { return new Add(*this); }
};

std::shared_ptr<SALAM::Instruction>
createAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);

// SALAM-FAdd // ------------------------------------------------------------//

class FAdd : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FAdd(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FAdd() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FAdd* clone() const { return new FAdd(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Sub // -------------------------------------------------------------//

class Sub : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        Sub(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Sub() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Sub* clone() const { return new Sub(*this); }
};

std::shared_ptr<SALAM::Instruction>
createSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FSub // -------------------------------------------------------------//

class FSub : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FSub(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FSub() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FSub* clone() const { return new FSub(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Mul // -------------------------------------------------------------//

class Mul : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        SALAM::APIntRegister *op1, *op2;
    protected:

    public:
        Mul(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Mul() = default;
        void initialize (llvm::Value * irval,
                        SALAM::irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Mul* clone() const { return new Mul(*this); }
};

std::shared_ptr<SALAM::Instruction>
createMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FMul // ------------------------------------------------------------//

class FMul : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FMul(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FMul() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FMul* clone() const { return new FMul(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-UDiv // ------------------------------------------------------------//

class UDiv : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        UDiv(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~UDiv() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual UDiv* clone() const { return new UDiv(*this); }
};

std::shared_ptr<SALAM::Instruction>
createUDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SDiv // ------------------------------------------------------------//

class SDiv : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        SDiv(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SDiv() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual SDiv* clone() const { return new SDiv(*this); }
};

std::shared_ptr<SALAM::Instruction>
createSDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FDiv // ------------------------------------------------------------//

class FDiv : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FDiv(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FDiv() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FDiv* clone() const { return new FDiv(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-URem // ------------------------------------------------------------//

class URem : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        URem(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~URem() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual URem* clone() const { return new URem(*this); }
};

std::shared_ptr<SALAM::Instruction>
createURemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SRem // ------------------------------------------------------------//

class SRem : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        SRem(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SRem() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual SRem* clone() const { return new SRem(*this); }
};

std::shared_ptr<SALAM::Instruction>
createSRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FRem // ------------------------------------------------------------//

class FRem : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FRem(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FRem() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FRem* clone() const { return new FRem(*this); }
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
    protected:

    public:
        Shl(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Shl() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Shl* clone() const { return new Shl(*this); }
};

std::shared_ptr<SALAM::Instruction>
createShlInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-LShr // ------------------------------------------------------------//

class LShr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        LShr(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~LShr() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual LShr* clone() const { return new LShr(*this); }
};

std::shared_ptr<SALAM::Instruction>
createLShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-AShr // ------------------------------------------------------------//

class AShr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        AShr(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~AShr() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual AShr* clone() const { return new AShr(*this); }
};

std::shared_ptr<SALAM::Instruction>
createAShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-And // -------------------------------------------------------------//

class And : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        And(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~And() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual And* clone() const { return new And(*this); }
};

std::shared_ptr<SALAM::Instruction>
createAndInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Or // --------------------------------------------------------------//

class Or : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        Or(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Or() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Or* clone() const { return new Or(*this); }
};

std::shared_ptr<SALAM::Instruction>
createOrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Xor // -------------------------------------------------------------//

class Xor : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        Xor(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Xor() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Xor* clone() const { return new Xor(*this); }
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
    protected:


    public:
        Load(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Load() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Load* clone() const { return new Load(*this); }
};

std::shared_ptr<SALAM::Instruction>
createLoadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Store // -----------------------------------------------------------//

class Store : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        Store (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Store() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Store* clone() const { return new Store(*this); }
};

std::shared_ptr<SALAM::Instruction>
createStoreInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-GEP // -------------------------------------------------------------//

class GetElementPtr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
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
        void compute()      override;
        virtual GetElementPtr* clone() const { return new GetElementPtr(*this); }
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
    protected:

    public:
        Trunc(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Trunc() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Trunc* clone() const { return new Trunc(*this); }
};

std::shared_ptr<SALAM::Instruction>
createTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-ZExt // ------------------------------------------------------------//

class ZExt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        ZExt(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~ZExt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual ZExt* clone() const { return new ZExt(*this); }
};

std::shared_ptr<SALAM::Instruction>
createZExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SExt // ------------------------------------------------------------//

class SExt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        SExt(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SExt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual SExt* clone() const { return new SExt(*this); }
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
    protected:

    public:
        FPToUI(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPToUI() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FPToUI* clone() const { return new FPToUI(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFPToUIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FPToSI // ----------------------------------------------------------//

class FPToSI : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FPToSI(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPToSI() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FPToSI* clone() const { return new FPToSI(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFPToSIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-UIToFP // ----------------------------------------------------------//

class UIToFP : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        UIToFP(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~UIToFP() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual UIToFP* clone() const { return new UIToFP(*this); }
};

std::shared_ptr<SALAM::Instruction>
createUIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-SIToFP // ----------------------------------------------------------//

class SIToFP : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        SIToFP(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~SIToFP() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual SIToFP* clone() const { return new SIToFP(*this); }
};

std::shared_ptr<SALAM::Instruction>
createSIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FPTrunc // ---------------------------------------------------------//

class FPTrunc : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FPTrunc(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPTrunc() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FPTrunc* clone() const { return new FPTrunc(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFPTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FPExt // -----------------------------------------------------------//

class FPExt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FPExt(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FPExt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FPExt* clone() const { return new FPExt(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFPExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-PtrToInt // --------------------------------------------------------//

class PtrToInt : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        PtrToInt (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~PtrToInt() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual PtrToInt* clone() const { return new PtrToInt(*this); }
};

std::shared_ptr<SALAM::Instruction>
createPtrToIntInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-IntToPtr // --------------------------------------------------------//

class IntToPtr : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        IntToPtr(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~IntToPtr() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual IntToPtr* clone() const { return new IntToPtr(*this); }
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
    protected:

    public:
        ICmp(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~ICmp() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual ICmp* clone() const { return new ICmp(*this); }
};

std::shared_ptr<SALAM::Instruction>
createICmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-FCmp // ------------------------------------------------------------//

class FCmp : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        FCmp(uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~FCmp() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual FCmp* clone() const { return new FCmp(*this); }
};

std::shared_ptr<SALAM::Instruction>
createFCmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- Other / Edge Instructions ---------------------------------------//
//---------------------------------------------------------------------------//

// SALAM-Phi // -------------------------------------------------------------//

typedef std::pair<std::shared_ptr<SALAM::Value>, std::shared_ptr<SALAM::Value>> phiNode;
typedef std::vector< phiNode> phiArgs;

class Phi : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        // [Value, Previous BB]
        phiArgs arguments;

    protected:

    public:
        Phi (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Phi() = default;
        void initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        std::shared_ptr<SALAM::Value> evaluate(std::shared_ptr<SALAM::Value> previousBB);
        void compute()      override;
        virtual Phi* clone() const { return new Phi(*this); }
};

std::shared_ptr<SALAM::Instruction>
createPHIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Call // ------------------------------------------------------------//

class Call : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
    protected:

    public:
        Call (uint64_t id,
            uint64_t OpCode,
            uint64_t cycles);
        ~Call() = default;
        void initialize (llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList);
        void compute()      override;
        virtual Call* clone() const { return new Call(*this); }
};

std::shared_ptr<SALAM::Instruction>
createCallInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
// SALAM-Select // ----------------------------------------------------------//

class Select : public Instruction {
    private:
        std::vector< std::vector<uint64_t> > conditions;
        std::shared_ptr<SALAM::Value> condition;
        std::shared_ptr<SALAM::Value> trueValue;
        std::shared_ptr<SALAM::Value> falseValue;
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
        std::shared_ptr<SALAM::Value> evaluate();
        void compute()      override;
        virtual Select* clone() const { return new Select(*this); }
};

std::shared_ptr<SALAM::Instruction>
createSelectInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles);
//---------------------------------------------------------------------------//
//--------- End Instruction Classes -----------------------------------------//
//---------------------------------------------------------------------------//

// std::shared_ptr<SALAM::Value> attach(llvm::Value * irval, irvmap * irmap);

}

#endif // __HWACC_LLVM_INSTRUCTION_HH__