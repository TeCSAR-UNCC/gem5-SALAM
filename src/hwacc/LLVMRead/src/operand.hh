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
#include <map>
#include <memory>
#include <vector>
 
namespace SALAM
{
class OperandValue;
typedef std::pair<OperandValue*, void*> Ops; 

class OperandValue
{   
    protected:
        virtual ~OperandValue() = default;
        virtual void *allocate() const = 0;
        virtual void *cast(void *opType) const = 0; 

};

template<typename T> class OperandType : public OperandValue
{
    protected:
        virtual void *allocate() const { return new T; }
        virtual void *cast(void *opType) const { return static_cast<T*>(opType); }
};

class Operand
{
    protected:
        std::vector<OperandValue *> opsValues;
        std::vector<Ops> ops;
    public:
        OperandValue *setOp(llvm::Type *T, void *value); 

};

class Constant: public Value {
    private:
    protected:
        SALAM::valueListTy operands;
    public:
        Constant(uint64_t id);
        //Constant(const Constant&);
        Value *clone() { return new Constant(*this); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * values);
};

class GlobalConstant : public Constant {
    private:
    protected:
    public:
        GlobalConstant(uint64_t id);
        Value *clone() { return new GlobalConstant(*this); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * values) override;
};

class Argument : public Value {
    private:
    protected:
    public:
        Argument(uint64_t id);
        Value *clone() { return new Argument(*this); }
        virtual void initialize(llvm::Value * irval, irvmap * irmap) override;
};

}

//------------------------------------------//
#endif //__HWACC_OPERAND_HH__