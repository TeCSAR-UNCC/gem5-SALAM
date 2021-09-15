#include "simulator_config.hh"

SimulatorConfig::SimulatorConfig(const SimulatorConfigParams &params) :
    SimObject(params) { }

// SimulatorConfig*
// SimulatorConfigParams::create() {
//     //// if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
//     return new SimulatorConfig(this);
// }