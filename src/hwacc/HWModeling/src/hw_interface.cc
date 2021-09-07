#include "hw_interface.hh"

HWInterface::HWInterface(HWInterfaceParams *params) :
    SimObject(params) { }

HWInterface*
HWInterfaceParams::create() {
    //if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
   // std::cout << "\n\n\n\n Initialized HW Model \n\n\n\n";
    return new HWInterface(this);
}