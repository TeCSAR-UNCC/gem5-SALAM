//------------------------------------------//
#include "utilization.hh"
//------------------------------------------//

Utilization::Utilization(int clock_period, int fu_clock_period, RegisterList* List) {
    _Clock_Period = clock_period;
    regList = List;
    int transistorTime = fu_clock_period;
    totalPwr.cycleTime = transistorTime;
    regPwr.cycleTime = transistorTime;
 //   getRegisterPowerArea(transistorTime, &regPwr.internal_power, &regPwr.switch_power, &regPwr.leakage_power, &regPwr.area);
 //   getAdderPowerArea(transistorTime, &adderPwr.internal_power, &adderPwr.switch_power, &adderPwr.leakage_power, &adderPwr.area);
 //   getMultiplierPowerArea(transistorTime, &multiPwr.internal_power, &multiPwr.switch_power, &multiPwr.leakage_power, &multiPwr.area);
 //   getBitPowerArea(transistorTime, &bitPwr.internal_power, &bitPwr.switch_power, &bitPwr.leakage_power, &bitPwr.area);
 //   getShifterPowerArea(transistorTime, &shiftPwr.internal_power, &shiftPwr.switch_power, &shiftPwr.leakage_power, &shiftPwr.area);
 //   getSinglePrecisionFloatingPointAdderPowerArea(transistorTime, &spfpAddPwr.internal_power, &spfpAddPwr.switch_power, &spfpAddPwr.leakage_power, &spfpAddPwr.area);
 //  getDoublePrecisionFloatingPointAdderPowerArea(transistorTime, &dpfpAddPwr.internal_power, &dpfpAddPwr.switch_power, &dpfpAddPwr.leakage_power, &dpfpAddPwr.area);
 //   getSinglePrecisionFloatingPointMultiplierPowerArea(transistorTime, &spfpMulPwr.internal_power, &spfpMulPwr.switch_power, &spfpMulPwr.leakage_power, &spfpMulPwr.area);
 //   getDoublePrecisionFloatingPointMultiplierPowerArea(transistorTime, &dpfpMulPwr.internal_power, &dpfpMulPwr.switch_power, &dpfpMulPwr.leakage_power, &dpfpMulPwr.area);
}

Hardware::Hardware(int Latency) {
    DPRINTF(Hardware, "Initializing Hardware Resources \n");
    counter = new FunctionalUnit(Latency, COUNTER);
    int_adder = new FunctionalUnit(Latency, INTADDER);
    int_multiplier = new FunctionalUnit(Latency, INTMULTI);
    int_shifter = new FunctionalUnit(Latency, INTSHIFTER);
    int_bitwise = new FunctionalUnit(Latency, INTBITWISE);
    fp_sp_adder = new FunctionalUnit(Latency, FPSPADDER);
    fp_dp_adder = new FunctionalUnit(Latency, FPDPADDER);
    fp_sp_multiplier = new FunctionalUnit(Latency, FPSPMULTI);
    fp_sp_division = new FunctionalUnit(Latency, FPDPDIVID);
    fp_dp_multiplier = new FunctionalUnit(Latency, FPDPMULTI);
    fp_dp_division = new FunctionalUnit(Latency, FPDPDIVID);
    comparison = new FunctionalUnit(Latency, COMPARE);
    getelementptr = new FunctionalUnit(Latency, GETELEMENTPTR);
    conversion = new FunctionalUnit(Latency, CONVERSION);
    registers = new FunctionalUnit(Latency, REGISTER);
    other = new FunctionalUnit(Latency, OTHER);
    loads = 0; stores = 0; control_flow = 0;
}

void
Hardware::update(){
    updateMax();
    // Power
    // Registers
    reset();
}

void
Hardware::updateParsed(uint8_t HardwareUnit) {
     switch(HardwareUnit) {
        case COUNTER: counter->updateParse(); break;
        case INTADDER: int_adder->updateParse(); break;
        case INTMULTI: int_multiplier->updateParse(); break;
        case INTSHIFTER: int_shifter->updateParse(); break;
        case INTBITWISE: int_bitwise->updateParse(); break;
        case FPSPADDER: fp_sp_adder ->updateParse(); break;
        case FPDPADDER: fp_dp_adder->updateParse(); break;
        case FPSPMULTI: fp_sp_multiplier->updateParse(); break;
        case FPSPDIVID: fp_sp_division->updateParse(); break;
        case FPDPMULTI: fp_dp_multiplier->updateParse(); break;
        case FPDPDIVID: fp_dp_division->updateParse(); break;
        case COMPARE: comparison->updateParse(); break;
        case GETELEMENTPTR: getelementptr->updateParse(); break;
        case CONVERSION: conversion->updateParse(); break;
        default: other->updateParse(); break;
     }
}

