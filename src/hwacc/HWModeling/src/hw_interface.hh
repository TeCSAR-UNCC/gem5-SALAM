#ifndef __HWMODEL_HW_MODEL_HH__
#define __HWMODEL_HW_MODEL_HH__

#include "params/HWInterface.hh"
#include "sim/sim_object.hh"

#include "cycle_counts.hh"
#include "functional_units.hh"
#include "instruction_config.hh"
#include "salam_power_model.hh"
//#include "cacti_wrapper.hh"
#include "hw_statistics.hh"
#include "simulator_config.hh"
#include <iostream>
#include <cstdlib>
#include <vector>

class HWInterface : public SimObject 
{
    friend class LLVMInterface;
    private:
        CycleCounts *cycle_counts;
        FunctionalUnits *functional_units;
        HWStatistics *hw_statistics;
        InstConfig *inst_config;
        OpCodes *opcodes;
        SALAMPowerModel *salam_power_model;
        SimulatorConfig *simulator_config;

    protected:

    public:
        HWInterface();
        HWInterface(HWInterfaceParams *params);
        uint32_t getTestParam() { return this->cycle_counts->add_inst; }

};

#endif //__HWMODEL_HW_MODEL_HH__