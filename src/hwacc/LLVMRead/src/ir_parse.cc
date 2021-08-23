#include "ir_parse.hh"
#include "instruction.hh"

namespace SALAM {
    int ir_parser(std::string file) {
        llvm::StringRef filename = file;
        llvm::LLVMContext context;
        llvm::SMDiagnostic error;

        // Load LLVM IR file
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileOrErr = llvm::MemoryBuffer::getFileOrSTDIN(filename);
        if (std::error_code ec = fileOrErr.getError()) {
            std::cerr << " Error opening input file: " + ec.message() << std::endl;
            return 2;
        }

        // Load LLVM Module
        llvm::ErrorOr<std::unique_ptr<llvm::Module>> moduleOrErr = llvm::parseIRFile(filename, error, context);
        if (std::error_code ec = moduleOrErr.getError()) {
            std::cerr << "Error reading Module: " + ec.message() << std::endl;
            return 3;
        }

        std::unique_ptr<llvm::Module> m(llvm::parseIRFile(filename, error, context));
        if(!m) return 4;

        std::cout << "Successfully Loaded Module:" << std::endl;
        std::cout << " Name: " << m->getName().str() << std::endl;
        std::cout << " Target Triple: " << m->getTargetTriple() << std::endl;

        std::vector<std::shared_ptr<SALAM::Instruction>> inst_List;

        for (auto func_iter = m->getFunctionList().begin(); func_iter != m->getFunctionList().end(); func_iter++) {
            llvm::Function &f = *func_iter;
            std::cout << " Function: " << f.getName().str() << std::endl;
            for (auto bb_iter = f.getBasicBlockList().begin(); bb_iter != f.getBasicBlockList().end(); bb_iter++) {
                llvm::BasicBlock &bb = *bb_iter;
                std::cout << "  BasicBlock: " << bb.getName().str() << std::endl;
                for (auto inst_iter = bb.begin(); inst_iter != bb.end(); inst_iter++) {
                    llvm::Instruction &llvm_inst = *inst_iter;
                    SALAM::register_instruction(llvm_inst.clone(), inst_List);
                }
            }
        }
        
        // Test Function Only
        for (auto inst_list_it = inst_List.begin() ; inst_list_it != inst_List.end(); inst_list_it++) {
            (*inst_list_it)->test();
        }
        
        return 0;
    }

    void register_instruction(llvm::Instruction * inst, std::vector<std::shared_ptr<SALAM::Instruction>> &inst_List) {       
        std::shared_ptr<SALAM::Instruction> newInst(new SALAM::Instruction(inst));
        inst_List.push_back(std::move(newInst));
    }


}