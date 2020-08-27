#ifndef __HWACC_LLVM_INSTRUCTION_HH__
#define __HWACC_LLVM_INSTRUCTION_HH__

#include <cstdlib>
#include <iostream>
#include "value.hh"
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>

//------------------------------------------//
//#include "hwacc/comm_interface.hh"
//#include "mem_request.hh"
//#include "llvm_types.hh"
//#include "debug_flags.hh"
//#include "registers.hh"
//#include "base_instruction.hh"


namespace SALAM {

    class Instruction : public Value { 
        private:
            llvm::Instruction * llvmIRClone;
        //    Operation operation;
            
        protected:


        public:
            Instruction(llvm::Instruction *newInstruction): Value(0),
                        llvmIRClone(newInstruction) {
                            std::cout << llvmIRClone->getOpcodeName() << " Loaded \n";
                         }
            Instruction(llvm::Instruction *newInstruction, uint64_t id): Value(id),
                        llvmIRClone(newInstruction) {
                            std::cout << llvmIRClone->getOpcodeName() << " Loaded \n";
                         }             
            Instruction(uint64_t id) : Value(id), llvmIRClone(NULL) {}
            void test() { std::cout << llvmIRClone->getOpcodeName() << " Test \n"; }
            virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
            virtual void compute() { }

    };

    std::shared_ptr<SALAM::Instruction> retInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> brInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> switchInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> addInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> faddInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> subInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fsubInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> mulInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fmulInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> udivInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> sdivInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fdivInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> uremInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> sremInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fremInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> shlInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> lshrInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> ashrInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> andInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> orInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> xorInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> loadInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> storeInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> getelementptrInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> truncInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> zextInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> sextInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fptouiInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fptosiInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> uitofpInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> sitofpInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fptruncInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fpextInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> ptrtointInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> inttoptrInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> icmpInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> fcmpInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> phiInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> callInst(llvm::Instruction * inst, uint64_t id);
    std::shared_ptr<SALAM::Instruction> selectInst(llvm::Instruction * inst, uint64_t id);


    //---------------------------------------------------------------------------//
    //--------- Begin Terminator Instruction Base -------------------------------//
    //---------------------------------------------------------------------------//
    class BadInstruction : public Instruction {
        // Used to draw hard dependencies, ie: ret
        public:
            BadInstruction();
            ~BadInstruction();
            void compute()      override { }
            virtual BadInstruction* clone() const { return new BadInstruction(*this); }
    };

    class Br : public Instruction {
        protected:

        public:
            // Unconditional Branch Constructor
            // Br ();
            // Conditional Branch Constructor
            Br();
            ~Br();
            void compute()      override;
            virtual Br* clone() const { return new Br(*this); }
    };


    class Ret : public Instruction {
        protected:

        public:
            Ret();
            ~Ret();
            void compute()      override;
            virtual Ret* clone() const { return new Ret(*this); }
    };

    class LLVMSwitch : public Instruction {
        protected:

        public:
            LLVMSwitch();
            ~LLVMSwitch();
            void compute()      override;
            virtual LLVMSwitch* clone() const {  return new LLVMSwitch(*this); }
    };
    //---------------------------------------------------------------------------//
    //--------- End Terminator Instruction Base ---------------------------------//
    //---------------------------------------------------------------------------//
    //--------- Begin Binary Instruction Base -----------------------------------//
    //---------------------------------------------------------------------------//
    // ---- Binary ---- Integer Instructions

    class Add : public Instruction {
        protected:

        public:
            Add();
            ~Add();
            void compute()      override;
            virtual Add* clone() const { return new Add(*this); }
    };

    class Sub : public Instruction {
        protected:

        public:
            Sub();
            ~Sub();
            void compute()      override;
            virtual Sub* clone() const { return new Sub(*this); }
    };

    class Mul : public Instruction {
        protected:

        public:
            Mul();
            ~Mul();
            void compute()      override;
            virtual Mul* clone() const { return new Mul(*this); }
    };

    class UDiv : public Instruction {
        protected:

        public:
            UDiv();
            ~UDiv();
            void compute()      override;
            virtual UDiv* clone() const { return new UDiv(*this); }
    };

    class SDiv : public Instruction {
        protected:

        public:
            SDiv();
            ~SDiv();
            void compute()      override;
            virtual SDiv* clone() const { return new SDiv(*this); }
    };

    class URem : public Instruction {
        protected:

        public:
            URem();
            ~URem();
            void compute()      override;
            virtual URem* clone() const { return new URem(*this); }
    };

    class SRem : public Instruction {
        protected:

        public:
            SRem();
            ~SRem();
            void compute()      override;
            virtual SRem* clone() const { return new SRem(*this); }
    };

    // ---- Binary ---- Floating Point Instructions

    class FAdd : public Instruction {
        protected:

        public:
            FAdd();
            ~FAdd();
            void compute()      override;
            virtual FAdd* clone() const { return new FAdd(*this); }
    };

    class FSub : public Instruction {
        protected:

        public:
            FSub();
            ~FSub();
            void compute()      override;
            virtual FSub* clone() const { return new FSub(*this); }
    };

    class FMul : public Instruction {
        protected:

        public:
            FMul();
            ~FMul();
            void compute()      override;
            virtual FMul* clone() const { return new FMul(*this); }
    };

    class FDiv : public Instruction {
        protected:

        public:
            FDiv();
            ~FDiv();
            void compute()      override;
            virtual FDiv* clone() const { return new FDiv(*this); }
    };

    class FRem : public Instruction {
        protected:

