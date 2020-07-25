#ifndef __HWACC_LLVM_INSTRUCTION_HH__
#define __HWACC_LLVM_INSTRUCTION_HH__

#include <cstdlib>
#include <iostream>
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>



namespace SALAM {
    class Instruction {
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
            Instruction(llvm::Instruction *newInstruction):
                        llvmIRClone(newInstruction) {
                            std::cout << llvmIRClone->getOpcodeName() << " Loaded \n";
                         }
            void test() { std::cout << llvmIRClone->getOpcodeName() << " Test \n"; }


    };
}

#endif // __HWACC_LLVM_INSTRUCTION_HH__