void
Hardware::updateDynamic(uint8_t HardwareUnit) {
     switch(HardwareUnit) {
        case COUNTER: counter->updateDynamic(); break;
        case INTADDER: int_adder->updateDynamic(); break;
        case INTMULTI: int_multiplier->updateDynamic(); break;
        case INTSHIFTER: int_shifter->updateDynamic(); break;
        case INTBITWISE: int_bitwise->updateDynamic(); break;
        case FPSPADDER: fp_sp_adder ->updateDynamic(); break;
        case FPDPADDER: fp_dp_adder->updateDynamic(); break;
        case FPSPMULTI: fp_sp_multiplier->updateDynamic(); break;
        case FPSPDIVID: fp_sp_division->updateDynamic(); break;
        case FPDPMULTI: fp_dp_multiplier->updateDynamic(); break;
        case FPDPDIVID: fp_dp_division->updateDynamic(); break;
        case COMPARE: comparison->updateDynamic(); break;
        case GETELEMENTPTR: getelementptr->updateDynamic(); break;
        case CONVERSION: conversion->updateDynamic(); break;
        default: other->updateDynamic(); break;
     }
}

bool
Hardware::available(uint8_t HardwareUnit) {
     switch(HardwareUnit) {
        case COUNTER: return counter->available(); break;
        case INTADDER: return int_adder->available(); break;
        case INTMULTI: return int_multiplier->available(); break;
        case INTSHIFTER: return int_shifter->available(); break;
        case INTBITWISE: return int_bitwise->available(); break;
        case FPSPADDER: return fp_sp_adder ->available(); break;
        case FPDPADDER: return fp_dp_adder->available(); break;
        case FPSPMULTI: return fp_sp_multiplier->available(); break;
        case FPSPDIVID: return fp_sp_division->available(); break;
        case FPDPMULTI: return fp_dp_multiplier->available(); break;
        case FPDPDIVID: return fp_dp_division->available(); break;
        case COMPARE: return comparison->available(); break;
        case GETELEMENTPTR: return getelementptr->available(); break;
        case CONVERSION: return conversion->available(); break;
        default: return true; break;
    }
    return true;
}

void
Hardware::updateMax() {
    counter->maxDynamic();
    int_adder->maxDynamic();
    int_multiplier->maxDynamic();
    int_shifter->maxDynamic();
    int_bitwise->maxDynamic();
    fp_sp_adder->maxDynamic();
    fp_dp_adder->maxDynamic();
    fp_sp_multiplier->maxDynamic();
    fp_sp_division->maxDynamic();
    fp_dp_multiplier->maxDynamic();
    fp_dp_division->maxDynamic();
    comparison->maxDynamic();
    getelementptr->maxDynamic();
    conversion->maxDynamic();
}

void
Hardware::reset() {
    counter->reset();
    int_adder->reset();
    int_multiplier->reset();
    int_shifter->reset();
    int_bitwise->reset();
    fp_sp_adder->reset();
    fp_dp_adder->reset();
    fp_sp_multiplier->reset();
    fp_sp_division->reset();
    fp_dp_multiplier->reset();
    fp_dp_division->reset();
    comparison->reset();
    getelementptr->reset();
    conversion->reset();
}

void
Hardware::updateLimit(int Counter, 
                            int IntAdder, 
                            int IntMul, 
                            int IntShift, 
                            int IntBit, 
                            int FPSPAdd, 
                            int FPDPAdd, 
                            int FPSPMul, 
                            int FPSPDiv, 
                            int FPDPMul, 
                            int FPDPDiv, 
                            int Compare, 
                            int GEP, 
                            int Conversion) {
    counter->setStatic(Counter);
    int_adder->setStatic(IntAdder);
    int_multiplier->setStatic(IntMul);
    int_shifter->setStatic(IntShift);
    int_bitwise->setStatic(IntBit);
    fp_sp_adder->setStatic(FPSPAdd);
    fp_dp_adder->setStatic(FPDPAdd);
    fp_sp_multiplier->setStatic(FPSPMul);
    fp_sp_division->setStatic(FPSPDiv);
    fp_dp_multiplier->setStatic(FPDPMul);
    fp_dp_division->setStatic(FPDPDiv);
    comparison->setStatic(Compare);
    getelementptr->setStatic(GEP);
    conversion->setStatic(Conversion);
}

