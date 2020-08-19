#ifndef __SALAM_BASIC_BLOCK_HH__
#define __SALAM_BASIC_BLOCK_HH__
//------------------------------------------//
#include "debug_flags.hh"
#include "value.hh"
#include "instruction.hh"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
//------------------------------------------//
#include <memory>
#include <iomanip>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
//------------------------------------------//

namespace SALAM {
  class BasicBlock : public Value {
    private:
      std::vector<SALAM::BasicBlock*> predecessors;
      std::vector<std::shared_ptr<SALAM::Instruction>> instructions;
      bool _debug = false;
      RegisterList * regList;

    protected:
    public:
      BasicBlock(uint64_t id);
      void initialize(llvm::Value * irval, irvmap *vmap, RegisterList * regs);
      std::vector<std::shared_ptr<SALAM::Instruction> > * Instructions() { return &instructions; }
  };
}

#endif //__SALAM_BASIC_BLOCK_HH__
