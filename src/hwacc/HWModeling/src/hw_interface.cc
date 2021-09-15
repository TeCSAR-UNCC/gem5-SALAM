#include "hw_interface.hh"

HWInterface::HWInterface(const HWInterfaceParams &params) :
    SimObject(params),
    cycle_counts(params.cycle_counts),
    functional_units(params.functional_units),
    hw_statistics(params.hw_statistics),
    inst_config(params.inst_config),
    opcodes(params.opcodes),
    salam_power_model(params.salam_power_model),
    simulator_config(params.simulator_config) { }


// HWInterface*
// HWInterfaceParams::create() {
//     return new HWInterface(this);
// }