#ifndef __HWACC_OPERAND_HH__
#define __HWACC_OPERAND_HH__
//------------------------------------------//
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Instructions.h"
#include <llvm-c/Core.h>
#include "debug_flags.hh"
#include "value.hh"
#include "registers.hh"
#include <map>
#include <memory>
#include <vector>

namespace SALAM
{


class Operand: public Value
{
    private:
        std::shared_ptr<SALAM::Register> lockedValue;
        bool set = false;

    protected:
        class Operand_Debugger: public Debugger
        {
            public:
                Operand_Debugger();
                ~Operand_Debugger() = default;
                virtual void dumper(SALAM::Operand *op);
        };

        Operand_Debugger* op_dbg;
    public:
        void dump() { if (dbg) op_dbg->dumper(this); }
        void initOperandReg();
        //Operand(uint64_t id);
        Operand(const SALAM::Value &copy_val);
        Operand(const Operand &copy_val);
        Operand(std::shared_ptr<SALAM::Value> copy_val);
        Operand& operator = (Operand &copy_val);
        ~Operand() = default;
        //Value *clone() { return new Operand(*this); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
        void updateOperandRegister();

        virtual uint64_t getPtrRegValue() { return lockedValue->getPtrData(); }
    #if USE_LLVM_AP_VALUES
        virtual llvm::APFloat getFloatRegValue() { return lockedValue->getFloatData(); }
        virtual llvm::APSInt getIntRegValue() { return lockedValue->getIntData(); }
        virtual bool hasIntVal() { return lockedValue->isInt(); }
        virtual bool hasPtrVal() { return lockedValue->isPtr(); }
    #else
        virtual uint64_t getFloatRegValue() { return lockedValue->getFloatData(); }
        virtual float getFloatFromReg() { return lockedValue->getFloat(); }
        virtual double getDoubleFromReg() { return lockedValue->getDouble(); }
        virtual uint64_t getIntRegValue() { return lockedValue->getIntData(); }
        virtual uint64_t getUIntRegValue() { return lockedValue->getUnsignedInt(); }
        virtual int64_t getSIntRegValue() { return lockedValue->getSignedInt(size); }
        virtual bool hasIntVal() { return lockedValue->isInt(); }
        virtual bool hasPtrVal() { return lockedValue->isPtr(); }
    #endif
        std::shared_ptr<SALAM::Register> getOpRegister() { return lockedValue; }
};

class Constant: public Value {
    private:
    protected:
        SALAM::valueListTy operands;
    public:
        Constant(uint64_t id, gem5::SimObject * owner, bool dbg);
        ~Constant() = default;
        virtual bool isConstant() { return true; }
        //Value *clone() { return new Constant(*this); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * values);
};

class GlobalConstant : public Constant {
    private:
    protected:
    public:
        GlobalConstant(uint64_t id, gem5::SimObject * owner, bool dbg);
        ~GlobalConstant() = default;
        virtual bool isGlobalConstant() { return true; }
        //Value *clone() { return new GlobalConstant(*this); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * values) override;
};

class Argument : public Value {
    private:
    protected:
    public:
        Argument(uint64_t id, gem5::SimObject * owner, bool dbg);
        ~Argument() = default;
        virtual bool isArgument() { return true; }
        //Value *clone() { return new Argument(*this); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
};

}

//------------------------------------------//
#endif //__HWACC_OPERAND_HH__