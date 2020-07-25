#ifndef __HWACC_LLVM_IR_PARSER_HH__
#define __HWACC_LLVM_IR_PARSER_HH__

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Support/raw_ostream.h>
#include "instruction.hh"
#include <string>
#include <iostream>
#include <vector>
#include <memory>

namespace SALAM {
    int ir_parser(std::string file);  
    void register_instruction(llvm::Instruction * inst, std::vector<std::shared_ptr<SALAM::Instruction>> &inst_List);

}

#endif //__HWACC_LLVM_IR_PARSER_HH__