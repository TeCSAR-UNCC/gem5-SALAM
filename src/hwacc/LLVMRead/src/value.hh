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

class Value
{
    protected:
        uint64_t uid = 0;
        uint64_t size = 0;
        llvm::Type *irtype;
        Register *reg;
        bool dbg = false;

        void addRegister(bool isTracked=true);
        void addAPIntRegister(const llvm::APInt & val);
        void addAPIntRegister(const llvm::APSInt & val);
        void addAPFloatRegister(const llvm::APFloat & val);
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
                void dumper()   override;
        }; 

        Value_Debugger* value_dbg;

    public:
        Value(uint64_t id);
        ~Value();
        virtual void initialize(llvm::Value *irval,
                                SALAM::irvmap *irmap);
        uint64_t getSize() { return size; }
        uint64_t getUID() { return uid; }
        Register *getReg() { return reg; }
        llvm::Type *getType() { return irtype; }
        //virtual Value *clone() = 0;
};
} // End SALAM Namespace


#endif //__SALAM_VALUE_HH__