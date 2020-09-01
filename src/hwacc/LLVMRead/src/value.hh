#ifndef __SALAM_VALUE_HH__
#define __SALAM_VALUE_HH__

#include "registers.hh"
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"
#include <map>
#include <memory>
#include <vector>

namespace SALAM {
	class Value;
	typedef std::pair<llvm::Value *, std::shared_ptr<Value>> irvmaptype;
	typedef std::map<llvm::Value *, std::shared_ptr<Value>> irvmap;
	typedef std::vector<std::shared_ptr<Value>> valueListTy;

	class Value {
		// Sub Class - Register
		class Register {
            private:
                uint64_t data;
				unsigned size;
				// TODO : Add support for larger registers

            public:
                Register() = default;
				Register(unsigned bitSize) : size(bitSize) { }
				// set value function
        };

		private:
			//
		protected:
			uint64_t uid;
			llvm::Type * irtype;
			Register * reg;
			unsigned size;

			void addRegister() { reg = new Register(size); }

		public:
			Value(uint64_t id) {
				uid = id;
				size = 0;
			}
			virtual void initialize(llvm::Value * irval, irvmap * irmap);
			uint64_t getUID() { return uid; }
			// add register function

			// virtual Value* clone() const = 0;
	};
	class ConstantValue : public Value {
		private:
		protected:
			uint64_t constData;
		public:
			ConstantValue(uint64_t id) : Value(id) { }
			virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
			template<typename T>
			T getConstData() { return *(T *)&constData; }
	};

	class GlobalConstantValue : public ConstantValue {
		private:
		protected:
		public:
			GlobalConstantValue(uint64_t id) : ConstantValue(id) { }
			virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
	};

	class Argument : public Value {
		private:
		protected:
		public:
			Argument(uint64_t id) : Value(id) { }
			virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
	};
}


#endif //__SALAM_VALUE_HH__