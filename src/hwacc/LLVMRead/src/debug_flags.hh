#ifndef LLVMREAD_DEBUG_HH
#define LLVMREAD_DEBUG_HH
// License 

//____________________________________________________________________________
// Doxygen setup
/**
 * @defgroup flags Flags group
 */

//____________________________________________________________________________
// File description
/**
 * @file
 * Contains include information for all M5 debug flags and base 
 * implementation of the Debugger class used within gem5-SALAM. 
 */

//____________________________________________________________________________
// Global includes
// - debug_flags.hh is a common header for all files, so all files included
// - here will available throughout the entire application
#include "macros.hh"

//____________________________________________________________________________
// Debug includes
#include "debug/AddrRanges.hh"
#include "debug/CommInterface.hh"
#include "debug/CommInterfaceQueues.hh"
#include "debug/DeviceMMR.hh"
#include "debug/DMA.hh"
#include "debug/LLVMInterface.hh"
#include "debug/NoncoherentDma.hh"
#include "debug/LLVMParse.hh"
#include "debug/Runtime.hh"
#include "debug/RuntimeCompute.hh"
#include "debug/RuntimeQueues.hh"
#include "debug/SALAM_Debug.hh"
#include "debug/StreamDma.hh"
#include "debug/Trace.hh"
#include "debug/Step.hh"

//____________________________________________________________________________
// M5 includes
#include "base/trace.hh"

/**
 * @namespace SALAM
 */
namespace SALAM
{

/**
 * @class Debugger
 * Base implementation of the debugger used in gem5-SALAM
 */
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
