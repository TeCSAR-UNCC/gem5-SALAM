#ifndef __HWACC_LLVM_INSTRUCTION_HH__
#define __HWACC_LLVM_INSTRUCTION_HH__

#include <cstdlib>
#include <iostream>
#include "value.hh"
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>
#include "debug_flags.hh"

// initialize value
// initialize instruction data-struction first
// initialize instruction values
// initialize instruction sub-type
// initialize register

// value init
// instruction init
// register init
// instruction sub type

namespace SALAM { 

    //---------------------------------------------------------------------------//
    //--------- Instruction Base Class ------------------------------------------//
    //---------------------------------------------------------------------------//

    class Instruction : public Value { 
        private:
            Value * returnRegister;
            valueListTy staticDependencies;
            valueListTy dynamicDependencies;
            valueListTy dynamicUsers;
            uint64_t llvmOpCode;

        protected:


        public:
            Instruction(uint64_t id) : Value(id) { 
                            CLASSOUT("--SALAM::Instruction::Instruction(uint64_t)", id); }
            Instruction(uint64_t id, uint64_t OpCode) : Value(id), llvmOpCode(OpCode) { 
                            CLASSOUT("--SALAM::Instruction::Instruction(uint64_t) [" << \
                            llvm::Instruction::getOpcodeName(OpCode) << "]", id); }
            ~Instruction() = default;
            virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * valueList);
            // Create initialize function for instruction - common item containers
            virtual void compute() { }
            // virtual Instruction* clone() const { return new Instruction(*this); }

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
            BadInstruction(uint64_t id) : Instruction(id) {
                            CLASSOUT("----SALAM::BadInstruction::BadInstruction(uint64_t)", id); }
            ~BadInstruction() = default;
            void compute()      override { }
            virtual BadInstruction* clone() const { return new BadInstruction(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createBadInst(uint64_t id, uint64_t opCode);

    // SALAM-Ret // -------------------------------------------------------------//
    class Ret;
    void initializeRetInst(SALAM::Ret &salamInstruction);

    class Ret : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Ret(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) { 
                            CLASSOUT("----SALAM::Ret::Ret(uint64_t)", id); }
            ~Ret() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Ret::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeRetInst(*this); }
            Ret &setA() { std::cout << "a\n"; return *this; }
            Ret &setB() { std::cout << "b\n"; return *this; }
            void compute()      override;
            virtual Ret* clone() const { return new Ret(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createRetInst(uint64_t id, uint64_t opCode);

    // SALAM-Br // --------------------------------------------------------------//
    class Br;
    void initializeBrInst(SALAM::Br &salamInstruction);
    
    class Br : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            // Unconditional Branch Constructor
            // Br (uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) { }
            // Conditional Branch Constructor
            Br(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Br::Br(uint64_t)", id); }
            ~Br() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Br::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeBrInst(*this); }
            void compute()      override;
            virtual Br* clone() const { return new Br(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createBrInst(uint64_t id, uint64_t OpCode);

    // SALAM-Switch // ----------------------------------------------------------//
    class Switch;
    void initializeSwitchInst(SALAM::Switch &salamInstruction);
    
    class Switch : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Switch(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Switch::Switch(uint64_t)", id); }
            ~Switch() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Switch::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeSwitchInst(*this); }
            void compute()      override;
            virtual Switch* clone() const {  return new Switch(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createSwitchInst(uint64_t id, uint64_t OpCode);

    //---------------------------------------------------------------------------//
    //--------- Binary Operator Instructions ------------------------------------//
    //---------------------------------------------------------------------------//

    // SALAM-Add // -------------------------------------------------------------//
    class Add;
    void initializeAddInst(SALAM::Add &salamInstruction);

    class Add : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Add(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Add::Add(uint64_t)", id); } 
            ~Add() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Add::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeAddInst(*this); }
            Add &setA() { std::cout << "a\n"; return *this; }
            Add &setB() { std::cout << "b\n"; return *this; }
            void compute()      override;
            // virtual inintialize
            virtual Add* clone() const { return new Add(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createAddInst(uint64_t id, uint64_t OpCode);

    // SALAM-FAdd // ------------------------------------------------------------//    
    class FAdd;
    void initializeFAddInst(SALAM::FAdd &salamInstruction);
    
    class FAdd : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FAdd(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FAdd::FAdd(uint64_t)", id); } 
            ~FAdd() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FAdd::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFAddInst(*this); }
            void compute()      override;
            virtual FAdd* clone() const { return new FAdd(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFAddInst(uint64_t id, uint64_t OpCode);

    // SALAM-Sub // -------------------------------------------------------------//
    class Sub;
    void initializeSubInst(SALAM::Sub &salamInstruction);
    
    class Sub : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Sub(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Sub::Sub(uint64_t)", id); } 
            ~Sub() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Sub::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeSubInst(*this); }
            void compute()      override;
            virtual Sub* clone() const { return new Sub(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createSubInst(uint64_t id, uint64_t OpCode);

    // SALAM-FSub // -------------------------------------------------------------//    
    class FSub;
    void initializeFSubInst(SALAM::FSub &salamInstruction);
    
    class FSub : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FSub(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FSub::FSub(uint64_t)", id); } 
            ~FSub() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FSub::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFSubInst(*this); }
            void compute()      override;
            virtual FSub* clone() const { return new FSub(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFSubInst(uint64_t id, uint64_t OpCode);

    // SALAM-Mul // -------------------------------------------------------------//
    class Mul;
    void initializeMulInst(SALAM::Mul &salamInstruction);
    
    class Mul : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Mul(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Mul::Mul(uint64_t)", id); } 
            ~Mul() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Mul::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeMulInst(*this); }
            void compute()      override;
            virtual Mul* clone() const { return new Mul(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createMulInst(uint64_t id, uint64_t OpCode);

    // SALAM-FMul // ------------------------------------------------------------//    
    class FMul;
    void initializeFMulInst(SALAM::FMul &salamInstruction);
    
    class FMul : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FMul(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FMul::FMul(uint64_t)", id); } 
            ~FMul() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FMul::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFMulInst(*this); }
            void compute()      override;
            virtual FMul* clone() const { return new FMul(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFMulInst(uint64_t id, uint64_t OpCode);

    // SALAM-UDiv // ------------------------------------------------------------//
    class UDiv;
    void initializeUDivInst(SALAM::UDiv &salamInstruction);
    
    class UDiv : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            UDiv(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::UDiv::UDiv(uint64_t)", id); } 
            ~UDiv() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::UDiv::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeUDivInst(*this); }
            void compute()      override;
            virtual UDiv* clone() const { return new UDiv(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createUDivInst(uint64_t id, uint64_t OpCode);

    // SALAM-SDiv // ------------------------------------------------------------//
    class SDiv;
    void initializeSDivInst(SALAM::SDiv &salamInstruction);
    
    class SDiv : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SDiv(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::SDiv::SDiv(uint64_t)", id); } 
            ~SDiv() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::SDiv::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeSDivInst(*this); }
            void compute()      override;
            virtual SDiv* clone() const { return new SDiv(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createSDivInst(uint64_t id, uint64_t OpCode);

    // SALAM-FDiv // ------------------------------------------------------------//
    class FDiv;
    void initializeFDivInst(SALAM::FDiv &salamInstruction);
    
    class FDiv : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FDiv(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FDiv::FDiv(uint64_t)", id); } 
            ~FDiv() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FDiv::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFDivInst(*this); }
            void compute()      override;
            virtual FDiv* clone() const { return new FDiv(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFDivInst(uint64_t id, uint64_t OpCode);

    // SALAM-URem // ------------------------------------------------------------//
    class URem;
    void initializeURemInst(SALAM::URem &salamInstruction);
    
    class URem : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            URem(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::URem::URem(uint64_t)", id); } 
            ~URem() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::URem::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeURemInst(*this); }
            void compute()      override;
            virtual URem* clone() const { return new URem(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createURemInst(uint64_t id, uint64_t OpCode);

    // SALAM-SRem // ------------------------------------------------------------//
    class SRem;
    void initializeSRemInst(SALAM::SRem &salamInstruction);
    
    class SRem : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SRem(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::SRem::SRem(uint64_t)", id); } 
            ~SRem() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::SRem::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeSRemInst(*this); }
            void compute()      override;
            virtual SRem* clone() const { return new SRem(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createSRemInst(uint64_t id, uint64_t OpCode);

    // SALAM-FRem // ------------------------------------------------------------//
    class FRem;
    void initializeFRemInst(SALAM::FRem &salamInstruction);
    
    class FRem : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FRem(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FRem::FRem(uint64_t)", id); } 
            ~FRem() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FRem::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFRemInst(*this); }
            void compute()      override;
            virtual FRem* clone() const { return new FRem(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFRemInst(uint64_t id, uint64_t OpCode);

    //---------------------------------------------------------------------------//
    //--------- Bitwise Binary Operator Instructions ----------------------------//
    //---------------------------------------------------------------------------//

    // SALAM-Shl // -------------------------------------------------------------//
    class Shl;
    void initializeShlInst(SALAM::Shl &salamInstruction);
    
    class Shl : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Shl(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Shl::Shl(uint64_t)", id); } 
            ~Shl() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Shl::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeShlInst(*this); }
            void compute()      override;
            virtual Shl* clone() const { return new Shl(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createShlInst(uint64_t id, uint64_t OpCode);

    // SALAM-LShr // ------------------------------------------------------------//
    class LShr;
    void initializeLShrInst(SALAM::LShr &salamInstruction);
    
    class LShr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            LShr(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::LShr::LShr(uint64_t)", id); } 
            ~LShr() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::LShr::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeLShrInst(*this); }
            void compute()      override;
            virtual LShr* clone() const { return new LShr(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createLShrInst(uint64_t id, uint64_t OpCode);

    // SALAM-AShr // ------------------------------------------------------------//
    class AShr;
    void initializeAShrInst(SALAM::AShr &salamInstruction);
    
    class AShr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            AShr(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::AShr::AShr(uint64_t)", id); } 
            ~AShr() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::AShr::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeAShrInst(*this); }
            void compute()      override;
            virtual AShr* clone() const { return new AShr(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createAShrInst(uint64_t id, uint64_t OpCode);

    // SALAM-And // -------------------------------------------------------------//
    class And;
    void initializeAndInst(SALAM::And &salamInstruction);
    
    class And : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            And(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::And::And(uint64_t)", id); } 
            ~And() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::And::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeAndInst(*this); }
            void compute()      override;
            virtual And* clone() const { return new And(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createAndInst(uint64_t id, uint64_t OpCode);

    // SALAM-Or // --------------------------------------------------------------//
    class Or;
    void initializeOrInst(SALAM::Or &salamInstruction);
    
    class Or : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Or(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Or::Or(uint64_t)", id); } 
            ~Or() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Or::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeOrInst(*this); }
            void compute()      override;
            virtual Or* clone() const { return new Or(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createOrInst(uint64_t id, uint64_t OpCode);

    // SALAM-Xor // -------------------------------------------------------------//
    class Xor;
    void initializeXorInst(SALAM::Xor &salamInstruction);
    
    class Xor : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Xor(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Xor::Xor(uint64_t)", id); } 
            ~Xor() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Xor::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeXorInst(*this); }
            void compute()      override;
            virtual Xor* clone() const { return new Xor(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createXorInst(uint64_t id, uint64_t OpCode);

    //---------------------------------------------------------------------------//
    //--------- Memory Instructions ---------------------------------------------//
    //---------------------------------------------------------------------------//

    // SALAM-Load // ------------------------------------------------------------//
    class Load;
    void initializeLoadInst(SALAM::Load &salamInstruction);
    
    class Load : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions; 
        protected:
        
        
        public:
            Load(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Load::Load(uint64_t)", id); } 
            ~Load() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Load::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeLoadInst(*this); }
            void compute()      override;
            virtual Load* clone() const { return new Load(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createLoadInst(uint64_t id, uint64_t OpCode);

    // SALAM-Store // -----------------------------------------------------------//
    class Store;
    void initializeStoreInst(SALAM::Store &salamInstruction);
    
    class Store : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Store (uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Store::Store(uint64_t)", id); } 
            ~Store() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Store::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeStoreInst(*this); }
            void compute()      override;
            virtual Store* clone() const { return new Store(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createStoreInst(uint64_t id, uint64_t OpCode);

    // SALAM-GEP // -------------------------------------------------------------//
    class GetElementPtr;
    void initializeGetElementPtrInst(SALAM::GetElementPtr &salamInstruction);
    
    class GetElementPtr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions; 
        protected:

        public:
            GetElementPtr(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::GetElementPtr::GetElementPtr(uint64_t)", id); }
            ~GetElementPtr() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::GEP::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeGetElementPtrInst(*this); }
            void compute()      override;
            virtual GetElementPtr* clone() const { return new GetElementPtr(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createGetElementPtrInst(uint64_t id, uint64_t OpCode);

    //---------------------------------------------------------------------------//
    //--------- Other / Cast Instructions ---------------------------------------//
    //---------------------------------------------------------------------------//

    // SALAM-Trunc // -----------------------------------------------------------//
    class Trunc;
    void initializeTruncInst(SALAM::Trunc &salamInstruction);
    
    class Trunc : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Trunc(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Trunc::Trunc(uint64_t)", id); } 
            ~Trunc() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Trunc::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeTruncInst(*this); }
            void compute()      override;
            virtual Trunc* clone() const { return new Trunc(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createTruncInst(uint64_t id, uint64_t OpCode);

    // SALAM-ZExt // ------------------------------------------------------------//
    class ZExt;
    void initializeZExtInst(SALAM::ZExt &salamInstruction);

    class ZExt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            ZExt(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::ZExt::ZExt(uint64_t)", id); } 
            ~ZExt() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::ZExt::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeZExtInst(*this); }
            void compute()      override;
            virtual ZExt* clone() const { return new ZExt(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createZExtInst(uint64_t id, uint64_t OpCode);

    // SALAM-SExt // ------------------------------------------------------------//
    class SExt;
    void initializeSExtInst(SALAM::SExt &salamInstruction);
    
    class SExt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SExt(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::SExt::SExt(uint64_t)", id); } 
            ~SExt() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::SExt::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeSExtInst(*this); }
            void compute()      override;
            virtual SExt* clone() const { return new SExt(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createSExtInst(uint64_t id, uint64_t OpCode);

    // SALAM-FPToUI // ----------------------------------------------------------//
    class FPToUI;
    void initializeFPToUIInst(SALAM::FPToUI &salamInstruction);

    class FPToUI : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPToUI(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FPToUI::FPToUI(uint64_t)", id); } 
            ~FPToUI() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FPToUI::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFPToUIInst(*this); }
            void compute()      override;
            virtual FPToUI* clone() const { return new FPToUI(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFPToUIInst(uint64_t id, uint64_t OpCode);

    // SALAM-FPToSI // ----------------------------------------------------------//
    class FPToSI;
    void initializeFPToSIInst(SALAM::FPToSI &salamInstruction);    
    
    class FPToSI : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPToSI(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FPToSI::FPToSI(uint64_t)", id); } 
            ~FPToSI() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FPToSI::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFPToSIInst(*this); }
            void compute()      override;
            virtual FPToSI* clone() const { return new FPToSI(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFPToSIInst(uint64_t id, uint64_t OpCode);

    // SALAM-UIToFP // ----------------------------------------------------------//
    class UIToFP;
    void initializeUIToFPInst(SALAM::UIToFP &salamInstruction);

    class UIToFP : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            UIToFP(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::UIToFP::UIToFP(uint64_t)", id); } 
            ~UIToFP() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::UIToFP::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeUIToFPInst(*this); }
            void compute()      override;
            virtual UIToFP* clone() const { return new UIToFP(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createUIToFPInst(uint64_t id, uint64_t OpCode);

    // SALAM-SIToFP // ----------------------------------------------------------//
    class SIToFP;
    void initializeSIToFPInst(SALAM::SIToFP &salamInstruction);    
    class SIToFP : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SIToFP(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::SIToFP::SIToFP(uint64_t)", id); } 
            ~SIToFP() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::SIToFP::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeSIToFPInst(*this); }
            void compute()      override;
            virtual SIToFP* clone() const { return new SIToFP(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createSIToFPInst(uint64_t id, uint64_t OpCode);

    // SALAM-FPTrunc // ---------------------------------------------------------//
    class FPTrunc;
    void initializeFPTruncInst(SALAM::FPTrunc &salamInstruction);

    class FPTrunc : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPTrunc(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FPTrunc::FPTrunc(uint64_t)", id); } 
            ~FPTrunc() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FPTrunc::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFPTruncInst(*this); }            
            void compute()      override;
            virtual FPTrunc* clone() const { return new FPTrunc(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFPTruncInst(uint64_t id, uint64_t OpCode);

    // SALAM-FPExt // -----------------------------------------------------------//
    class FPExt;
    void initializeFPExtInst(SALAM::FPExt &salamInstruction);    
    
    class FPExt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPExt(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FPExt::FPExt(uint64_t)", id); } 
            ~FPExt() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FPExt::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFPExtInst(*this); }
            void compute()      override;
            virtual FPExt* clone() const { return new FPExt(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFPExtInst(uint64_t id, uint64_t OpCode);

    // SALAM-PtrToInt // --------------------------------------------------------//
    class PtrToInt;
    void initializePtrToIntInst(SALAM::PtrToInt &salamInstruction);

    class PtrToInt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            PtrToInt (uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::PtrToInt::PtrToInt(uint64_t)", id); } 
            ~PtrToInt() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::PtrToInt::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializePtrToIntInst(*this); }
            void compute()      override;
            virtual PtrToInt* clone() const { return new PtrToInt(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createPtrToIntInst(uint64_t id, uint64_t OpCode);

    // SALAM-IntToPtr // --------------------------------------------------------//
    class IntToPtr;
    void initializeIntToPtrInst(SALAM::IntToPtr &salamInstruction);

    class IntToPtr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            IntToPtr(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::IntToPtr::IntToPtr(uint64_t)", id); } 
            ~IntToPtr() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::IntToPtr::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeIntToPtrInst(*this); }
            void compute()      override;
            virtual IntToPtr* clone() const { return new IntToPtr(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createIntToPtrInst(uint64_t id, uint64_t OpCode);

    //---------------------------------------------------------------------------//
    //--------- Other / Comparison Instructions ---------------------------------//
    //---------------------------------------------------------------------------//

    // SALAM-ICmp // ------------------------------------------------------------//
    class ICmp;
    void initializeICmpInst(SALAM::ICmp &salamInstruction);
    
    class ICmp : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected: 

        public:
            ICmp(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::ICmp::ICmp(uint64_t)", id); } 
            ~ICmp() = default;           
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::ICmp::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeICmpInst(*this); }
            void compute()      override;
            virtual ICmp* clone() const { return new ICmp(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createICmpInst(uint64_t id, uint64_t OpCode);

    // SALAM-FCmp // ------------------------------------------------------------//
    class FCmp;
    void initializeFCmpInst(SALAM::FCmp &salamInstruction);

    class FCmp : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FCmp(uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::FCmp::FCmp(uint64_t)", id); } 
            ~FCmp() = default;         
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::FCmp::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeFCmpInst(*this); }
            void compute()      override;
            virtual FCmp* clone() const { return new FCmp(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createFCmpInst(uint64_t id, uint64_t OpCode);

    //---------------------------------------------------------------------------//
    //--------- Other / Edge Instructions ---------------------------------------//
    //---------------------------------------------------------------------------//

    // SALAM-Phi // -------------------------------------------------------------//
    class Phi;
    void initializePhiInst(SALAM::Phi &salamInstruction);

    class Phi : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Phi (uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Phi::Phi(uint64_t)", id); } 
            ~Phi() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Phi::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializePhiInst(*this); }
            void compute()      override;
            virtual Phi* clone() const { return new Phi(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createPHIInst(uint64_t id, uint64_t OpCode);

    // SALAM-Call // ------------------------------------------------------------//
    class Call;
    void initializeCallInst(SALAM::Call &salamInstruction);
    class Call : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Call (uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Call::Call(uint64_t)", id); } 
            ~Call() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Call::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeCallInst(*this); }
            void compute()      override;
            virtual Call* clone() const { return new Call(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createCallInst(uint64_t id, uint64_t OpCode);

    // SALAM-Select // ----------------------------------------------------------//
    class Select;
    void initializeSelectInst(SALAM::Select &salamInstruction);
    class Select : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:

        public:
            // ---- Constructor
            Select (uint64_t id, uint64_t OpCode) : Instruction(id, OpCode) {
                            CLASSOUT("----SALAM::Select::Select(uint64_t)", id); } 
            ~Select() = default;
            void initialize (llvm::Value * irval, 
                            irvmap * irmap, 
                            SALAM::valueListTy * valueList) { 
                                CLASSOUT("SALAM::Select::init(llvm::Value*, irvmap*, SALAM::valueListTy*)", uid);
                                Instruction::initialize(irval, irmap, valueList);
                                initializeSelectInst(*this); }
            void compute()      override;
            virtual Select* clone() const { return new Select(*this); }
    };

    std::shared_ptr<SALAM::Instruction> createSelectInst(uint64_t id, uint64_t OpCode);

    //---------------------------------------------------------------------------//
    //--------- End Instruction Classes -----------------------------------------//
    //---------------------------------------------------------------------------//
}

#endif // __HWACC_LLVM_INSTRUCTION_HH__