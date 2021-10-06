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

bool 
HWInterface::availableFunctionalUnit(uint64_t functional_unit) {
    switch(functional_unit) {
        case INTADDER : { }
        case INTMULTI : { }
        case INTSHIFTER : { }
        case INTBITWISE : { }
        case FPSPADDER : { }
        case FPDPADDER : { }
        case FPSPMULTI : { }
        case FPSPDIVID : { }
        case FPDPMULTI : { }
        case FPDPDIVID : { }
        case COMPARE : { }
        case GETELEMENTPTR : { }
        case CONVERSION : { }
        case OTHERINST : { }
        case REGISTER : { }
        case COUNTER : { }
        default: { 
            // assert() 
            return false;
        }
    }
    return false;
}