void
Hardware::pipelined() {
    fp_sp_adder->multistaged();
    fp_dp_adder->multistaged();
    fp_sp_multiplier->multistaged();
    fp_sp_division->multistaged();
    fp_dp_multiplier->multistaged();
    fp_dp_division->multistaged();
}

void
Hardware::printResults() {
    printOccupancyResults();
    printLLVMFunctionalUnits();
    printDefinedFunctionalUnits();
    printDynamicFunctionalUnits();
}

void
Hardware::printTotals() {
    /*
    std::cout << "   SPM Leakage Power:               " << spm_leakage << " mW" << std::endl;
    std::cout << "   SPM Optimized Leakage Power:     " << spm_opt_leakage << " mW" << std::endl;
    std::cout << "   SPM Read Dynamic Power:          " << spm_read_dynamic << " mW" << std::endl;
    std::cout << "   SPM Write Dynamic Power:         " << spm_write_dynamic << " mW" << std::endl;
    std::cout << "   SPM Total Power:                 " << spm_leakage+spm_read_dynamic+spm_write_dynamic << " mW" << std::endl;
    std::cout << "   Total Accelerator Power:         " << total_power+spm_leakage+spm_read_dynamic+spm_write_dynamic << " mW" << std::endl;
    std::cout << std::endl;
    std::cout << "   ========= Area Analysis ===================" << std::endl;
    std::cout << "   SPM Area:                        " << spm_area/1000 << " um^2" << std::endl;
    std::cout << "   Total Accelerator Area:          " << total_area + spm_area/1000 << " um^2 (" << total_area/1000000 << " mm^2)" << std::endl;
    std::cout << std::endl;
    */
}



void
Hardware::printOccupancyResults() {
    std::cout << "   ========= Stalled Cycle Breakdown ==========" << std::endl;    
    std::cout << "       Load Only:                   " << occ_stalled.loadOnly << " cycles" << std::endl;
    std::cout << "       Store Only:                  " << occ_stalled.storeOnly << " cycles" << std::endl;
    std::cout << "       Compute Only:                " << occ_stalled.compOnly << " cycles" << std::endl;
    std::cout << "       Compute & Store:             " << occ_stalled.storeComp << " cycles" << std::endl;
    std::cout << "       Load & Store:                " << occ_stalled.loadStore << " cycles" << std::endl;
    std::cout << "       Load & Compute:              " << occ_stalled.loadComp << " cycles" << std::endl;
    std::cout << "       Load & Compute & Store:      " << occ_stalled.loadStoreComp << " cycles" << std::endl;
    std::cout << "   ========= Execution Cycle Breakdown ========" << std::endl;    
    std::cout << "       Load Only:                   " << occ_scheduled.loadOnly << " cycles" << std::endl;
    std::cout << "       Store Only:                  " << occ_scheduled.storeOnly << " cycles" << std::endl;
    std::cout << "       Compute Only:                " << occ_scheduled.compOnly << " cycles" << std::endl;
    std::cout << "       Compute & Store:             " << occ_scheduled.storeComp << " cycles" << std::endl;
    std::cout << "       Load & Store:                " << occ_scheduled.loadStore << " cycles" << std::endl;
    std::cout << "       Load & Compute:              " << occ_scheduled.loadComp << " cycles" << std::endl;
    std::cout << "       Load & Compute & Store:      " << occ_scheduled.loadStoreComp << " cycles" << std::endl;
    std::cout << std::endl;
}

void 
Hardware::printPowerAnalysis() {
    /*
    std::cout << "   ========= Power Analysis ==================" << std::endl;
    std::cout << "   FU Leakage Power:                " << fu_leakage << " mW " << std::endl;
    std::cout << "   FU Dynamic Power:                " << fu_dynamic << " mW " << std::endl;
    std::cout << "   FU Total Power:                  " << fu_total_power << " mW " << std::endl;
    */
}

void 
Hardware::printAreaAnalysis() {
    /*
    std::cout << "   FU Area:                         " << fu_area << " um^2 (" << fu_area/1000000 << " mm^2)" << std::endl;
    */
}

void 
Hardware::printRegisterUsageAnalysis() {
    /*
    std::cout << "   ========= Pipeline Register Usage =========" << std::endl;
    std::cout << "   Total Number of Registers:       " << reg_total << std::endl;
    std::cout << "   Max Register Usage Per Cycle:    " << reg_max_usage << std::endl;
    std::cout << "   Avg Register Usage Per Cycle:    " << reg_avg_usage << std::endl;
    std::cout << "   Avg Register Size Per Cycle:     " << reg_avg_size << std::endl;
    std::cout << "       Total Register Reads:        " << reg_reads << std::endl;
    std::cout << "       Total Register Writes:       " << reg_writes << std::endl;
    std::cout << std::endl;
    */
}

