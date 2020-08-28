#ifndef __HWACC_LLVM_INSTRUCTION_HH__
#define __HWACC_LLVM_INSTRUCTION_HH__

#include <cstdlib>
#include <iostream>
#include "value.hh"
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>


namespace SALAM { 

    //---------------------------------------------------------------------------//
    //--------- Instruction Base Class ------------------------------------------//
    //---------------------------------------------------------------------------//

    class Instruction : public Value { 
        private:
            llvm::Instruction * llvmIRClone;
            Value * returnRegister;
            std::vector<Value*> dependencies;
            std::vector<Value*> parents;
            std::vector<Value*> dynamicDependencies;
            std::vector<Value*> dynamicParents;
            std::vector<Value*> users;
            uint64_t llvmOpCode;

        protected:


        public:
            Instruction(uint64_t id) : Value(id) { std::cout << "ID: " << id << "\n"; }
            ~Instruction() = default;
            virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
            // Create initialize function for instruction - common item containers
            virtual void compute() { }
            // virtual Instruction* clone() const { return new Instruction(*this); }

    };

    //---------------------------------------------------------------------------//
    //--------- Terminator Instructions -----------------------------------------//
    //---------------------------------------------------------------------------//

    class BadInstruction : public Instruction {
        // Used to draw hard dependencies, ie: ret
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            BadInstruction(uint64_t id) : Instruction(id) { }
            ~BadInstruction() = default;
            void compute()      override { }
            virtual BadInstruction* clone() const { return new BadInstruction(*this); }
    };

