//------------------------------------------//
#include "registers.hh"
//------------------------------------------//

SALAM::Register::Register(bool trk,
                          bool nul) :
                          tracked(trk),
                          isNULL(nul)
{
    //if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

SALAM::APFloatRegister::APFloatRegister(llvm::Type * T,
                                        bool isTracked) :
                                        Register(isTracked)
{
    //if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
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
    //if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    data = new llvm::APFloat(RHS);
}

SALAM::APIntRegister::APIntRegister(llvm::Type * T,
                                    bool isTracked) :
                                    Register(isTracked)
{
    //if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    llvm::IntegerType * it = llvm::dyn_cast<llvm::IntegerType>(T);
    assert(it);
    data = new llvm::APSInt(it->getBitWidth(), 0);
}

SALAM::APIntRegister::APIntRegister(const llvm::APInt &RHS) :
                                    Register(false)
{
    //if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    data = new llvm::APSInt(RHS);
}

SALAM::PointerRegister::PointerRegister(bool isTracked,
                                        bool isNull) :
                                        Register(isTracked,
                                        isNull),
                                        pointer(new uint64_t(0))
{
    //if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

SALAM::PointerRegister::PointerRegister(uint64_t val,
                                        bool isTracked,
                                        bool isNull) :
                                        Register(isTracked,
                                        isNull),
                                        pointer(new uint64_t(val))
{
    //if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}