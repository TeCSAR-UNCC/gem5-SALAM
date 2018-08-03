#ifndef __POWER_HH__

#define __POWER_HH__

//#include "power_func.hh"

#include <cstdint>

#define TRANSISTOR 10

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

struct Reg_Usage {
    uint64_t reads;
    uint64_t writes;
    Reg_Usage():reads(0), writes(0) {}
};

#endif //__POWER_HH__