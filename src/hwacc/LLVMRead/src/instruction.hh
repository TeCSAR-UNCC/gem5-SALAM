#ifndef __HWACC_LLVM_INSTRUCTION_HH__
#define __HWACC_LLVM_INSTRUCTION_HH__

#include <cstdlib>
#include <iostream>
#include "value.hh"
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>



namespace SALAM {
    class Instruction : public Value {
        class Register {
            Register();

        };
        public:
        //    enum Operation : uint64_t {
        //        Add,
        //        Sub,
        //        Mul
        //    }; // Not Full List
        //    constexpr Instruction(Operation currInst) : value(currInst) { }
        //    operator Operation() const { return operation; }
        //    explicit operator bool() = delete;
        //    constexpr bool isAdd() const { return value == Add; }
        private:
            llvm::Instruction * llvmIRClone;
        //    Operation operation;
            


        protected:






        public:
            Instruction(llvm::Instruction *newInstruction): Value(0),
                        llvmIRClone(newInstruction) {
                            std::cout << llvmIRClone->getOpcodeName() << " Loaded \n";
                         }
            Instruction(uint64_t id) : Value(id), llvmIRClone(NULL) {}
            void test() { std::cout << llvmIRClone->getOpcodeName() << " Test \n"; }
            virtual void initialize(llvm::Value * irval, irvmap * irmap) override;

    };
}

#endif // __HWACC_LLVM_INSTRUCTION_HH__