#ifndef LLVMREAD_DEBUG_HH
#define LLVMREAD_DEBUG_HH
//------------------------------------------//
#include "macros.hh"   // Included here so all files have access
//------------------------------------------//
#include "debug/AddrRanges.hh"
#include "debug/CommInterface.hh"
#include "debug/CommInterfaceQueues.hh"
#include "debug/DeviceMMR.hh"
#include "debug/DMA.hh"
#include "debug/LLVMInterface.hh"
#include "debug/NoncoherentDma.hh"
#include "debug/Runtime.hh"
#include "debug/RuntimeQueues.hh"
#include "debug/SALAM_Debug.hh"
#include "debug/StreamDma.hh"
#include "debug/Trace.hh"
#include "base/trace.hh"

namespace SALAM
{
class Debugger
{
    private:
        bool dbg = false;
    protected:
    public:
        Debugger();
        ~Debugger() = default;
        virtual void dumper() { };
        bool enabled() { return dbg; }

};
}
//------------------------------------------//
#endif //__LLVMREAD_DEBUG_HH__
