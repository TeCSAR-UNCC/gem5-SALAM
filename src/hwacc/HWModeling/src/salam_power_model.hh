#ifndef __HWMODEL_SALAM_POWER_MODEL_HH__
#define __HWMODEL_SALAM_POWER_MODEL_HH__

#include "params/SALAMPowerModel.hh"
#include "sim/sim_object.hh"

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace gem5;

class SALAMPowerModel : public SimObject
{
    private:
        struct units {
            std::string name;
            double exponent;
        } power_unit, energy_unit, time_unit, area_unit;
        
        double internal_power;
        double swtich_power;
        double dynmaic_power;
        double leakage_power;

        double dynamic_energy;
        double latency;
        double path_delay;
        double area;

    protected:

    public:
        SALAMPowerModel();
        SALAMPowerModel(const SALAMPowerModelParams &params);
    
};

#endif //__HWMODEL_SALAM_POWER_MODEL_HH__