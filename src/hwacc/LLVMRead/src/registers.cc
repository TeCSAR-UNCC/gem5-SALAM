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
        this->reg_dbg = new Register_Debugger();
    }
}

SALAM::Register::~Register() 
{
        if (DTRACE(Trace)) DPRINTF(Runtime, "Trace Deleted: %s \n", __PRETTY_FUNCTION__);
        //if (DTRACE(SALAM_Debug)) delete reg_dbg;
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

SALAM::APFloatRegister::APFloatRegister(const llvm::APFloat &RHS) :
                                        Register(false)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    data = new llvm::APFloat(RHS);
}

SALAM::APIntRegister::APIntRegister(llvm::Type * T,
                                    bool tracked) :
                                    Register(tracked)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    llvm::IntegerType * it = llvm::dyn_cast<llvm::IntegerType>(T);
    assert(it);
    data = new llvm::APSInt(it->getBitWidth(), 0);
}

SALAM::APIntRegister::APIntRegister(const llvm::APInt &RHS) :
                                    Register(false)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    data = new llvm::APSInt(RHS);
}

SALAM::PointerRegister::PointerRegister(bool tracked,
                                        bool isNull) :
                                        Register(tracked,
                                        isNull),
                                        pointer(new uint64_t(0))
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

SALAM::PointerRegister::PointerRegister(uint64_t val,
                                        bool tracked,
                                        bool isNull) :
                                        Register(tracked,
                                        isNull),
                                        pointer(new uint64_t(val))
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

llvm::APFloat *
SALAM::APFloatRegister::getFloatData(bool incReads)
{
    if (incReads && tracked) reads++;
    return data;
}

void
SALAM::APFloatRegister::writeFloatData(llvm::APFloat * apf, bool incWrites)
{
    if (incWrites && tracked) writes++;
    *data = *apf;
}

llvm::APSInt *
SALAM::APIntRegister::getIntData(bool incReads)
{
    if (incReads && tracked) reads++;
    return data;
}

void
SALAM::APIntRegister::writeIntData(llvm::APInt * api, bool incWrites)
{
    if (incWrites && tracked) writes++;
    *data = *api;
}

uint64_t *
SALAM::PointerRegister::getPtrData(bool incReads)
{
    if (incReads && tracked) reads++;
    return pointer;
}

void
SALAM::PointerRegister::writePtrData(uint64_t * ptr, size_t len, bool incWrites)
{
    if (incWrites && tracked) writes++;
    std::memcpy(pointer, ptr, len);
}