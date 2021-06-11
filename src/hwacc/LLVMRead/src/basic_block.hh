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
  class Instruction; // Do not remove

  class BasicBlock : public Value {
    private:
      std::vector<SALAM::BasicBlock*> predecessors;
      std::vector<std::shared_ptr<SALAM::Instruction>> instructions;
      bool dbg = false;
    protected:
      class BasicBlock_Debugger: public Debugger
        {
            public:
                BasicBlock_Debugger();
                ~BasicBlock_Debugger() = default;
                virtual void dumper(SALAM::BasicBlock * bb);
        }; 

       BasicBlock_Debugger* bb_dbg;  
    public:
      BasicBlock(uint64_t id);
      ~BasicBlock();
      virtual bool isBasicBlock() { return true; }
      void initialize(llvm::Value * irval, irvmap *vmap, SALAM::valueListTy *valueList);
      std::vector<std::shared_ptr<SALAM::Instruction> > * Instructions() { return &instructions; }
      void dump() { if (dbg) bb_dbg->dumper(this); }
  };
}

#endif //__SALAM_BASIC_BLOCK_HH__
