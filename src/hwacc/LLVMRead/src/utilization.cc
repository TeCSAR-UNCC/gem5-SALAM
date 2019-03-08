#include "utilization.hh"
#include "debugFlags.hh"

Utilization::Utilization(int clock_period) {
    _Clock_Period = clock_period;
    int transistorTime = clock_period/1000;
    totalPwr.cycleTime = transistorTime;
    getRegisterPowerArea(transistorTime, &regPwr.internal_power, &regPwr.switch_power, &regPwr.leakage_power, &regPwr.area);
    getAdderPowerArea(transistorTime, &adderPwr.internal_power, &adderPwr.switch_power, &adderPwr.leakage_power, &adderPwr.area);
    getMultiplierPowerArea(transistorTime, &multiPwr.internal_power, &multiPwr.switch_power, &multiPwr.leakage_power, &multiPwr.area);
    getBitPowerArea(transistorTime, &bitPwr.internal_power, &bitPwr.switch_power, &bitPwr.leakage_power, &bitPwr.area);
    getShifterPowerArea(transistorTime, &shiftPwr.internal_power, &shiftPwr.switch_power, &shiftPwr.leakage_power, &shiftPwr.area);
    getSinglePrecisionFloatingPointAdderPowerArea(transistorTime, &spfpAddPwr.internal_power, &spfpAddPwr.switch_power, &spfpAddPwr.leakage_power, &regPwr.area);
    getDoublePrecisionFloatingPointAdderPowerArea(transistorTime, &dpfpAddPwr.internal_power, &dpfpAddPwr.switch_power, &dpfpAddPwr.leakage_power, &regPwr.area);
    getSinglePrecisionFloatingPointMultiplierPowerArea(transistorTime, &spfpMulPwr.internal_power, &spfpMulPwr.switch_power, &spfpMulPwr.leakage_power, &regPwr.area);
    getDoublePrecisionFloatingPointMultiplierPowerArea(transistorTime, &dpfpMulPwr.internal_power, &dpfpMulPwr.switch_power, &dpfpMulPwr.leakage_power, &regPwr.area);
}

void 
Utilization::updatePowerConsumption(FunctionalUnits units) {
/*
    printf("%s %s %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s %3d %s",
    "   Power",
    "\n*******************************************************************************",
    "\n   Counters:", units.counter_units,
    "   Adder: ", units.int_adder_units,
    "   Mul/Div: ", units.int_multiply_units,
    "   Shift: ", units.int_shifter_units,
    "   Bit: ", units.int_bit_units,
    "\n   SP Adder: ", units.fp_sp_adder,
    "   SP Mul/Div: ", units.fp_sp_multiply,
    "   DP Adder: ", units.fp_dp_adder,
    "   DP Mul/Div: ", units.fp_dp_multiply,
    "\n   Compare: ",units.compare,
    "   GEP: ", units.gep,
    "   Conversion: ", units.conversion,
    "   Other: ", units.other,
    "\n*******************************************************************************\n"
    );
*/
    calculateLeakagePowerUsage(units);
    calculateDynamicPowerUsage(units);
}


void
Utilization::calculateLeakagePowerUsage(FunctionalUnits units) {
    // --
    totalPwr.leakage_power += adderPwr.leakage_power*units.int_adder_units;
    totalPwr.leakage_power += multiPwr.leakage_power*units.int_multiply_units;
    totalPwr.leakage_power += bitPwr.leakage_power*units.int_bit_units;
    totalPwr.leakage_power += shiftPwr.leakage_power*units.int_shifter_units;
    totalPwr.leakage_power += spfpAddPwr.leakage_power*units.fp_sp_adder;
    totalPwr.leakage_power += dpfpAddPwr.leakage_power*units.fp_dp_adder;
    totalPwr.leakage_power += spfpMulPwr.leakage_power*units.fp_sp_multiply;
    totalPwr.leakage_power += dpfpMulPwr.leakage_power*units.fp_dp_multiply;
}

void 
Utilization::calculateDynamicPowerUsage(FunctionalUnits units) {
    // for(auto it = pwrUnits.begin(); it != pwrUnits.end(); ++it) 
    // --
    totalPwr.dynamic_power = (adderPwr.switch_power+adderPwr.internal_power)*units.int_adder_units;
    totalPwr.dynamic_power += (multiPwr.switch_power+multiPwr.internal_power)*units.int_multiply_units;
    totalPwr.dynamic_power += (bitPwr.switch_power+bitPwr.internal_power)*units.int_bit_units;
    totalPwr.dynamic_power += (shiftPwr.switch_power+shiftPwr.internal_power)*units.int_shifter_units;
    totalPwr.dynamic_power += (spfpAddPwr.switch_power+spfpAddPwr.internal_power)*units.fp_sp_adder;
    totalPwr.dynamic_power += (dpfpAddPwr.switch_power+dpfpAddPwr.internal_power)*units.fp_sp_adder;
    /*
    if(units.fpDivision > 0) {
        totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*units.fpDivision;
        totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*units.fpDivision;
    }
    */
    totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*units.fp_sp_multiply;
    totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*units.fp_dp_multiply;
    totalPwr.dynamic_energy += (totalPwr.dynamic_power*totalPwr.cycleTime);
}

/*
void 
Utilization::calculateRegisterPowerUsage(int read, int write, int count, int wordSize) {
    float flop_leakage_power = regPwr.leakage_power*wordSize*count;
    totalPwr.readEnergy = read*wordSize*(regPwr.internal_power + regPwr.switch_power)*regPwr.cycleTime;
    totalPwr.writeEnergy = write*wordSize*(regPwr.internal_power + regPwr.switch_power)*regPwr.cycleTime;
    totalPwr.reg_leakage_power += (count*wordSize*regPwr.leakage_power + flop_leakage_power);
    totalPwr.reg_dynamic_energy += (totalPwr.readEnergy + totalPwr.writeEnergy);
    //totalPwr.area += count*wordSize*regPwr.area;
     totalPwr.area += 12*wordSize*regPwr.area;
}

void 
Utilization::calculateArea() {
    totalPwr.area += adderPwr.area*maxIntHardwareUnits[ADDUNIT];
    totalPwr.area += multiPwr.area*maxIntHardwareUnits[MULUNIT];
    totalPwr.area += bitPwr.area*maxIntHardwareUnits[BITUNIT];
    totalPwr.area += shiftPwr.area*maxIntHardwareUnits[SHIFTUNIT];
    totalPwr.area += dpfpAddPwr.area*maxfpHardwareUnits[ADDUNIT];
    totalPwr.area += dpfpMulPwr.area*maxfpHardwareUnits[MULUNIT];
}
*/