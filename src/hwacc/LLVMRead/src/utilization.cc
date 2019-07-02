#include "utilization.hh"
#include "debugFlags.hh"

Utilization::Utilization(int clock_period, RegisterList* List) {
    _Clock_Period = clock_period;
    regList = List;
    int transistorTime = clock_period/1000;
    totalPwr.cycleTime = transistorTime;
    regPwr.cycleTime = transistorTime;
    std::cout << "Clock Period: " << _Clock_Period << " Transistor Delay: " << transistorTime << std::endl;
    getRegisterPowerArea(transistorTime, &regPwr.internal_power, &regPwr.switch_power, &regPwr.leakage_power, &regPwr.area);
    getAdderPowerArea(transistorTime, &adderPwr.internal_power, &adderPwr.switch_power, &adderPwr.leakage_power, &adderPwr.area);
    getMultiplierPowerArea(transistorTime, &multiPwr.internal_power, &multiPwr.switch_power, &multiPwr.leakage_power, &multiPwr.area);
    getBitPowerArea(transistorTime, &bitPwr.internal_power, &bitPwr.switch_power, &bitPwr.leakage_power, &bitPwr.area);
    getShifterPowerArea(transistorTime, &shiftPwr.internal_power, &shiftPwr.switch_power, &shiftPwr.leakage_power, &shiftPwr.area);
    getSinglePrecisionFloatingPointAdderPowerArea(transistorTime, &spfpAddPwr.internal_power, &spfpAddPwr.switch_power, &spfpAddPwr.leakage_power, &spfpAddPwr.area);
    getDoublePrecisionFloatingPointAdderPowerArea(transistorTime, &dpfpAddPwr.internal_power, &dpfpAddPwr.switch_power, &dpfpAddPwr.leakage_power, &dpfpAddPwr.area);
    getSinglePrecisionFloatingPointMultiplierPowerArea(transistorTime, &spfpMulPwr.internal_power, &spfpMulPwr.switch_power, &spfpMulPwr.leakage_power, &spfpMulPwr.area);
    getDoublePrecisionFloatingPointMultiplierPowerArea(transistorTime, &dpfpMulPwr.internal_power, &dpfpMulPwr.switch_power, &dpfpMulPwr.leakage_power, &dpfpMulPwr.area);
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
Utilization::finalPowerUsage(FunctionalUnits units) {
        calculateFinalLeakagePowerUsage(units);
        regList->totalAccess(&regUsage);
        calculateArea(units);
        calculateRegisterPowerUsage(&regUsage);

}

void
Utilization::calculateFinalLeakagePowerUsage(FunctionalUnits units) {
    // --
    finalPwr.leakage_power = adderPwr.leakage_power*units.int_adder_units;
    finalPwr.leakage_power += multiPwr.leakage_power*units.int_multiply_units;
    finalPwr.leakage_power += bitPwr.leakage_power*units.int_bit_units;
    finalPwr.leakage_power += shiftPwr.leakage_power*units.int_shifter_units;
    finalPwr.leakage_power += spfpAddPwr.leakage_power*units.fp_sp_adder;
    finalPwr.leakage_power += dpfpAddPwr.leakage_power*units.fp_dp_adder;
   
   // Since leakage power is much greater for floating point multipliers, assume full reuse
    if (units.fp_sp_multiply > 0) finalPwr.leakage_power += spfpMulPwr.leakage_power; 
    if (units.fp_dp_multiply > 0) finalPwr.leakage_power += dpfpMulPwr.leakage_power;
   // finalPwr.leakage_power += spfpMulPwr.leakage_power*units.fp_sp_multiply;
   // finalPwr.leakage_power += dpfpMulPwr.leakage_power*units.fp_dp_multiply;
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
    totalPwr.dynamic_power += (dpfpAddPwr.switch_power+dpfpAddPwr.internal_power)*units.fp_dp_adder;
    /*
    if(units.fpDivision > 0) {
        totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*units.fpDivision;
        totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*units.fpDivision;
    }
    */
    totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*units.fp_sp_multiply;
    totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*units.fp_dp_multiply;
    totalPwr.dynamic_energy += (totalPwr.dynamic_power);
}


#define WORDSIZE 4.0

void 
Utilization::calculateRegisterPowerUsage(Reg_Usage *regUsage) {
    //float flop_leakage_power = regPwr.leakage_power*WORDSIZE*regList->count();
    totalPwr.readEnergy = ((float)regUsage->reads)*regList->count()*WORDSIZE*(regPwr.internal_power + regPwr.switch_power)*regPwr.cycleTime;
    totalPwr.writeEnergy = ((float)regUsage->writes)*regList->count()*WORDSIZE*(regPwr.internal_power + regPwr.switch_power)*regPwr.cycleTime;
    totalPwr.reg_leakage_power = regPwr.leakage_power*WORDSIZE*regList->count();
    totalPwr.reg_dynamic_energy = (totalPwr.readEnergy + totalPwr.writeEnergy);
    totalPwr.reg_area = regList->count()*WORDSIZE*regPwr.area;
}

void 
Utilization::calculateArea(FunctionalUnits units) {
    
    totalPwr.area = multiPwr.area*units.int_multiply_units;
    totalPwr.area += bitPwr.area*units.int_bit_units;
    totalPwr.area += shiftPwr.area*units.int_shifter_units;
    totalPwr.area += dpfpAddPwr.area*units.fp_dp_adder;
    //totalPwr.area += dpfpMulPwr.area*units.fp_dp_multiply;
    totalPwr.area += spfpAddPwr.area*units.fp_sp_adder;
    //totalPwr.area += spfpMulPwr.area*units.fp_sp_multiply;
    // Since area is much greater for floating point multipliers, assume full reuse
    if (units.fp_sp_multiply > 0) totalPwr.area += spfpMulPwr.area; 
    if (units.fp_dp_multiply > 0) totalPwr.area += dpfpMulPwr.area;
}
