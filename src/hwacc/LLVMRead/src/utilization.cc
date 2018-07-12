#include "utilization.hh"

Utilization::Utilization() {
    for(int i = 0; i < 4; i++) {
        intHardwareUnits[i] = 0;
        maxIntHardwareUnits[i] = 0;
    }
    for(int i = 0; i < 2; i++) {
        fpHardwareUnits[i] = 0;
        maxfpHardwareUnits[i] = 0;
    }
    getRegisterPowerArea(regPwr.cycleTime, &regPwr.internal_power, &regPwr.switch_power, &regPwr.leakage_power, &regPwr.area);
    pwrUnits.push_back(&regPwr);
    getAdderPowerArea(adderPwr.cycleTime, &adderPwr.internal_power, &adderPwr.switch_power, &adderPwr.leakage_power, &adderPwr.area);
    pwrUnits.push_back(&adderPwr);
    getMultiplierPowerArea(multiPwr.cycleTime, &multiPwr.internal_power, &multiPwr.switch_power, &multiPwr.leakage_power, &multiPwr.area);
    pwrUnits.push_back(&multiPwr);
    getBitPowerArea(bitPwr.cycleTime, &bitPwr.internal_power, &bitPwr.switch_power, &bitPwr.leakage_power, &bitPwr.area);
    pwrUnits.push_back(&bitPwr);
    getShifterPowerArea(shiftPwr.cycleTime, &shiftPwr.internal_power, &shiftPwr.switch_power, &shiftPwr.leakage_power, &shiftPwr.area);
    pwrUnits.push_back(&shiftPwr);
    getSinglePrecisionFloatingPointAdderPowerArea(spfpAddPwr.cycleTime, &spfpAddPwr.internal_power, &spfpAddPwr.switch_power, &spfpAddPwr.leakage_power, &regPwr.area);
    pwrUnits.push_back(&spfpAddPwr);
    getDoublePrecisionFloatingPointAdderPowerArea(dpfpAddPwr.cycleTime, &dpfpAddPwr.internal_power, &dpfpAddPwr.switch_power, &dpfpAddPwr.leakage_power, &regPwr.area);
    pwrUnits.push_back(&dpfpAddPwr);
    getSinglePrecisionFloatingPointMultiplierPowerArea(spfpMulPwr.cycleTime, &spfpMulPwr.internal_power, &spfpMulPwr.switch_power, &spfpMulPwr.leakage_power, &regPwr.area);
    pwrUnits.push_back(&spfpMulPwr);
    getDoublePrecisionFloatingPointMultiplierPowerArea(dpfpMulPwr.cycleTime, &dpfpMulPwr.internal_power, &dpfpMulPwr.switch_power, &dpfpMulPwr.leakage_power, &regPwr.area);
    pwrUnits.push_back(&dpfpMulPwr);
}

void 
Utilization::update(Instruction instr) {
     if(instr.general.binary){
        if(instr.binary.ty == "double") {
            floats[instr.general.opCode]++;
        } else if(instr.binary.ty == "float") {
            floats[instr.general.opCode]++;
        } else if(instr.binary.ty[0] == 'i') {
            integer[instr.general.opCode]++;
        } else {
            others[instr.general.opCode]++;
        }
     } else if(instr.general.bitwise){
         if(instr.general.opCode == "or" || instr.general.opCode == "and" || instr.general.opCode == "xor") {
            bitCount[instr.general.opCode]++; 
         }
         else shiftCount[instr.general.opCode]++;
     } else {
         opCount[instr.general.opCode]++;
     }
}


void
Utilization::maxUnits(int count, int unit, bool fp) {
    if(fp) {
        if(maxfpHardwareUnits[unit] < count) maxfpHardwareUnits[unit] = count;
    } else {
        if(maxIntHardwareUnits[unit] < count) maxIntHardwareUnits[unit] = count;
    }
}

void
Utilization::currUnits(int count, int unit, bool fp) {
    if(fp) fpHardwareUnits[unit] = count;
    else intHardwareUnits[unit] = count;
    maxUnits(count, unit, fp);
}

int
Utilization::totalUnits() {
    int total = 0;
    for(int i = 0; i < 4; i++) {
        total += intHardwareUnits[i];
    }
    for(int i = 0; i < 2; i++) {
        total += fpHardwareUnits[i];
    }
    total*=3;
    return total;
}

