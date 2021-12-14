#ifndef __SALAM_FUNCTION_HH__
#define __SALAM_FUNCTION_HH__

#include "value.hh"
#include "basic_block.hh"
#include "llvm/IR/Function.h"
#include "llvm/IR/DataLayout.h"

namespace SALAM {
	class Function : public Value {
	private:
		std::vector<std::shared_ptr<SALAM::BasicBlock>> bbList;
		std::vector<std::shared_ptr<SALAM::Value>> arguments;
		bool top;
		unsigned activeInstances = 0;
		unsigned instanceLimit = 1;
	public:
		Function(uint64_t id, gem5::SimObject * owner, bool dbg);
		void initialize(llvm::Value * irval, irvmap *vmap, SALAM::valueListTy *valueList, std::string topName);
		bool isTop() { return top; }
		void setTop(bool isTop) { top = isTop; }
		virtual bool isFunction() { return true; }
		std::vector<std::shared_ptr<SALAM::BasicBlock>> * getBBList() { return &bbList; }
		std::vector<std::shared_ptr<SALAM::Value>> * getArguments() { return & arguments; }
		std::shared_ptr<SALAM::BasicBlock> entry() { return bbList.front(); }
		void addInstance() { activeInstances++; }
		void removeInstance() { if (activeInstances>0) activeInstances--; }
		bool canLaunch() { return activeInstances < instanceLimit; }
	};
}

#endif //__SALAM_FUNCTION_HH__