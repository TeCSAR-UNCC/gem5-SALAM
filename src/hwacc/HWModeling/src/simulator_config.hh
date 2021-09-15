#ifndef __HWMODEL_SIMULATOR_CONFIG_HH__
#define __HWMODEL_SIMULATOR_CONFIG_HH__

#include "params/SimulatorConfig.hh"
#include "sim/sim_object.hh"

using namespace gem5;

class SimulatorConfig : public SimObject
{
    private:

    protected:

    public:
        SimulatorConfig();
        SimulatorConfig(const SimulatorConfigParams &params);
};

#endif //__HWMODEL_SIMULATOR_CONFIG_HH__