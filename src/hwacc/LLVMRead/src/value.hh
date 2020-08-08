#ifndef __SALAM_VALUE_HH__
#define __SALAM_VALUE_HH__

#include "llvm/IR/Value.h"
#include <map>

namespace SALAM {
	class Value;
	typedef std::map<llvm::Value *, Value *> irvmap;

	class Value {
	private:
		//
	protected:
		uint64_t uid;
		llvm::Type * irtype;
	public:
		Value(uint64_t id) { uid=id; }
		virtual void initialize(llvm::Value * irval, irvmap * irmap) { irtype = irval->getType(); }
		uint64_t getUID() { return uid; }
	};
}


#endif //__SALAM_VALUE_HH__