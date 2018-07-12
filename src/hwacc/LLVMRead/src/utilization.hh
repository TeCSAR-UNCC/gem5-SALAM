#ifndef __UTILIZATION_HH__

#define __UTILIZATION_HH__

#include "debugFlags.hh" 
#include "debug/Hardware.hh"
#include "power_func.hh"
#include "instruction.hh"
#include <map>
#include <vector>

#define ADDUNIT 0
#define MULUNIT 1
#define BITUNIT 2
#define SHIFTUNIT 3

struct PowerUsage {
    float cycleTime = 1;
    float internal_power = 0;
    float switch_power = 0;
    float leakage_power = 0;
    float area = 0;
};

struct PowerTotals {
    float reg_leakage_power = 0;
    float reg_dynamic_energy = 0;
    float leakage_power = 0;
    float dynamic_power = 0;
    float readEnergy = 0;
    float writeEnergy = 0;
    float area = 0;
};

class Utilization {
    private:
     float cycleTime = 1;
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
    
     Utilization(); 
     void maxUnits(int count, int unit, bool fp);
     void currUnits(int count, int unit, bool fp);
     int fpMaxMul() { return fpHardwareUnits[MULUNIT]; }
     int fpMaxAdd() { return fpHardwareUnits[ADDUNIT]; }
     int intMaxMul() { return intHardwareUnits[MULUNIT]; }
     int intMaxAdd() { return intHardwareUnits[ADDUNIT]; }
     int maxShift() { return intHardwareUnits[SHIFTUNIT]; }
     int maxBit() { return intHardwareUnits[BITUNIT]; }
     int totalUnits();
     void calculateLeakagePowerUsage();
     void calculateDynamicPowerUsage();
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
     
     Floats floats;
     Integer integer;
     Others others;
     opCodeCount opCount;
     BitCount bitCount;
     ShiftCount shiftCount;
};


#endif //__UTILIZATION_HH__