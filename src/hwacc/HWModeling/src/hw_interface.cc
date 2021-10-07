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
        case INTADDER : {
            if (functional_units->_integer_adder->is_available()) {
                functional_units->_integer_adder->use_functional_unit();
                return true;
            } break;
        }
        case INTMULTI : {
            if (functional_units->_integer_multiplier->is_available()) {
                functional_units->_integer_multiplier->use_functional_unit();
                return true;
            } break;
        }
        case INTSHIFTER : {
            if (functional_units->_bit_shifter->is_available()) {
                functional_units->_bit_shifter->use_functional_unit();
                return true;
            } break;
        } 
        case INTBITWISE : {
            if (functional_units->_bitwise_operations->is_available()) {
                functional_units->_bitwise_operations->use_functional_unit();
                return true;
            } break;
        }
        case FPSPADDER : {
            if (functional_units->_float_adder->is_available()) {
                functional_units->_float_adder->use_functional_unit();
                return true;
            } break;
        }
        case FPDPADDER : {
            if (functional_units->_double_adder->is_available()) {
                functional_units->_double_adder->use_functional_unit();
                return true;
            } break;
        }
        case FPSPMULTI : {
            if (functional_units->_float_multiplier->is_available()) {
                functional_units->_float_multiplier->use_functional_unit();
                return true;
            } break;
        }
        case FPSPDIVID : {
            if (functional_units->_float_divider->is_available()) {
                functional_units->_float_divider->use_functional_unit();
                return true;
            } break;
        }
        case FPDPMULTI : {
            if (functional_units->_double_multiplier->is_available()) {
                functional_units->_double_multiplier->use_functional_unit();
                return true;
            } break;
        }
        case FPDPDIVID : {
            if (functional_units->_double_divider->is_available()) {
                functional_units->_double_divider->use_functional_unit();
                return true;
            } break;
        }
        case COMPARE : { break; }
        case GETELEMENTPTR : { break; }
        case CONVERSION : { break; }
        case OTHERINST : { break; }
        case REGISTER : {
            if (functional_units->_bit_register->is_available()) {
                functional_units->_bit_register->use_functional_unit();
                return true;
            } break;
        }
        case COUNTER : { break; }
        default: { 
            // assert() 
            return false;
        }
    }
    return false;
}

void
HWInterface::clearFunctionalUnit(uint64_t unit) {
    switch(unit) {
        case INTADDER : { 
                functional_units->_integer_adder->clear_functional_unit();
                break;
            } 
        case INTMULTI : { 
                functional_units->_integer_multiplier->clear_functional_unit();
                break;
            }
        case INTSHIFTER : { 
                functional_units->_bit_shifter->clear_functional_unit();
                break;
            }
        case INTBITWISE : { 
                functional_units->_bitwise_operations->clear_functional_unit();
                break;
            }
        case FPSPADDER : { 
                functional_units->_float_adder->clear_functional_unit();
                break;
            }
        case FPDPADDER : { 
                functional_units->_double_adder->clear_functional_unit();
                break;
            }
        case FPSPMULTI : { 
                functional_units->_float_multiplier->clear_functional_unit();
                break;
            }
        case FPSPDIVID : { 
                functional_units->_float_divider->clear_functional_unit();
                break;
            }
        case FPDPMULTI : { 
                functional_units->_double_multiplier->clear_functional_unit();
                break;
            }
        case FPDPDIVID : { 
                functional_units->_double_divider->clear_functional_unit();
                break;
            }
        case COMPARE : { break; }
        case GETELEMENTPTR : { break; }
        case CONVERSION : { break; }
        case OTHERINST : { break; }
        case REGISTER : { 
                functional_units->_bit_register->clear_functional_unit();
                break;
            }
        case COUNTER : { break; }
        default: { 
            // assert() 
        }
    }
}