void 
Utilization::clearAll() {
    for(int i = 0; i < 4; i++) {
        intHardwareUnits[i] = 0;
        maxIntHardwareUnits[i] = 0;
    }
    for(int i = 0; i < 2; i++) {
        fpHardwareUnits[i] = 0;
        maxfpHardwareUnits[i] = 0;
    } 
}

void
Utilization::calculateLeakagePowerUsage() {
    // for(auto it = pwrUnits.begin(); it != pwrUnits.end(); ++it) 
    totalPwr.leakage_power += adderPwr.leakage_power*intHardwareUnits[ADDUNIT];
    totalPwr.leakage_power += multiPwr.leakage_power*intHardwareUnits[MULUNIT];
    totalPwr.leakage_power += bitPwr.leakage_power*intHardwareUnits[BITUNIT];
    totalPwr.leakage_power += shiftPwr.leakage_power*intHardwareUnits[SHIFTUNIT];
    totalPwr.leakage_power += spfpAddPwr.leakage_power*fpHardwareUnits[ADDUNIT];
    //totalPwr.leakage_power += dpfpAddPwr.leakage_power*fpHardwareUnits[ADDUNIT];
    totalPwr.leakage_power += spfpMulPwr.leakage_power*fpHardwareUnits[MULUNIT];
    //totalPwr.leakage_power += dpfpMulPwr.leakage_power*fpHardwareUnits[MULUNIT];
}

void 
Utilization::calculateDynamicPowerUsage() {
    // for(auto it = pwrUnits.begin(); it != pwrUnits.end(); ++it) 
    totalPwr.dynamic_power += (adderPwr.switch_power+adderPwr.internal_power)*intHardwareUnits[ADDUNIT];
    totalPwr.dynamic_power += (multiPwr.switch_power+multiPwr.internal_power)*intHardwareUnits[MULUNIT];
    totalPwr.dynamic_power += (bitPwr.switch_power+bitPwr.internal_power)*intHardwareUnits[BITUNIT];
    totalPwr.dynamic_power += (shiftPwr.switch_power+shiftPwr.internal_power)*intHardwareUnits[SHIFTUNIT];
    totalPwr.dynamic_power += (spfpAddPwr.switch_power+spfpAddPwr.internal_power)*fpHardwareUnits[ADDUNIT];
    //totalPwr.dynamic_power += (dpfpAddPwr.switch_power+dpfpAddPwr.internal_power)*fpHardwareUnits[ADDUNIT];
    totalPwr.dynamic_power += (spfpMulPwr.switch_power+spfpMulPwr.internal_power)*fpHardwareUnits[MULUNIT];
    //totalPwr.dynamic_power += (dpfpMulPwr.switch_power+dpfpMulPwr.internal_power)*fpHardwareUnits[MULUNIT];
}

void 
Utilization::calculateRegisterPowerUsage(int read, int write, int count, int wordSize) {
    float flop_leakage_power = regPwr.leakage_power*32*count;
    totalPwr.readEnergy += wordSize*8*(regPwr.internal_power + regPwr.switch_power)*read;
    totalPwr.writeEnergy += wordSize*8*(regPwr.internal_power + regPwr.switch_power)*write;
    totalPwr.reg_leakage_power += (count*8*regPwr.leakage_power + flop_leakage_power);
    totalPwr.reg_dynamic_energy = totalPwr.readEnergy + totalPwr.writeEnergy;
    totalPwr.area += count*8*regPwr.area;
    totalPwr.area += regPwr.area*32*count;
}

void 
Utilization::calculateArea() {
    totalPwr.area += adderPwr.area*maxIntHardwareUnits[ADDUNIT];
    totalPwr.area += multiPwr.area*maxIntHardwareUnits[MULUNIT];
    totalPwr.area += bitPwr.area*maxIntHardwareUnits[BITUNIT];
    totalPwr.area += shiftPwr.area*maxIntHardwareUnits[SHIFTUNIT];
    totalPwr.area += adderPwr.area*maxfpHardwareUnits[ADDUNIT];
    totalPwr.area += multiPwr.area*maxfpHardwareUnits[MULUNIT];
}