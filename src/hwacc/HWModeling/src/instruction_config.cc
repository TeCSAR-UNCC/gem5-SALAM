#include "instruction_config.hh"

InstConfig::InstConfig(const InstConfigParams &params) :
    SimObject(params) { }

// InstConfig*
// InstConfigParams::create() {
//     //// if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
//     return new InstConfig(this);
// }