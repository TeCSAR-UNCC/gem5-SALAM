//------------------------------------------//
#include "registers.hh"
//------------------------------------------//

SALAM::Register::Register(bool trk,
                          bool nul) :
                          tracked(trk),
                          isNULL(nul)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbg = true;
        // this->reg_dbg = new Register_Debugger();
    }
}

SALAM::Register::~Register()
{
        if (DTRACE(Trace)) DPRINTF(Runtime, "Trace Deleted: %s \n", __PRETTY_FUNCTION__);
        // if (DTRACE(SALAM_Debug)) delete reg_dbg;
}

SALAM::Register::Register_Debugger::Register_Debugger()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

void
SALAM::Register::Register_Debugger::dumper(SALAM::Register *reg)
{
    if (DTRACE(SALAM_Debug)) {
        if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s\n", __PRETTY_FUNCTION__);
        std::string type = "N/A";
        if (reg->isInt()) type = "Int";
        else if (reg->isFP()) type = "FP";
        else if (reg->isPtr()) type = "Ptr";

        DPRINTF(SALAM_Debug, "| %s |\n\t\t %s %s  \n\n", 
            "************** Register Dump **************",
            "   Type: ", type
        );
    }
}


SALAM::APFloatRegister::APFloatRegister(llvm::Type * T,
                                        bool tracked) :
                                        Register(tracked)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    #if USE_LLVM_AP_VALUES
    switch (T->getTypeID()) {
        case llvm::Type::FloatTyID:
        {
            data = llvm::APFloat::getZero(llvm::APFloat::IEEEsingle());
            break;
        }
        case llvm::Type::DoubleTyID:
        {
            data = llvm::APFloat::getZero(llvm::APFloat::IEEEdouble());
            break;
        }
        case llvm::Type::HalfTyID:
        {
            data = llvm::APFloat::getZero(llvm::APFloat::IEEEhalf());
            break;
        }
        case llvm::Type::X86_FP80TyID:
        {
            data = llvm::APFloat::getZero(llvm::APFloat::x87DoubleExtended());
            break;
        }
        case llvm::Type::FP128TyID:
        {
            data = llvm::APFloat::getZero(llvm::APFloat::IEEEquad());
            break;
        }
        case llvm::Type::PPC_FP128TyID:
        {
            data = llvm::APFloat::getZero(llvm::APFloat::PPCDoubleDouble());
            break;
        }
        default:
            assert(0 && "Specified Floating Point type is not supported");
    }
    #else
    switch (T->getTypeID()) {
        case llvm::Type::FloatTyID:
        {
            data = 0;
            break;
        }
        case llvm::Type::DoubleTyID:
        {
            data = 0;
            break;
        }
        default:
            assert(0 && "Specified Floating Point type is not supported");
    }
    #endif
}

SALAM::APFloatRegister::APFloatRegister(const llvm::APFloat &RHS) :
                                        Register(false)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    #if USE_LLVM_AP_VALUES
        data = (RHS);
    #else
        auto bitcast = RHS.bitcastToAPInt();
        data = (uint64_t)(bitcast.getLimitedValue());
    #endif
}

SALAM::APIntRegister::APIntRegister(llvm::Type * T,
                                    bool tracked) :
                                    Register(tracked)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    #if USE_LLVM_AP_VALUES
        llvm::IntegerType * it = llvm::dyn_cast<llvm::IntegerType>(T);
        assert(it);
        data = llvm::APSInt::getMinValue(it->getBitWidth(), true);
    #else
        data = 0;
    #endif
}

SALAM::APIntRegister::APIntRegister(const llvm::APInt &RHS) :
                                    Register(false)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    #if USE_LLVM_AP_VALUES
        data = RHS;
    #else
        data = (uint64_t)(RHS.getLimitedValue());
    #endif
}

SALAM::PointerRegister::PointerRegister(bool tracked,
                                        bool isNull) :
                                        Register(tracked,
                                        isNull),
                                        pointer(0)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

