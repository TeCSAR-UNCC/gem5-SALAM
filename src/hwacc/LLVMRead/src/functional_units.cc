//------------------------------------------//
#include "functional_units.hh"
//------------------------------------------//


FunctionalUnit::FunctionalUnit(int Latency, uint8_t HardwareUnit) {
    hardwareUnit = HardwareUnit;
    powerAreaProfile.latency = Latency;
    cycle_count = 0;
    parse_count = 0;
    dynamic_count = 0;
    dynamic_max = 0;
    static_limit = 0;
    switch(HardwareUnit) {
        case COUNTER: {
                getCounterPowerArea(         Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        case INTADDER: {
                getAdderPowerArea(           Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        case INTMULTI: {
                getMultiplierPowerArea(      Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        case INTSHIFTER: {
                getShifterPowerArea(         Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        case INTBITWISE: {
                getBitPowerArea(             Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        case FPSPADDER: {
                getSinglePrecisionFloatingPointAdderPowerArea( Latency, 
                                                               &powerAreaProfile.internal_power, 
                                                               &powerAreaProfile.switch_power,
                                                               &powerAreaProfile.dynamic_power,
                                                               &powerAreaProfile.dynamic_energy,
                                                               &powerAreaProfile.leakage_power,
                                                               &powerAreaProfile.area);
            break;
        }
        case FPDPADDER: {
                getDoublePrecisionFloatingPointAdderPowerArea( Latency, 
                                                               &powerAreaProfile.internal_power, 
                                                               &powerAreaProfile.switch_power,
                                                               &powerAreaProfile.dynamic_power,
                                                               &powerAreaProfile.dynamic_energy,
                                                               &powerAreaProfile.leakage_power,
                                                               &powerAreaProfile.area);
            break;
        }
        case FPSPMULTI: {
                getSinglePrecisionFloatingPointMultiplierPowerArea( Latency, 
                                                                    &powerAreaProfile.internal_power, 
                                                                    &powerAreaProfile.switch_power,
                                                                    &powerAreaProfile.dynamic_power,
                                                                    &powerAreaProfile.dynamic_energy,
                                                                    &powerAreaProfile.leakage_power,
                                                                    &powerAreaProfile.area);
            break;
        }
        case FPDPMULTI: {
                getDoublePrecisionFloatingPointMultiplierPowerArea( Latency, 
                                                                    &powerAreaProfile.internal_power, 
                                                                    &powerAreaProfile.switch_power,
                                                                    &powerAreaProfile.dynamic_power,
                                                                    &powerAreaProfile.dynamic_energy,
                                                                    &powerAreaProfile.leakage_power,
                                                                    &powerAreaProfile.area);
            break;
        }
        case FPSPDIVID: {
                getSinglePrecisionFloatingPointMultiplierPowerArea( Latency, 
                                                                    &powerAreaProfile.internal_power, 
                                                                    &powerAreaProfile.switch_power,
                                                                    &powerAreaProfile.dynamic_power,
                                                                    &powerAreaProfile.dynamic_energy,
                                                                    &powerAreaProfile.leakage_power,
                                                                    &powerAreaProfile.area);
            break;
        }
        case FPDPDIVID: {
                getDoublePrecisionFloatingPointMultiplierPowerArea( Latency, 
                                                                    &powerAreaProfile.internal_power, 
                                                                    &powerAreaProfile.switch_power,
                                                                    &powerAreaProfile.dynamic_power,
                                                                    &powerAreaProfile.dynamic_energy,
                                                                    &powerAreaProfile.leakage_power,
                                                                    &powerAreaProfile.area);
            break;
        }
        case COMPARE: {
                getBitPowerArea(             Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        case GETELEMENTPTR: {

            break;
        }
        case CONVERSION: {
                getShifterPowerArea(         Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        case REGISTER: {
                getRegisterPowerArea(        Latency, 
                                             &powerAreaProfile.internal_power, 
                                             &powerAreaProfile.switch_power,
                                             &powerAreaProfile.dynamic_power,
                                             &powerAreaProfile.dynamic_energy,
                                             &powerAreaProfile.leakage_power,
                                             &powerAreaProfile.area);
            break;
        }
        default: {
                // Other
            break;
        }
    }
}

bool
FunctionalUnit::available() {
    if ((dynamic_count < static_limit) || (static_limit < 0)) {
        updateDynamic();
        return true;
    } 
    return false;
}

void
FunctionalUnit::setStatic(int Static) {
    if (Static == 0) static_limit = parse_count;
    else static_limit = Static;
}