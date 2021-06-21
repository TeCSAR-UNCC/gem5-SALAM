#ifndef __SALAM_VALUE_HH__
#define __SALAM_VALUE_HH__

#include "debug_flags.hh"
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

namespace SALAM
{
class Value;
typedef std::pair<llvm::Value *, std::shared_ptr<Value>> irvmaptype;
typedef std::map<llvm::Value *, std::shared_ptr<Value>> irvmap;
typedef std::vector<std::shared_ptr<Value>> valueListTy;
//typedef std::vector<std::shared_ptr<SALAM::Register>> Ops;

class Value
{
    public:

    protected:
        uint64_t uid = 0;
        uint64_t size = 0;
        std::string ir_string;
        llvm::Type *irtype;
        std::shared_ptr<SALAM::Register> returnReg;
        //Ops opReg;
        bool dbg = false;

        void addRegister(bool isTracked=true);
    #ifdef USE_AP_VALUES
        void addAPIntRegister(const llvm::APInt & val);
        void addAPIntRegister(const llvm::APSInt & val);
        void addAPFloatRegister(const llvm::APFloat & val);
    #else
        void addAPIntRegister(const uint64_t & val);
        void addAPFloatRegister(const uint64_t & val);
    #endif
        void addPointerRegister(bool isTracked=true,
                                bool isNull=false);
        void addPointerRegister(uint64_t val,
                                bool isTracked=true,
                                bool isNull=false);

        class Value_Debugger: public Debugger
        {
            public:
                Value_Debugger();
                ~Value_Debugger() = default;
                virtual void dumper(SALAM::Value * value);
        }; 

        Value_Debugger* value_dbg;
        Value(const SALAM::Value &copy_val);

    public:
        /** Class Constructor.
         * @param id 
         */
        Value(uint64_t id);
        Value& operator = (Value &copy_val);
        ~Value();
        virtual void initialize(llvm::Value *irval,
                                SALAM::irvmap *irmap);
        uint64_t getSize() { return size; }
        uint64_t getSizeInBytes() {
            if (size==0)
                return 0;
            else
                return ((size - 1) >> 3) + 1;
        }
        uint64_t getUID() const { return uid; }
        std::shared_ptr<SALAM::Register> getReg() { return returnReg; }
        llvm::Type *getType() { return irtype; }
        std::string getIRString() { return ir_string; }

        // Helper functions for setting the value of the return register directly from the value
        // Using these functions will increment the write counters on tracked registers
        // If you'd like to avoid incrementing write counters, directly pull the register and
        // use its appropriate write function
    #ifdef USE_AP_VALUES
        void setRegisterValue(const llvm::APInt &data);
        void setRegisterValue(const llvm::APFloat &data);
    #endif
        void setRegisterValue(const uint64_t data);
        void setRegisterValue(uint8_t * data);
        void setRegisterValue(bool data);
        void setRegisterValue(std::shared_ptr<SALAM::Register> reg);

        // Helper functions for getting the value of the return register directly from the value
        // Using these functions will increment the read counters on tracked registers
        // If you'd like to avoid incrementing read counters, directly pull the register and
        // use its appropriate read function
        virtual uint64_t * getPtrRegValue() { return returnReg->getPtrData(); }
    #ifdef USE_AP_VALUES
        virtual llvm::APFloat * getFloatRegValue() { return returnReg->getFloatData(); }
        virtual llvm::APSInt * getIntRegValue() { return returnReg->getIntData(); }
    #else
        virtual uint64_t * getFloatRegValue() { return returnReg->getFloatData(); }
        virtual float getFloatFromReg() { return returnReg->getFloat(); }
        virtual double getDoubleFromReg() { return returnReg->getDouble(); }
        virtual uint64_t * getIntRegValue() { return returnReg->getIntData(); }
        virtual uint64_t getUIntRegValue() { return returnReg->getUnsignedInt(); }
        virtual int64_t getSIntRegValue() { return returnReg->getSignedInt(size); }
    #endif

        virtual bool isConstant() { return false; }
        virtual bool isGlobalConstant() { return false; }
        virtual bool isArgument() { return false; }
        virtual bool isFunction() { return false; }
        virtual bool isBasicBlock() { return false; }
        virtual bool isInstruction() { return false; }

        void value_dump() { if (dbg) value_dbg->dumper(this); }
        std::shared_ptr<SALAM::Value> clone() const { return createClone(); }
        virtual std::shared_ptr<SALAM::Value> createClone() const { return std::shared_ptr<SALAM::Value>(new SALAM::Value(*this)); }

};
} // End SALAM Namespace


#endif //__SALAM_VALUE_HH__