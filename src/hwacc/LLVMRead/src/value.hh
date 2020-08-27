#ifndef __SALAM_VALUE_HH__
#define __SALAM_VALUE_HH__

#include "registers.hh"
#include "llvm/IR/Value.h"
#include <map>
#include <memory>

namespace SALAM {
	class Value;
	typedef std::pair<llvm::Value *, std::shared_ptr<Value>> irvmaptype;
	typedef std::map<llvm::Value *, std::shared_ptr<Value>> irvmap;

	class Value {
		// Sub Class - Register
		class Register {
            private:
                uint64_t data;
				uint64_t size;
				// TODO : Add support for larger registers

            public:
                Register() = default;
				Register(uint64_t bitSize) : size(bitSize) { }
				// set value function
        };

		private:
			//
		protected:
			uint64_t uid;
			llvm::Type * irtype;
			Register * reg; 


		public:
			Value(uint64_t id) { uid=id; }
			virtual void initialize(llvm::Value * irval, irvmap * irmap) { irtype = irval->getType(); }
			uint64_t getUID() { return uid; }
			// add register function

			// virtual Value* clone() const = 0;
	};
}


#endif //__SALAM_VALUE_HH__