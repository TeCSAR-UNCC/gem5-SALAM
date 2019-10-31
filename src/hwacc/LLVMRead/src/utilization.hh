#ifndef __UTILIZATION_HH__
#define __UTILIZATION_HH__
#include "debugFlags.hh" 
#include "debug/Hardware.hh"
#include "power_func.hh"
#include "registers.hh"
#include "power.hh"
#include "macros.hh"
#include <map>
#include <vector>

struct FunctionalUnits {
  int32_t counter_units;
  int32_t int_adder_units;
  int32_t int_multiply_units;
  int32_t int_shifter_units;
  int32_t int_bit_units;
  int32_t fp_sp_adder;
  int32_t fp_dp_adder;
  int32_t fp_sp_multiply;
  int32_t fp_dp_multiply;  
  int32_t compare;
  int32_t gep;
  int32_t conversion;
  int32_t other;
  int32_t fpDivision = 0;  
};


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
    float reg_area = 0;
};

class Utilization {
    private:
     int _Clock_Period = 0;

     public:
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
      PowerTotals finalPwr;
      RegisterList *regList;
      Reg_Usage regUsage;
    
    Utilization(int clock_period, int fu_clock_period, RegisterList* List);
    void finalPowerUsage(FunctionalUnits units, int cycle); 
    uca_org_t getCactiResults(int cache_size, int word_size, int ports, int cache_type);
    void updatePowerConsumption(FunctionalUnits units);
    void calculateLeakagePowerUsage(FunctionalUnits units);
    void calculateFinalLeakagePowerUsage(FunctionalUnits units);
    void calculateDynamicPowerUsage(FunctionalUnits units);
    void calculateArea(FunctionalUnits units);
    void calculateRegisterPowerUsage(Reg_Usage *regUsage, int cycle);
};


#endif //__UTILIZATION_HH__