SALAM::PointerRegister::PointerRegister(uint64_t val,
                                        bool tracked,
                                        bool isNull) :
                                        Register(tracked,
                                        isNull),
                                        pointer(val)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

#if USE_LLVM_AP_VALUES
    llvm::APFloat
    SALAM::APFloatRegister::getFloatData(bool incReads)
    {
        if (incReads && tracked) reads++;
        return data;
    }

    void
    SALAM::APFloatRegister::writeFloatData(llvm::APFloat apf, bool incWrites)
    {
        if (incWrites && tracked) writes++;
        data = apf;
    }
#else
    uint64_t
    SALAM::APFloatRegister::getFloatData(bool incReads)
    {
        if (incReads && tracked) reads++;
        return data;
    }

    float
    SALAM::APFloatRegister::getFloat(bool incReads) {
        if (incReads && tracked) reads++;
        return *(float *)&data;
    }

    double
    SALAM::APFloatRegister::getDouble(bool incReads) {
        if (incReads && tracked) reads++;
        return *(double *)&data;
    }

    void
    SALAM::APFloatRegister::writeFloatData(uint64_t apf, size_t len, bool incWrites)
    {
        if (incWrites && tracked) writes++;
        std::memcpy(&data, &apf, len);
    }
#endif

#if USE_LLVM_AP_VALUES
    llvm::APSInt
    SALAM::APIntRegister::getIntData(bool incReads)
    {
        if (incReads && tracked) reads++;
        return data;
    }

    void
    SALAM::APIntRegister::writeIntData(llvm::APInt api, bool incWrites)
    {
        if (incWrites && tracked) writes++;
        data = api;
    }
#else
    uint64_t
    SALAM::APIntRegister::getIntData(bool incReads)
    {
        if (incReads && tracked) reads++;
        return data;
    }

    uint64_t
    SALAM::APIntRegister::getUnsignedInt(bool incReads) {
        if (incReads && tracked) reads++;
        return data;
    }

    int64_t
    SALAM::APIntRegister::getSignedInt(size_t sizeInBits, bool incReads) {
        if (incReads && tracked) reads++;
        int64_t tmp;
        switch (sizeInBits) {
            case 8:
            {
                tmp = (int64_t)((int8_t)(data));
                break;
            }
            case 16:
            {
                tmp = (int64_t)((int16_t)(data));
                break;
            }
            case 32:
            {
                tmp = (int64_t)((int32_t)(data));
                break;
            }
            case 64:
            {
                tmp = (int64_t)(data);
                break;
            }
            default:
            {
                assert(0 && "Must use AP values for nonstandard int sizes.");
                break;
            }
        }
        return tmp;
    }

    void
    SALAM::APIntRegister::writeIntData(uint64_t api, size_t len, bool incWrites)
    {
        if (incWrites && tracked) writes++;
        std::memcpy(&data, &api, len);
    }
#endif
uint64_t
SALAM::PointerRegister::getPtrData(bool incReads)
{
    if (incReads && tracked) reads++;
    return pointer;
}

void
SALAM::PointerRegister::writePtrData(uint64_t ptr, size_t len, bool incWrites)
{
    if (incWrites && tracked) writes++;
    std::memcpy(&pointer, &ptr, len);
}

#include <sstream>
#include <ios>

std::string
SALAM::APFloatRegister::dataString() {
    std::stringstream ss;
#if USE_LLVM_AP_VALUES

#else
    ss << "0x" << std::hex << data;
#endif
    return ss.str();
}

std::string
SALAM::APIntRegister::dataString() {
    std::stringstream ss;
#if USE_LLVM_AP_VALUES

#else
    ss << "0x" << std::hex << data;
#endif
    return ss.str();
}

std::string
SALAM::PointerRegister::dataString() {
    std::stringstream ss;
#if USE_LLVM_AP_VALUES

#else
    ss << "0x" << std::hex << pointer;
#endif
    return ss.str();
}