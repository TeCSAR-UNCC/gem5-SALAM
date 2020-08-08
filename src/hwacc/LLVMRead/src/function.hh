#ifndef __SALAM_FUNCTION_HH__
#define __SALAM_FUNCTION_HH__

#include "value.hh"
#include "basic_block.hh"
#include "llvm/IR/Function.h"

namespace SALAM {
	class Function : public Value {
	private:
		llvm::Function * irvalue;
		std::list<SALAM::BasicBlock*> * bbList;
		RegisterList * regList;
		bool debug;
		bool top;
	public:
		Function(uint64_t id);
		void initialize(llvm::Value * irval, irvmap *vmap, RegisterList * regs, bool dbg=false, bool isTop=false);
		bool isTop() { return top; }
		std::list<SALAM::BasicBlock*> * getBBList() { return bbList; }
	};
}

#endif //__SALAM_FUNCTION_HH__