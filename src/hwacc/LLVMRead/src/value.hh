#ifndef __SALAM_VALUE_HH__
#define __SALAM_VALUE_HH__

#include "registers.hh"
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/APFloat.h"
#include <llvm-c/Core.h>

#include <map>
#include <memory>
#include <vector>

namespace SALAM {
	class Value;
	typedef std::pair<llvm::Value *, std::shared_ptr<Value>> irvmaptype;
	typedef std::map<llvm::Value *, std::shared_ptr<Value>> irvmap;
	typedef std::vector<std::shared_ptr<Value>> valueListTy;

	class Value {
		/*****************************************************************************
		* Register is the data storage container for SALAM::Values.
		* Every instruction and function argument has a corresponding register that
		* is tracked for power/area/timing. Additionally Constants have corresponding
		* registers, which are not tracked, since they do not have a timing component.
		*****************************************************************************/
		class Register {
            protected:
            	bool tracked;
            	bool isNULL = false;
            public:
                Register(bool trk=true, bool nul=false) : tracked(trk), isNULL(nul) { TRACEOUT("SALAM::Value::Register::Register"); }
                virtual llvm::APFloat * getFloatData() = 0;
                virtual llvm::APSInt * getIntData() = 0;
                virtual uint64_t * getPtrData() = 0;
                virtual bool isInt() { return false; }
                virtual bool isFP() { return false; }
                virtual bool isPtr() { return false; }
				bool isTracked() { return tracked; }
                bool isNull() { return isNULL; }
                void setNull(bool flag) { isNULL = flag; }
                void setTracked(bool flag) { tracked = flag; }
        };
        class APFloatRegister : public Register {
        	private:
        		llvm::APFloat *data;
        	public:
        		APFloatRegister(llvm::Type * T, bool isTracked=true) : Register(isTracked) {
					TRACEOUT("SALAM::Value::Register::APFloatRegister::APFloatRegister");
        			switch (T->getTypeID()) {
        				case llvm::Type::HalfTyID:
        				{
        					data = new llvm::APFloat(llvm::APFloat::IEEEhalf());
        					break;
        				}
        				case llvm::Type::FloatTyID:
        				{
        					data = new llvm::APFloat(llvm::APFloat::IEEEsingle());
        					break;
        				}
        				case llvm::Type::DoubleTyID:
        				{
        					data = new llvm::APFloat(llvm::APFloat::IEEEdouble());
        					break;
        				}
        				case llvm::Type::X86_FP80TyID:
        				{
        					data = new llvm::APFloat(llvm::APFloat::x87DoubleExtended());
        					break;
        				}
        				case llvm::Type::FP128TyID:
        				{
        					data = new llvm::APFloat(llvm::APFloat::IEEEquad());
        					break;
        				}
        				case llvm::Type::PPC_FP128TyID:
        				{
        					data = new llvm::APFloat(llvm::APFloat::PPCDoubleDouble());
        					break;
        				}
        				default:
        					assert(0);
        			}
        		}
        		APFloatRegister(const llvm::APFloat &RHS) : Register(false) {
					TRACEOUT("SALAM::Value::Register::APFloatRegister::APFloatRegister");
        			data = new llvm::APFloat(RHS);
        		}
        		virtual llvm::APFloat * getFloatData() override { return data; }
                virtual llvm::APSInt * getIntData() {
                	assert(0);
                	return NULL;
                }
                virtual uint64_t * getPtrData() {
                	assert(0);
                	return NULL;
                }
        		virtual bool isFP() override { return true; }
        };
        class APIntRegister : public Register {
    		private:
    			llvm::APSInt *data;
    		public:
    			APIntRegister(llvm::Type * T, bool isTracked=true) : Register(isTracked) {
					TRACEOUT("SALAM::Value::Register::APIntRegister::APIntRegister");
    				llvm::IntegerType * it = llvm::dyn_cast<llvm::IntegerType>(T);
    				assert(it);
    				data = new llvm::APSInt(it->getBitWidth(), 0);
    			}
    			APIntRegister(const llvm::APInt &RHS) : Register(false) {
					TRACEOUT("SALAM::Value::Register::APIntRegister::APIntRegister");
    				data = new llvm::APSInt(RHS);
    			}
    			virtual llvm::APSInt * getIntData() override { return data; }
    			virtual llvm::APFloat * getFloatData() {
    				assert(0);
    				return NULL;
    			}
                virtual uint64_t * getPtrData() {
                	assert(0);
                	return NULL;
                }
    			virtual bool isInt() override { return true; }
        };
        class PointerRegister : public Register {
        	private:
        		uint64_t *pointer;
        	public:
        		PointerRegister(bool isTracked=true, bool isNull=false) : Register(isTracked, isNull), pointer(new uint64_t(0)) { TRACEOUT("SALAM::Value::Register::PointerRegister::PointerRegister"); }
        		PointerRegister(uint64_t val, bool isTracked=true, bool isNull=false) : Register(isTracked, isNull), pointer(new uint64_t(val)) { TRACEOUT("SALAM::Value::Register::PointerRegister::PointerRegister"); }
        		virtual bool isPtr() override { return true; }
        		virtual uint64_t * getPtrData() override { return pointer; }
        		virtual llvm::APFloat * getFloatData() {
        			assert(0);
        			return NULL;
        		}
                virtual llvm::APSInt * getIntData() {
                	assert(0);
                	return NULL;
                }
        };

		protected:
			uint64_t uid = 0;
			uint64_t OpCode = 0;
			llvm::Type * irtype;
			Register * reg;
			unsigned size;
			
			void addRegister(bool istracked=true);
			void addAPIntRegister(const llvm::APInt & val);
			void addAPIntRegister(const llvm::APSInt & val);
			void addAPFloatRegister(const llvm::APFloat & val);
			void addPointerRegister(bool isTracked=true, bool isNull=false);
			void addPointerRegister(uint64_t val, bool isTracked=true, bool isNull=false);

		public:
			Value(uint64_t id) {
				CLASSOUT("SALAM::Value::Value(uint64_t)", id);
				uid = id;
				size = 0;
			}
			Value(uint64_t id, uint64_t Op) {
				CLASSOUT("SALAM::Value::Value(uint64_t)", id);
				uid = id;
				OpCode = Op;
				size = 0;
			}
			virtual void initialize(llvm::Value * irval, irvmap * irmap);
			uint64_t getUID() { return uid; }
			uint64_t getOpCode() { return OpCode; }
			Register * getReg() { return reg; }

			// virtual Value* clone() const = 0;
	};

	class Constant: public Value {
		private:
		protected:
			SALAM::valueListTy operands;
		public:
			Constant(uint64_t id) : Value(id) { TRACEOUT("SALAM::Constant::Constant"); }
			virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * values);
	};

	class GlobalConstant : public Constant {
		private:
		protected:
		public:
			GlobalConstant(uint64_t id) : Constant(id) { TRACEOUT("SALAM::GlobalConstant::GlobalConstant"); }
			virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * values) override;
	};

	class Argument : public Value {
		private:
		protected:
		public:
			Argument(uint64_t id) : Value(id) { TRACEOUT("SALAM::Argument::Argument"); }
			virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
	};
}


#endif //__SALAM_VALUE_HH__