#include "salam_power_model.hh"

SALAMPowerModel::SALAMPowerModel(const SALAMPowerModelParams &params) :
    SimObject(params) { }

// SALAMPowerModel*
// SALAMPowerModelParams::create() {
//     //// if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
//     return new SALAMPowerModel(this);
// }