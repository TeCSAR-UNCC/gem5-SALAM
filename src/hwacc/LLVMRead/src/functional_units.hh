#ifndef FUNCTIONAL_UNITS_HH
#define FUNCTIONAL_UNITS_HH
//------------------------------------------//
#include "debug_flags.hh" 
#include "power_model.hh"
#include "registers.hh"
//------------------------------------------//
#include <map>
#include <vector>
#include <string>
#include <cstdint>
//------------------------------------------//


struct PowerAreaProfile {
    int latency = 0;
    float internal_power = 0;
    float switch_power = 0;
    float dynamic_power = 0;
    float dynamic_energy = 0;
    float leakage_power = 0;
    float area = 0;
    int usage = 0;
    PowerAreaProfile():
        latency(0.0),
        internal_power(0.0),
        switch_power(0.0),
        dynamic_power(0.0),
        dynamic_energy(0.0),
        leakage_power(0.0),
        area(0.0),
        usage(0) {}
};

class FunctionalUnit {
    private:
     std::vector<bool> occupied;
     std::vector<PowerAreaProfile> multistagePowerAreaReport;
     std::vector<PowerAreaProfile> multistagePowerAreaTotals;
     std::vector<PowerAreaProfile> registerPowerAreaReport; // [0] Read || [1] Write
     std::vector<PowerAreaProfile> registerPowerAreaTotals; // [0] Read || [1] Write
     PowerAreaProfile powerAreaProfile;
     PowerAreaProfile powerAreaReport;
     PowerAreaProfile powerAreaTotals;
     uint8_t hardwareUnit;
     std::string name;
     int cycle_count;
     int parse_count;
     int dynamic_count;
     int dynamic_max;
     int static_limit;
     bool multistage;
     int stages;
     int ID;
    public:
    FunctionalUnit(int Latency, uint8_t HardwareUnit, uint8_t Stages, std::string Name);
    int getDynamicMax() { return dynamic_max; }
    int getStaticLimit() { return static_limit; }
    int getParsedLimit() { return parse_count; }
    int getLatency() { return powerAreaProfile.latency;}
    uint8_t getHardwareUnit() { return hardwareUnit; }
    std::string getName() { return name; }
    int getStages() { return stages; }
    void updateDynamic() { dynamic_count++; }
    void maxDynamic() { if(dynamic_max < dynamic_count) dynamic_max = dynamic_count; }
    int setStatic(int Static);
    void updateParse() { parse_count++; }
    void setCycleCount(int Count) { cycle_count = Count; } 
    void resetStages();
    void resetRuntime();
    void configureMultistage();
    bool isMultistaged() { return multistage; }
    bool available();
    void occupancy();
    void powerUpdate();
    void setID(int newID) { ID = newID; }
    int getID() { return ID; }
    void reserve() {  }
    int defineLimit(int Config_Defined);
    void setStage(int Stage, bool Active);
    bool isActive(int Stage) { return occupied.at(Stage); }
    void printHardwareStats();
};

#endif 