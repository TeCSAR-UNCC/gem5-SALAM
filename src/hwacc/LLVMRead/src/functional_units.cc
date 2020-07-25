//------------------------------------------//
#include "functional_units.hh"
//------------------------------------------//


FunctionalUnit::FunctionalUnit(int Latency, uint8_t HardwareUnit, uint8_t Stages, std::string Name) {
    hardwareUnit = HardwareUnit;
    powerAreaProfile.latency = Latency;
    powerAreaProfile.usage = 0;
    cycle_count = 0;
    parse_count = 0;
    dynamic_count = 0;
    dynamic_max = 0;
    static_limit = 0;
    name = Name;
    stages = Stages;
    powerAreaTotals.latency = Latency;
    powerAreaTotals.internal_power = 0;
    powerAreaTotals.switch_power = 0;
    powerAreaTotals.dynamic_power = 0;
    powerAreaTotals.dynamic_energy = 0;
    powerAreaTotals.leakage_power = 0;
    powerAreaTotals.area = 0;
    powerAreaTotals.usage = 0;
    powerAreaReport.latency = Latency;
    powerAreaReport.internal_power = 0;
    powerAreaReport.switch_power = 0;
    powerAreaReport.dynamic_power = 0;
    powerAreaReport.dynamic_energy = 0;
    powerAreaReport.leakage_power = 0;
    powerAreaReport.area = 0;
    powerAreaReport.usage = 0;
    /////////////////////////////////
    if (stages > 1) multistage = true;
    else multistage = false;
    /////////////////////////////////
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
                                             powerAreaProfile.internal_power = 0;
                                             powerAreaProfile.switch_power = 0;
                                             powerAreaProfile.dynamic_power = 0;
                                             powerAreaProfile.dynamic_energy = 0;
                                             powerAreaProfile.leakage_power = 0;
                                             powerAreaProfile.area = 0;
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
                    for (int i = 0; i < 2; i++) {
                        registerPowerAreaReport.push_back(powerAreaReport);
                        registerPowerAreaTotals.push_back(powerAreaTotals);
                    }
            break;
        }
        default: {
                // Other
                                             powerAreaProfile.internal_power = 0;
                                             powerAreaProfile.switch_power = 0;
                                             powerAreaProfile.dynamic_power = 0;
                                             powerAreaProfile.dynamic_energy = 0;
                                             powerAreaProfile.leakage_power = 0;
                                             powerAreaProfile.area = 0;
            break;
        }
    }
    /////////////////////////////////

    for ( int i = 0; i < stages; i++) {
        occupied.push_back(false);
        if (multistage) {
            multistagePowerAreaReport.push_back(powerAreaReport);
            multistagePowerAreaTotals.push_back(powerAreaTotals);
        }
    }
}

bool
FunctionalUnit::available() {
    return !(occupied.front());
}

int
FunctionalUnit::setStatic(int Static) {
    if (Static <= 0) static_limit = parse_count;
    else static_limit = Static;
    return static_limit;
}

void
FunctionalUnit::powerUpdate() {
    if(multistage) {
        for (int i = 0; i < occupied.size(); i++) {
            if (occupied.at(i)) {
                multistagePowerAreaReport.at(i).usage++;
                multistagePowerAreaReport.at(i).internal_power += powerAreaProfile.internal_power;
                multistagePowerAreaReport.at(i).switch_power += powerAreaProfile.switch_power;
                multistagePowerAreaReport.at(i).dynamic_power += powerAreaProfile.dynamic_power;
                multistagePowerAreaReport.at(i).dynamic_energy += powerAreaProfile.dynamic_energy;
            } else {
                multistagePowerAreaReport.at(i).leakage_power += powerAreaProfile.leakage_power;
            }
        }
    } else {
       if (occupied.front()) {
            powerAreaReport.usage++;
            powerAreaReport.internal_power += powerAreaProfile.internal_power;
            powerAreaReport.switch_power += powerAreaProfile.switch_power;
            powerAreaReport.dynamic_power += powerAreaProfile.dynamic_power;
            powerAreaReport.dynamic_energy += powerAreaProfile.dynamic_energy;
        } else {
            powerAreaReport.leakage_power += powerAreaProfile.leakage_power;
        } 
    }
}

void 
FunctionalUnit::setStage(int Stage, bool Active) {
    occupied.at(Stage) = Active;
}

void
FunctionalUnit::resetStages() {
    for (int i = 0; i < occupied.size(); i++ ) occupied.at(i) = false;
} 

void
FunctionalUnit::resetRuntime() {
    dynamic_count = 0;
}

void 
FunctionalUnit::printHardwareStats() {

    std::cout << name << " Unit[" << ID << "] | " << "Limit: " << static_limit << " | Stages: " << stages << "\n";
}