        public:
            FRem();
            ~FRem();
            void compute()      override;
            virtual FRem* clone() const { return new FRem(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- End Binary Instruction Base -------------------------------------//
    //---------------------------------------------------------------------------//
    //--------- Begin Bitwise Instruction Base ----------------------------------//
    //---------------------------------------------------------------------------//
    class Shl : public Instruction {
        protected:

        public:
            Shl();
            ~Shl();
            void compute()      override;
            virtual Shl* clone() const { return new Shl(*this); }
    };

    class LShr : public Instruction {
        protected:

        public:
            LShr();
            ~LShr();
            void compute()      override;
            virtual LShr* clone() const { return new LShr(*this); }
    };

    class AShr : public Instruction {
        protected:

        public:
            AShr();
            ~AShr();
            void compute()      override;
            virtual AShr* clone() const { return new AShr(*this); }
    };

    class And : public Instruction {
        protected:

        public:
            And();
            ~And();
            void compute()      override;
            virtual And* clone() const { return new And(*this); }
    };

    class Or : public Instruction {
        protected:

        public:
            Or();
            ~Or();
            void compute()      override;
            virtual Or* clone() const { return new Or(*this); }
    };

    class Xor : public Instruction {
        protected:

        public:
            Xor();
            ~Xor();
            void compute()      override;
            virtual Xor* clone() const { return new Xor(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- End Bitwise Instruction Base ------------------------------------//
    //---------------------------------------------------------------------------//
    //--------- Begin Conversion Instruction Base -------------------------------//
    //---------------------------------------------------------------------------//
    class Trunc : public Instruction {
        public:
            Trunc();
            ~Trunc();
            void compute()      override;
            virtual Trunc* clone() const { return new Trunc(*this); }
    };

    class ZExt : public Instruction {
        public:
            ZExt();
            ~ZExt();
            void compute()      override;
            virtual ZExt* clone() const { return new ZExt(*this); }
    };

    class SExt : public Instruction {
        public:
            SExt();
            ~SExt();
            void compute()      override;
            virtual SExt* clone() const { return new SExt(*this); }
    };

    class FPToUI : public Instruction {
        public:
            FPToUI();
            ~FPToUI();
            void compute()      override;
            virtual FPToUI* clone() const { return new FPToUI(*this); }
    };

    class FPToSI : public Instruction {
        public:
            FPToSI();
            ~FPToSI();
            void compute()      override;
            virtual FPToSI* clone() const { return new FPToSI(*this); }
    };

    class UIToFP : public Instruction {
        public:
            UIToFP();
            ~UIToFP();
            void compute()      override;
            virtual UIToFP* clone() const { return new UIToFP(*this); }
    };

    class SIToFP : public Instruction {
        public:
            SIToFP();
            ~SIToFP();
            void compute()      override;
            virtual SIToFP* clone() const { return new SIToFP(*this); }
    };

    class FPTrunc : public Instruction {
        public:
            FPTrunc();
            ~FPTrunc();
            void compute()      override;
            virtual FPTrunc* clone() const { return new FPTrunc(*this); }
    };

    class FPExt : public Instruction {
        public:
            FPExt();
            ~FPExt();
            void compute()      override;
            virtual FPExt* clone() const { return new FPExt(*this); }
    };

    class PtrToInt : public Instruction {
        public:
            PtrToInt ();
            ~PtrToInt();
            void compute()      override;
            virtual PtrToInt* clone() const { return new PtrToInt(*this); }
    };

    class IntToPtr : public Instruction {
        public:
            IntToPtr();
            ~IntToPtr();
            void compute()      override;
            virtual IntToPtr* clone() const { return new IntToPtr(*this); }
    };

    //---------------------------------------------------------------------------//
    //--------- End Conversion Instruction Base ---------------------------------//
    //---------------------------------------------------------------------------//
    //--------- Begin Memory Instruction Base -----------------------------------//
    //---------------------------------------------------------------------------//
    class Load : public Instruction {
        private:
        
        protected:
        
        
        public:
            Load();
            ~Load();
            void compute()      override;
            virtual Load* clone() const { return new Load(*this); }
    };

    class Store : public Instruction {
        private:
        
        protected:
        
        public:
            Store ();
            ~Store();
            void compute()      override;
            virtual Store* clone() const { return new Store(*this); }
    };

    class GetElementPtr : public Instruction {
        private:
        
        protected:

        public:
            GetElementPtr();
            ~GetElementPtr();
            void compute()      override;
            virtual GetElementPtr* clone() const { return new GetElementPtr(*this); }
    };
    //---------------------------------------------------------------------------//
    //--------- End Memory Instruction Base -------------------------------------//
    //---------------------------------------------------------------------------//
    //--------- Begin Other Instruction Base ------------------------------------//
    //---------------------------------------------------------------------------//
    class Phi : public Instruction {
        private:

        protected:
        
        public:
            Phi ();
            ~Phi();
            void compute()      override;
            virtual Phi* clone() const { return new Phi(*this); }
    };

    class Select : public Instruction {
        private:

        protected:

        public:
            // ---- Constructor
            Select ();
            ~Select();
            void compute()      override;
            virtual Select* clone() const { return new Select(*this); }
    };
    // ---- Other Sub Type ---- Compare
    class ICmp : public Instruction {
        private:

        protected: 

        public:
            ICmp();
            ~ICmp();           
            void compute()      override;
            virtual ICmp* clone() const { return new ICmp(*this); }
    };

    class FCmp : public Instruction {
        private:

        protected:
        
        public:
            FCmp();
            ~FCmp();         
            void compute()      override;
            virtual FCmp* clone() const { return new FCmp(*this); }
    };
    //---------------------------------------------------------------------------//
    //--------- End Other Instruction Base --------------------------------------//
    //---------------------------------------------------------------------------//
}

#endif // __HWACC_LLVM_INSTRUCTION_HH__