    class Br : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            // Unconditional Branch Constructor
            // Br (uint64_t id) : Instruction(id) { }
            // Conditional Branch Constructor
            Br(uint64_t id) : Instruction(id) { }
            ~Br() = default;
            void compute()      override;
            virtual Br* clone() const { return new Br(*this); }
    };

    class Ret : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Ret(uint64_t id) : Instruction(id) { }
            ~Ret() = default;
            void compute()      override;
            virtual Ret* clone() const { return new Ret(*this); }
    };

    class Switch : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Switch(uint64_t id) : Instruction(id) { }
            ~Switch() = default;
            void compute()      override;
            virtual Switch* clone() const {  return new Switch(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- Binary Instructions ---------------------------------------------//
    //---------------------------------------------------------------------------//
    //// Integer Instructions

    class Add : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Add(uint64_t id) : Instruction(id) { }
            //Add(uint64_t id) : Instruction(id) { }
            ~Add() = default;
            void compute()      override;
            // virtual inintialize
            virtual Add* clone() const { return new Add(*this); }
    };

    class Sub : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Sub(uint64_t id) : Instruction(id) { }
            ~Sub() = default;
            void compute()      override;
            virtual Sub* clone() const { return new Sub(*this); }
    };

    class Mul : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Mul(uint64_t id) : Instruction(id) { }
            ~Mul() = default;
            void compute()      override;
            virtual Mul* clone() const { return new Mul(*this); }
    };

    class UDiv : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            UDiv(uint64_t id) : Instruction(id) { }
            ~UDiv() = default;
            void compute()      override;
            virtual UDiv* clone() const { return new UDiv(*this); }
    };

    class SDiv : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SDiv(uint64_t id) : Instruction(id) { }
            ~SDiv() = default;
            void compute()      override;
            virtual SDiv* clone() const { return new SDiv(*this); }
    };

    class URem : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            URem(uint64_t id) : Instruction(id) { }
            ~URem() = default;
            void compute()      override;
            virtual URem* clone() const { return new URem(*this); }
    };

    class SRem : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SRem(uint64_t id) : Instruction(id) { }
            ~SRem() = default;
            void compute()      override;
            virtual SRem* clone() const { return new SRem(*this); }
    };

    //// Floating Point Instructions

    class FAdd : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FAdd(uint64_t id) : Instruction(id) { }
            ~FAdd() = default;
            void compute()      override;
            virtual FAdd* clone() const { return new FAdd(*this); }
    };

    class FSub : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FSub(uint64_t id) : Instruction(id) { }
            ~FSub() = default;
            void compute()      override;
            virtual FSub* clone() const { return new FSub(*this); }
    };

    class FMul : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FMul(uint64_t id) : Instruction(id) { }
            ~FMul() = default;
            void compute()      override;
            virtual FMul* clone() const { return new FMul(*this); }
    };

    class FDiv : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FDiv(uint64_t id) : Instruction(id) { }
            ~FDiv() = default;
            void compute()      override;
            virtual FDiv* clone() const { return new FDiv(*this); }
    };

    class FRem : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FRem(uint64_t id) : Instruction(id) { }
            ~FRem() = default;
            void compute()      override;
            virtual FRem* clone() const { return new FRem(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- Logical Instructions --------------------------------------------//
    //---------------------------------------------------------------------------//

    class Shl : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Shl(uint64_t id) : Instruction(id) { }
            ~Shl() = default;
            void compute()      override;
            virtual Shl* clone() const { return new Shl(*this); }
    };

    class LShr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            LShr(uint64_t id) : Instruction(id) { }
            ~LShr() = default;
            void compute()      override;
            virtual LShr* clone() const { return new LShr(*this); }
    };

    class AShr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            AShr(uint64_t id) : Instruction(id) { }
            ~AShr() = default;
            void compute()      override;
            virtual AShr* clone() const { return new AShr(*this); }
    };

    class And : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            And(uint64_t id) : Instruction(id) { }
            ~And() = default;
            void compute()      override;
            virtual And* clone() const { return new And(*this); }
    };

    class Or : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Or(uint64_t id) : Instruction(id) { }
            ~Or() = default;
            void compute()      override;
            virtual Or* clone() const { return new Or(*this); }
    };

    class Xor : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Xor(uint64_t id) : Instruction(id) { }
            ~Xor() = default;
            void compute()      override;
            virtual Xor* clone() const { return new Xor(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- Cast Instructions -----------------------------------------------//
    //---------------------------------------------------------------------------//

    class Trunc : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Trunc(uint64_t id) : Instruction(id) { }
            ~Trunc() = default;
            void compute()      override;
            virtual Trunc* clone() const { return new Trunc(*this); }
    };

    class ZExt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            ZExt(uint64_t id) : Instruction(id) { }
            ~ZExt() = default;
            void compute()      override;
            virtual ZExt* clone() const { return new ZExt(*this); }
    };

    class SExt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SExt(uint64_t id) : Instruction(id) { }
            ~SExt() = default;
            void compute()      override;
            virtual SExt* clone() const { return new SExt(*this); }
    };

    class FPToUI : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPToUI(uint64_t id) : Instruction(id) { }
            ~FPToUI() = default;
            void compute()      override;
            virtual FPToUI* clone() const { return new FPToUI(*this); }
    };

    class FPToSI : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPToSI(uint64_t id) : Instruction(id) { }
            ~FPToSI() = default;
            void compute()      override;
            virtual FPToSI* clone() const { return new FPToSI(*this); }
    };

    class UIToFP : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            UIToFP(uint64_t id) : Instruction(id) { }
            ~UIToFP() = default;
            void compute()      override;
            virtual UIToFP* clone() const { return new UIToFP(*this); }
    };

    class SIToFP : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            SIToFP(uint64_t id) : Instruction(id) { }
            ~SIToFP() = default;
            void compute()      override;
            virtual SIToFP* clone() const { return new SIToFP(*this); }
    };

    class FPTrunc : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPTrunc(uint64_t id) : Instruction(id) { }
            ~FPTrunc() = default;
            void compute()      override;
            virtual FPTrunc* clone() const { return new FPTrunc(*this); }
    };

    class FPExt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FPExt(uint64_t id) : Instruction(id) { }
            ~FPExt() = default;
            void compute()      override;
            virtual FPExt* clone() const { return new FPExt(*this); }
    };

    class PtrToInt : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            PtrToInt (uint64_t id) : Instruction(id) { }
            ~PtrToInt() = default;
            void compute()      override;
            virtual PtrToInt* clone() const { return new PtrToInt(*this); }
    };

    class IntToPtr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            IntToPtr(uint64_t id) : Instruction(id) { }
            ~IntToPtr() = default;
            void compute()      override;
            virtual IntToPtr* clone() const { return new IntToPtr(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- Memory Instructions ---------------------------------------------//
    //---------------------------------------------------------------------------//

    class Load : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions; 
        protected:
        
        
        public:
            Load(uint64_t id) : Instruction(id) { }
            ~Load() = default;
            void compute()      override;
            virtual Load* clone() const { return new Load(*this); }
    };

    class Store : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Store (uint64_t id) : Instruction(id) { }
            ~Store() = default;
            void compute()      override;
            virtual Store* clone() const { return new Store(*this); }
    };

    class GetElementPtr : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions; 
        protected:

        public:
            GetElementPtr(uint64_t id) : Instruction(id) { }
            ~GetElementPtr() = default;
            void compute()      override;
            virtual GetElementPtr* clone() const { return new GetElementPtr(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- Other Instructions ----------------------------------------------//
    //---------------------------------------------------------------------------//

    class Phi : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            Phi (uint64_t id) : Instruction(id) { }
            ~Phi() = default;
            void compute()      override;
            virtual Phi* clone() const { return new Phi(*this); }
    };

    class Select : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:

        public:
            // ---- Constructor
            Select (uint64_t id) : Instruction(id) { }
            ~Select() = default;
            void compute()      override;
            virtual Select* clone() const { return new Select(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- Comparison Instructions -----------------------------------------//
    //---------------------------------------------------------------------------//

    class ICmp : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected: 

        public:
            ICmp(uint64_t id) : Instruction(id) { }
            ~ICmp() = default;           
            void compute()      override;
            virtual ICmp* clone() const { return new ICmp(*this); }
    };

    class FCmp : public Instruction {
        private:
            std::vector< std::vector<uint64_t> > conditions;
        protected:
        
        public:
            FCmp(uint64_t id) : Instruction(id) { }
            ~FCmp() = default;         
            void compute()      override;
            virtual FCmp* clone() const { return new FCmp(*this); }
    };
    //---------------------------------------------------------------------------//
    //--------- End Instruction Classes -----------------------------------------//
    //---------------------------------------------------------------------------//


    //---------------------------------------------------------------------------//
    //--------- Namespace Function Prototypes -----------------------------------//
    //---------------------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction> createRetInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createBrInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createSwitchInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createAddInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFAddInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createSubInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFSubInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createMulInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFMulInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createUDivInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createSDivInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFDivInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createURemInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createSRemInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFRemInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createShlInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createLShrInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createAShrInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createAndInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createOrInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createXorInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createLoadInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createStoreInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createGetElementPtrInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createTruncInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createZExtInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createSExtInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFPToUIInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFPToSIInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createUIToFPInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createSIToFPInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFPTruncInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFPExtInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createPtrToIntInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createIntToPtrInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createICmpInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createFCmpInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createPHIInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createCallInst(uint64_t id);
    std::shared_ptr<SALAM::Instruction> createSelectInst(uint64_t id);
    //---------------------------------------------------------------------------//

}

#endif // __HWACC_LLVM_INSTRUCTION_HH__