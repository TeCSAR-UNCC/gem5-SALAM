#ifndef __SALAM_FUNCTION_HH__
#define __SALAM_FUNCTION_HH__

#include "value.hh"
#include "basic_block.hh"
#include "llvm/IR/Function.h"

namespace SALAM {
	class Function : public Value {
	private:
		std::vector<std::shared_ptr<SALAM::BasicBlock>> bbList;
		std::vector<std::shared_ptr<SALAM::Value>> arguments;
		RegisterList * regList;
		bool top;
	public:
		Function(uint64_t id);
		void initialize(llvm::Value * irval, irvmap *vmap, RegisterList * regs, bool isTop=false);
		bool isTop() { return top; }
		std::vector<std::shared_ptr<SALAM::BasicBlock>> * getBBList() { return &bbList; }
		std::shared_ptr<SALAM::BasicBlock> entry() { return bbList.front(); }
	};
}

#endif //__SALAM_FUNCTION_HH__