void 
Hardware::printRegisterPowerAnalysis() {
    /*
    std::cout << "   Pipeline Register Leakage Power: " << reg_leakage  << " mW " << std::endl;
    std::cout << "   Pipeline Register Dynamic Power: " << reg_dynamic << " mW " << std::endl;
    std::cout << "   Pipeline Register Total Power:   " << reg_total_power <<  " mW" << std::endl;
    */
}

void 
Hardware::printRegisterAreaAnalysis() {
    /*
    std::cout << "   Register Area:                   " << reg_area << " um^2 (" << reg_area/1000000 << " mm^2)" << std::endl;
    */
}   

void
Hardware::printLLVMFunctionalUnits(){
    std::cout << "   ========= LLVM Parsed FU's =================" << std::endl;
    std::cout << "   Counter FU's:                    " << counter->getParsedLimit() << std::endl;
    std::cout << "   Integer Add/Sub FU's:            " << int_adder->getParsedLimit() << std::endl;
    std::cout << "   Integer Mul/Div FU's:            " << int_multiplier->getParsedLimit() << std::endl;
    std::cout << "   Integer Shifter FU's:            " << int_shifter->getParsedLimit() << std::endl;
    std::cout << "   Integer Bitwise FU's:            " << int_bitwise->getParsedLimit() << std::endl;
    std::cout << "   Floating Point Float Add/Sub:    " << fp_sp_adder->getParsedLimit() << std::endl;
    std::cout << "   Floating Point Double Add/Sub:   " << fp_dp_adder->getParsedLimit() << std::endl;
    std::cout << "   Floating Point Float Mul:        " << fp_sp_multiplier->getParsedLimit() << std::endl;
    std::cout << "   Floating Point Float Div:        " << fp_sp_division->getParsedLimit() << std::endl;
    std::cout << "   Floating Point Double Mul:       " << fp_dp_multiplier->getParsedLimit() << std::endl;
    std::cout << "   Floating Point Double Div:       " << fp_dp_division->getParsedLimit() << std::endl;
    std::cout << "   Compare FU's:                    " << comparison->getParsedLimit() << std::endl;
    std::cout << "   GEP Instruction FU's:            " << getelementptr->getParsedLimit() << std::endl;
    std::cout << "   Type Conversion FU's:            " << conversion->getParsedLimit() << std::endl;
    std::cout << "   Other FU's:                      " << other->getParsedLimit() << std::endl;
    std::cout << std::endl;
}

void
Hardware::printDefinedFunctionalUnits(){
    std::cout << "   ========= Statically Limited FU's ==========" << std::endl;
    std::cout << "   Counter FU's:                    " << counter->getStaticLimit() << std::endl;
    std::cout << "   Integer Add/Sub FU's:            " << int_adder->getStaticLimit() << std::endl;
    std::cout << "   Integer Mul/Div FU's:            " << int_multiplier->getStaticLimit() << std::endl;
    std::cout << "   Integer Shifter FU's:            " << int_shifter->getStaticLimit() << std::endl;
    std::cout << "   Integer Bitwise FU's:            " << int_bitwise->getStaticLimit() << std::endl;
    std::cout << "   Floating Point Float Add/Sub:    " << fp_sp_adder->getStaticLimit() << std::endl;
    std::cout << "   Floating Point Double Add/Sub:   " << fp_dp_adder->getStaticLimit() << std::endl;
    std::cout << "   Floating Point Float Mul:        " << fp_sp_multiplier->getStaticLimit() << std::endl;
    std::cout << "   Floating Point Float Div:        " << fp_sp_division->getStaticLimit() << std::endl;
    std::cout << "   Floating Point Double Mul:       " << fp_dp_multiplier->getStaticLimit() << std::endl;
    std::cout << "   Floating Point Double Div:       " << fp_dp_division->getStaticLimit() << std::endl;
    std::cout << "   Compare FU's:                    " << comparison->getStaticLimit() << std::endl;
    std::cout << "   GEP Instruction FU's:            " << getelementptr->getStaticLimit() << std::endl;
    std::cout << "   Type Conversion FU's:            " << conversion->getStaticLimit() << std::endl;
    std::cout << "   Other FU's:                      " << other->getStaticLimit() << std::endl;
    std::cout << std::endl;
}

