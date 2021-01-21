#ifndef __HWACC_REGISTERS_HH__
#define __HWACC_REGISTERS_HH__

#include "debug_flags.hh"
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/APFloat.h"
#include <llvm-c/Core.h>

namespace SALAM
{
/*****************************************************************************
* Register is the data storage container for SALAM::Values.
* Every instruction and function argument has a corresponding register that
* is tracked for power/area/timing. Additionally Constants have corresponding
* registers, which are not tracked, since they do not have a timing component.
*****************************************************************************/
class Register
{
    protected:
        uint64_t* raw;
        bool tracked;
        bool isNULL = false;
        bool dbg = false;

        class Register_Debugger: public Debugger
        {
            public:
                Register_Debugger();
                ~Register_Debugger() = default;
                virtual void dumper();
        }; 

        Register_Debugger* reg_dbg;

    public:
        Register(bool trk=true,
                 bool nul=false);
        ~Register();
        virtual llvm::APFloat *getFloatData() = 0;
        virtual llvm::APSInt *getIntData() = 0;
        virtual uint64_t *getPtrData() = 0;
        virtual bool isInt() { return false; }
        virtual bool isFP() { return false; }
        virtual bool isPtr() { return false; }
        bool isTracked() { return tracked; }
        bool isNull() { return isNULL; }
        void setNull(bool flag) { isNULL = flag; }
        void setTracked(bool flag) { tracked = flag; }
        void dump() { if (dbg) reg_dbg->dumper(); }
};

class APFloatRegister : public Register
{
    private:
        llvm::APFloat *data;
    public:
        APFloatRegister(llvm::Type *T,
                        bool isTracked=true);
        APFloatRegister(const llvm::APFloat &RHS);
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

class APIntRegister : public Register
{
    private:
        llvm::APSInt *data;
    public:
        APIntRegister(llvm::Type * T,
                      bool isTracked=true);
        APIntRegister(const llvm::APInt &RHS);
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

class PointerRegister : public Register
{
    private:
        uint64_t *pointer;
    public:
        PointerRegister(bool isTracked=true,
                        bool isNull=false);
        PointerRegister(uint64_t val,
                        bool isTracked=true,
                        bool isNull=false);
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
} // End SALAM Namespace
#endif