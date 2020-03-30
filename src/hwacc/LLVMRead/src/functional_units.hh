#ifndef FUNCTIONAL_UNITS_HH
#define FUNCTIONAL_UNITS_HH
//------------------------------------------//
#include "debug_flags.hh" 
#include "power_model.hh"
//------------------------------------------//
#include <map>
#include <vector>
#include <cstdint>
//------------------------------------------//

struct PowerAreaProfile {
    float latency = 0;
    float internal_power = 0;
    float switch_power = 0;
    float dynamic_power = 0;
    float dynamic_energy = 0;
    float leakage_power = 0;
    float area = 0;
    int cycleCount = 0;
    PowerAreaProfile():
        latency(0.0),
        internal_power(0.0),
        switch_power(0.0),
        dynamic_power(0.0),
        dynamic_energy(0.0),
        leakage_power(0.0),
        area(0.0),
        cycleCount(0) {}
};

class FunctionalUnit {
    private:
     PowerAreaProfile powerAreaProfile;
     uint8_t hardwareUnit;
     int cycle_count;
     int parse_count;
     int dynamic_count;
     int dynamic_max;
     int static_limit;
     bool multistage;
    public:
    FunctionalUnit(int Latency, uint8_t HardwareUnit);
    int getDynamicMax() { return dynamic_max; }
    int getStaticLimit() { return static_limit; }
    int getParsedLimit() { return parse_count; }
    void updateDynamic() { dynamic_count++; }
    void maxDynamic() { if(dynamic_max < dynamic_count) dynamic_max = dynamic_count; }
    void setStatic(int Static);
    void setRuntime(int Runtime) { dynamic_max = Runtime; }
    void updateParse() { parse_count++; }
    void setCycleCount(int Count) { cycle_count = Count; } 
    void reset() { dynamic_count = 0; }
    void multistaged() { multistage = true; }
    bool available();
    void occupancy();
};

#endif 