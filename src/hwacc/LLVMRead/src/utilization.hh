#ifndef __UTILIZATION_HH__
#define __UTILIZATION_HH__
#include "debugFlags.hh" 
#include "debug/Hardware.hh"
#include "power_func.hh"
#include <map>
#include <vector>

struct PowerUsage {
    float cycleTime = 0;
    float internal_power = 0;
    float switch_power = 0;
    float leakage_power = 0;
    float area = 0;
};

struct PowerTotals {
    float cycleTime = 0;
    float reg_leakage_power = 0;
    float reg_dynamic_energy = 0;
    float leakage_power = 0;
    float dynamic_power = 0;
    float dynamic_energy = 0;
    float readEnergy = 0;
    float writeEnergy = 0;
    float area = 0;
};

class Utilization {
    private:
     float internal_power;
     float switch_power;
     float leakage_power;
     float area;
     float max_leakage_power;
     int intHardwareUnits[4];
     int fpHardwareUnits[2];
     int maxIntHardwareUnits[4];
     int maxfpHardwareUnits[2];
     std::vector<PowerUsage*> pwrUnits;

     public:
      typedef std::map<std::string, int> Floats;
      typedef std::map<std::string, int> Integer;
      typedef std::map<std::string, int> Others;
      typedef std::map<std::string, int> opCodeCount;
      typedef std::map<std::string, int> BitCount;
      typedef std::map<std::string, int> ShiftCount;

      PowerUsage regPwr;
      PowerUsage adderPwr;
      PowerUsage multiPwr;
      PowerUsage bitPwr;
      PowerUsage shiftPwr;
      PowerUsage spfpAddPwr;
      PowerUsage dpfpAddPwr;
      PowerUsage spfpMulPwr;
      PowerUsage dpfpMulPwr;
      PowerTotals totalPwr;
    
     Utilization(int clock_period); 
     void maxUnits(int count, int unit, bool fp);
     void currUnits(int count, int unit, bool fp);
     int fpMaxMul() { return maxfpHardwareUnits[MULUNIT]; }
     int fpMaxAdd() { return maxfpHardwareUnits[ADDUNIT]; }
     int intMaxMul() { return maxIntHardwareUnits[MULUNIT]; }
     int intMaxAdd() { return maxIntHardwareUnits[ADDUNIT]; }
     int maxShift() { return maxIntHardwareUnits[SHIFTUNIT]; }
     int maxBit() { return maxIntHardwareUnits[BITUNIT]; }
     int currfpMul() { return fpHardwareUnits[MULUNIT]; }
     int currfpAdd() { return fpHardwareUnits[ADDUNIT]; }
     int currintMul() { return intHardwareUnits[MULUNIT]; }
     int currintAdd() { return intHardwareUnits[ADDUNIT]; }
     int currShift() { return intHardwareUnits[SHIFTUNIT]; }
     int currBit() { return intHardwareUnits[BITUNIT]; }
     int totalUnits();
     void calculateLeakagePowerUsage();
     void calculateDynamicPowerUsage(int cycle);
     void calculateRegisterPowerUsage(int read, int write, int count, int wordSize);
     void calculateArea();
     void update(Instruction instr);
     void clearAll();
     float getRegLeak() { return totalPwr.reg_leakage_power; }
     float getRegDyn() { return totalPwr.reg_dynamic_energy; }
     float getLeakage() { return totalPwr.leakage_power; }
     float getDyn() { return totalPwr.dynamic_power; }
     float getReadEnergy() { return totalPwr.readEnergy; }
     float getWriteEnergy() { return totalPwr.writeEnergy; }
     float getArea() { return totalPwr.area; }
     float getDynEnergy() { return totalPwr.dynamic_energy; }
     
     
     Floats floats;
     Integer integer;
     Others others;
     opCodeCount opCount;
     BitCount bitCount;
     ShiftCount shiftCount;
};


#endif //__UTILIZATION_HH__