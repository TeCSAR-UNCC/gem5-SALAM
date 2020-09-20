#ifndef UTILIZATION_HH
#define UTILIZATION_HH
//------------------------------------------//
#include "functional_units.hh" 
#include "debug_flags.hh" 
#include "cycle_count.hh"
#include "registers.hh"
#include <cstdint>


//------------------------------------------//

class RegisterList;

struct Pwr_Parameters {
    float cycleTime;
    float internal_power;
    float switch_power;
    float leakage_power;
    float area;
    Pwr_Parameters():
        cycleTime(0.0),
        internal_power(0.0),
        switch_power(0.0),
        leakage_power(0.0),
        area(0.0) {}
};

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

struct FunctionalUnitOccupancy {
  double counter_units;
  double int_adder_units;
  double int_multiply_units;
  double int_shifter_units;
  double int_bit_units;
  double fp_sp_adder;
  double fp_dp_adder;
  double fp_sp_multiply;
  double fp_dp_multiply;  
  double compare;
  double gep;
  double conversion;
  double other;
  double fpDivision = 0;  
};

struct PowerUsage {
    float cycleTime = 0;
    float internal_power = 0;
    float switch_power = 0;
    float dynamic_power = 0;
    float dynamic_energy = 0;
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

struct CactiResults {
    uca_org_t opt;
    uca_org_t leakage;
    uca_org_t dyn_read;
    uca_org_t dyn_write;
};

class Utilization {
    private:
     int _Clock_Period = 0;
     int reuse_factor;
     int fp_cycle_count = 5;

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
    void setFPReuseFactor(int newFactor) { reuse_factor = newFactor; }
};

struct Occupancy {
    int loadOnly; //
    int storeOnly; //
    int compOnly; //
    int loadStore; //
    int loadComp;
    int loadStoreComp;
    int storeComp;
    Occupancy():
        loadOnly(0),
        storeOnly(0),
        compOnly(0),
        loadStore(0),
        loadComp(0),
        loadStoreComp(0),
        storeComp(0) {}
};



class Hardware {
    public:
        CycleCounts * cycles;
        FunctionalUnit * counter;
        FunctionalUnit * int_adder;
        FunctionalUnit * int_multiplier;
        FunctionalUnit * int_shifter;
        FunctionalUnit * int_bitwise;
        FunctionalUnit * fp_sp_adder;
        FunctionalUnit * fp_dp_adder;
        FunctionalUnit * fp_sp_multiplier;
        FunctionalUnit * fp_sp_division;
        FunctionalUnit * fp_dp_multiplier;
        FunctionalUnit * fp_dp_division;
        FunctionalUnit * comparison;
        FunctionalUnit * getelementptr;
        FunctionalUnit * conversion;
        FunctionalUnit * registers;
        FunctionalUnit * other;
        RegisterList * regList;
        int loads;
        int stores;
        int control_flow;

        Occupancy occ_stalled;
        Occupancy occ_scheduled;
        Hardware(int Latency);
        void reset();
        void update();
        void updateMax();
        void pipelined();
        void linkRegList(RegisterList * List) { regList = List; }
        bool available(uint8_t HardwareUnit);
        void updateParsed(uint8_t HardwareUnit);
        void updateDynamic(uint8_t HardwareUnit);
        void memoryLoad() { loads++; }
        void memoryStore() { stores++; }
        void controlFlow() { control_flow++; }
        void updateLimit(int Counter, 
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
                            int Conversion);
        void printResults(); 
        void printOccupancyResults();
        void printLLVMFunctionalUnits();
        void printDefinedFunctionalUnits();
        void printDynamicFunctionalUnits();
        void printPowerAnalysis();
        void printAreaAnalysis();
        void printRegisterUsageAnalysis();
        void printRegisterPowerAnalysis();
        void printRegisterAreaAnalysis();
        void printTotals();                    

};

#endif //__UTILIZATION_HH__