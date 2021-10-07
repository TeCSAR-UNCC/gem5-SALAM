#ifndef __HWMODEL_HW_MODEL_HH__
#define __HWMODEL_HW_MODEL_HH__

#include "params/HWInterface.hh"
#include "sim/sim_object.hh"

#include "../../common/src/macros.hh"

#include "cycle_counts.hh"
#include "functional_units.hh"
#include "instruction_config.hh"
#include "salam_power_model.hh"
//#include "cacti_wrapper.hh"
#include "hw_statistics.hh"
#include "simulator_config.hh"
#include "opcodes.hh"
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace gem5;

class HWInterface : public SimObject 
{
    friend class LLVMInterface;
    private:
    protected:
    public:
        CycleCounts *cycle_counts;
        FunctionalUnits *functional_units;
        HWStatistics *hw_statistics;
        InstConfig *inst_config;
        InstOpCodes *opcodes;
        SALAMPowerModel *salam_power_model;
        SimulatorConfig *simulator_config;

        HWInterface();
        HWInterface(const HWInterfaceParams &params);
        bool availableFunctionalUnit(uint64_t functional_unit);
        void clearFunctionalUnit(uint64_t functional_unit);

};

#endif //__HWMODEL_HW_MODEL_HH__