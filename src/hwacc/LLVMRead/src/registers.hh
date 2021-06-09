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
        unsigned reads = 0;
        unsigned writes = 0;

        class Register_Debugger: public Debugger
        {
            public:
                Register_Debugger();
                ~Register_Debugger() = default;
                virtual void dumper(SALAM::Register *reg);
        }; 

        Register_Debugger* reg_dbg;

    public:
        Register(bool trk=true,
                 bool nul=false);
        ~Register();
        virtual llvm::APFloat *getFloatData(bool incReads=true) {
            assert(0 && "Attempted to read float data from non-float register");
            return NULL;
        }
        virtual llvm::APSInt *getIntData(bool incReads=true) {
            assert(0 && "Attempted to read integer data from non-integer register");
            return NULL;
        }
        virtual uint64_t *getPtrData(bool incReads=true) {
            assert(0 && "Attempted to read pointer data from non-pointer register");
            return NULL;
        }
        virtual void writeFloatData(llvm::APFloat * apf, bool incWrites=true) {
            assert(0 && "Attempted to write float data on non-float register");
        }
        virtual void writeIntData(llvm::APInt * api, bool incWrites=true) {
            assert(0 && "Attempted to write interger data on non-integer register");
        }
        virtual void writePtrData(uint64_t * ptr, size_t len=8, bool incWrites=true) {
            assert(0 && "Attempted to write pointer data on non-pointer register");
        }
        virtual bool isInt() { return false; }
        virtual bool isFP() { return false; }
        virtual bool isPtr() { return false; }
        bool isTracked() { return tracked; }
        bool isNull() { return isNULL; }
        void setNull(bool flag) { isNULL = flag; }
        void setTracked(bool flag) { tracked = flag; }
        void dump() { if (dbg) reg_dbg->dumper(this); }
};

class APFloatRegister : public Register
{
    private:
        llvm::APFloat *data;
    public:
        APFloatRegister(llvm::Type *T,
                        bool isTracked=true);
        APFloatRegister(const llvm::APFloat &RHS);
        virtual llvm::APFloat * getFloatData(bool incReads=true) override;
        virtual void writeFloatData(llvm::APFloat * apf, bool incWrites=true) override;
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
        virtual llvm::APSInt * getIntData(bool incReads=true) override;
        virtual void writeIntData(llvm::APInt * api, bool incWrites=true) override;
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
        virtual uint64_t * getPtrData(bool incReads=true) override;
        virtual void writePtrData(uint64_t * ptr, size_t len=8, bool incWrites=true) override;
};
} // End SALAM Namespace
#endif