void
Hardware::printDynamicFunctionalUnits(){
    std::cout << "   ========= Runtime Allocated FU's ===========" << std::endl;
    std::cout << "   Counter FU's:                    " << counter->getDynamicMax() << std::endl;
    std::cout << "   Integer Add/Sub FU's:            " << int_adder->getDynamicMax() << std::endl;
    std::cout << "   Integer Mul/Div FU's:            " << int_multiplier->getDynamicMax() << std::endl;
    std::cout << "   Integer Shifter FU's:            " << int_shifter->getDynamicMax() << std::endl;
    std::cout << "   Integer Bitwise FU's:            " << int_bitwise->getDynamicMax() << std::endl;
    std::cout << "   Floating Point Float Add/Sub:    " << fp_sp_adder->getDynamicMax() << std::endl;
    std::cout << "   Floating Point Double Add/Sub:   " << fp_dp_adder->getDynamicMax() << std::endl;
    std::cout << "   Floating Point Float Mul:        " << fp_sp_multiplier->getDynamicMax() << std::endl;
    std::cout << "   Floating Point Float Div:        " << fp_sp_division->getDynamicMax() << std::endl;
    std::cout << "   Floating Point Double Mul:       " << fp_dp_multiplier->getDynamicMax() << std::endl;
    std::cout << "   Floating Point Double Div:       " << fp_dp_division->getDynamicMax() << std::endl;
    std::cout << "   Compare FU's:                    " << comparison->getDynamicMax() << std::endl;
    std::cout << "   GEP Instruction FU's:            " << getelementptr->getDynamicMax() << std::endl;
    std::cout << "   Type Conversion FU's:            " << conversion->getDynamicMax() << std::endl;
    std::cout << "   Other FU's:                      " << other->getDynamicMax() << std::endl;
    std::cout << std::endl;
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
Utilization::finalPowerUsage(FunctionalUnits units, int cycle) {
        calculateFinalLeakagePowerUsage(units);
        regList->totalAccess(&regUsage);
        calculateArea(units);
         calculateRegisterPowerUsage(&regUsage, cycle);
}

uca_org_t
Utilization::getCactiResults(int cache_size, int word_size, int ports, int cache_type) {
    return cactiWrapper((cache_size), word_size, ports, cache_type);
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
    totalPwr.dynamic_power = (adderPwr.switch_power+adderPwr.internal_power)*units.int_adder_units ;
    totalPwr.dynamic_power += (multiPwr.switch_power+multiPwr.internal_power)*units.int_multiply_units;
    totalPwr.dynamic_power += (bitPwr.switch_power+bitPwr.internal_power)*units.int_bit_units;
    totalPwr.dynamic_power += (shiftPwr.switch_power+shiftPwr.internal_power)*units.int_shifter_units;
    totalPwr.dynamic_power += (spfpAddPwr.switch_power+spfpAddPwr.internal_power)*units.fp_sp_adder*5;
    totalPwr.dynamic_power += (dpfpAddPwr.switch_power+dpfpAddPwr.internal_power)*units.fp_dp_adder*5;
    totalPwr.dynamic_power += (ADD_0_5ns_int_power+ADD_0_5ns_switch_power)*(units.counter_units+units.conversion+units.compare+units.gep);
    /*
    if(units.fpDivision > 0) {
        totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*units.fpDivision;
        totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*units.fpDivision;
    }
    */
    if (units.fp_sp_multiply > 0) totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*5;
    if (units.fp_dp_multiply > 0) totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*5;
    // totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*units.fp_sp_multiply;
    // totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*units.fp_dp_multiply;
    totalPwr.dynamic_energy += (totalPwr.dynamic_power);
}


void 
Utilization::calculateRegisterPowerUsage(Reg_Usage *regUsage, int cycle) {
    totalPwr.readEnergy = ((float)regUsage->reads)*((regList->average())/cycle)*(regList->avgSize()/(regList->average()))*(regPwr.internal_power + regPwr.switch_power);
    totalPwr.writeEnergy = ((float)regUsage->writes)*((regList->average())/cycle)*(regList->avgSize()/(regList->average()))*(regPwr.internal_power + regPwr.switch_power);
    totalPwr.reg_leakage_power = regPwr.leakage_power*((regList->average())/cycle)*(regList->avgSize()/(regList->average()));
    totalPwr.reg_dynamic_energy = (totalPwr.readEnergy + totalPwr.writeEnergy);
    totalPwr.reg_area = ((regList->average())/cycle)*(regList->avgSize()/(regList->average()))*regPwr.area;
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
