#ifndef __HWACC_REGISTERS_HH__
#define __HWACC_REGISTERS_HH__

#include "debug_flags.hh"
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/APFloat.h"
#include <llvm-c/Core.h>

#define USE_LLVM_AP_VALUES 0

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
        bool tracked;
        bool isNULL = false;
        bool dbg = false;
        uint64_t reads = 0;
        uint64_t writes = 0;
        uint64_t regdata;

        class Register_Debugger: public Debugger
        {
            public:
                Register_Debugger();
                ~Register_Debugger() = default;
                virtual void dumper(SALAM::Register *reg);
        };

        Register_Debugger reg_dbg;

    public:
        Register(bool trk=true,
                 bool nul=false);
        ~Register();
    #if USE_LLVM_AP_VALUES
        virtual llvm::APFloat getFloatData(bool incReads=true) {
            assert(0 && "Attempted to read float data from non-float register");
            return llvm::APFloat::getNaN(llvm::APFloat::IEEEdouble());
        }
        virtual llvm::APSInt getIntData(bool incReads=true) {
            assert(0 && "Attempted to read integer data from non-integer register");
            return llvm::APSInt::getMinValue(1,true);
        }
    #else
        virtual uint64_t getFloatData(bool incReads=true) {
            assert(0 && "Attempted to read float data from non-float register");
            return 0;
        }
        virtual float getFloat(bool incReads=true) {
            assert(0 && "Attempted to read float data from non-float register");
            return NAN;
        }
        virtual double getDouble(bool incReads=true) {
            assert(0 && "Attempted to read float data from non-float register");
            return NAN;
        }
        virtual uint64_t getIntData(bool incReads=true) {
            assert(0 && "Attempted to read integer data from non-integer register");
            return 0;
        }
        virtual uint64_t getUnsignedInt(bool incReads=true) {
            assert(0 && "Attempted to read integer data from non-integer register");
            return 0;
        }
        virtual int64_t getSignedInt(size_t sizeInBits, bool incReads=true) {
            assert(0 && "Attempted to read integer data from non-integer register");
            return 0;
        }
    #endif
        virtual uint64_t getPtrData(bool incReads=true) {
            assert(0 && "Attempted to read pointer data from non-pointer register");
            return 0;
        }
    #if USE_LLVM_AP_VALUES
        virtual void writeFloatData(llvm::APFloat apf, bool incWrites=true) {
            assert(0 && "Attempted to write float data on non-float register");
        }
        virtual void writeIntData(llvm::APInt api, bool incWrites=true) {
            assert(0 && "Attempted to write interger data on non-integer register");
        }
    #else
        virtual void writeFloatData(uint64_t apf, size_t len=8, bool incWrites=true) {
            assert(0 && "Attempted to write float data on non-float register");
        }
        virtual void writeIntData(uint64_t api, size_t len=8, bool incWrites=true) {
            assert(0 && "Attempted to write interger data on non-integer register");
        }
    #endif
        virtual void writePtrData(uint64_t ptr, size_t len=8, bool incWrites=true) {
            assert(0 && "Attempted to write pointer data on non-pointer register");
        }
        virtual bool isInt() { return false; }
        virtual bool isFP() { return false; }
        virtual bool isPtr() { return false; }
        bool isTracked() { return tracked; }
        bool isNull() { return isNULL; }
        void setNull(bool flag) { isNULL = flag; }
        void setTracked(bool flag) { tracked = flag; }
        void dump() { if (dbg) reg_dbg.dumper(this); }
        uint64_t getReads() { return reads; }
        uint64_t getWrites() { return writes; }
        virtual std::string dataString() = 0;
};

class APFloatRegister : public Register
{
    private:
    #if USE_LLVM_AP_VALUES
        llvm::APFloat data = llvm::APFloat::getZero(llvm::APFloat::IEEEdouble());
    #else
        // We use uint64_t to store the bitcast of the FP value.
        // Compute should be performed after bitcasting back to appropriate type
        uint64_t data = 0;
    #endif
    public:
        APFloatRegister(llvm::Type::TypeID T,
                        bool isTracked);
        APFloatRegister(llvm::Type *T,
                        bool isTracked=true);
        // This constructor is only used for constants.
        APFloatRegister(const llvm::APFloat &RHS);
        // ~APFloatRegister() { if (data) delete data; }
    #if USE_LLVM_AP_VALUES
        virtual llvm::APFloat getFloatData(bool incReads=true) override;
        virtual void writeFloatData(llvm::APFloat apf, bool incWrites=true) override;
    #else
        // This constructor is only used for constants.
        APFloatRegister(const uint64_t RHS) : Register(false) {
            data = RHS;
            regdata = data;    
        }
        virtual uint64_t getFloatData(bool incReads=true) override;
        virtual float getFloat(bool incReads=true) override;
        virtual double getDouble(bool incReads=true) override;
        virtual void writeFloatData(uint64_t apf, size_t len=8, bool incWrites=true) override;
    #endif
        virtual bool isFP() override { return true; }
        virtual std::string dataString() override;
};

class APIntRegister : public Register
{
    private:
    #if USE_LLVM_AP_VALUES
        llvm::APSInt data = llvm::APSInt::getMinValue(1,true);
    #else
        uint64_t data = 0;
    #endif
    public:
        APIntRegister(uint64_t bitwidth,
                      bool isTracked);
        APIntRegister(llvm::Type * T,
                      bool isTracked=true);
        // This constructor is only used for constants.
        APIntRegister(const llvm::APInt &RHS);
        // ~APIntRegister() { if (data) delete data; }
    #if USE_LLVM_AP_VALUES
        virtual llvm::APSInt getIntData(bool incReads=true) override;
        virtual void writeIntData(llvm::APInt api, bool incWrites=true) override;
    #else
        // This constructor is only used for constants.
        APIntRegister(const uint64_t RHS) : Register(false) {
            data = RHS;
            regdata = data;
        }
        virtual uint64_t getIntData(bool incReads=true) override;
        virtual uint64_t getUnsignedInt(bool incReads=true) override;
        virtual int64_t getSignedInt(size_t sizeInBits, bool incReads=true) override;
        virtual void writeIntData(uint64_t api, size_t len=8, bool incWrites=true) override;
    #endif
        virtual bool isInt() override { return true; }
        virtual std::string dataString() override;
};

class PointerRegister : public Register
{
    private:
        uint64_t pointer = 0;
    public:
        PointerRegister(bool isTracked=true,
                        bool isNull=false);
        PointerRegister(uint64_t val,
                        bool isTracked=true,
                        bool isNull=false);
        // ~PointerRegister() { if (pointer) delete pointer; }
        virtual bool isPtr() override { return true; }
        virtual uint64_t getPtrData(bool incReads=true) override;
        virtual void writePtrData(uint64_t ptr, size_t len=8, bool incWrites=true) override;
        virtual std::string dataString() override;
};
} // End SALAM Namespace
#endif