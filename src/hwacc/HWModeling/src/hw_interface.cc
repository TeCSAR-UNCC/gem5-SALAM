#include "hw_interface.hh"

HWInterface::HWInterface(HWInterfaceParams *params):
    SimObject(params),
    cycleCounts(params->cycleCounts),
    testParam(params->testParam) { }

HWInterface*
HWInterfaceParams::create() {
    //if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return new HWInterface(this);
}