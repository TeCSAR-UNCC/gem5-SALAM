#include "hw_interface.hh"

HWInterface::HWInterface(HWInterfaceParams *params):
    CycleCounts(params),
    //cycleCounts(params->cycleCounts),
    testParam(params->testParam) { }

HWInterface*
HWInterfaceParams::create() {
    //if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
   // std::cout << "\n\n\n\n Initialized HW Model \n\n\n\n";
    return new HWInterface(this);
}