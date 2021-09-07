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
    private:
    uint32_t testParam;
    //CycleCounts *cycleCounts;

    protected:

    public:
    HWInterface(HWInterfaceParams *params);
    uint32_t getTestParam() { return testParam; }

};

#endif //__HWMODEL_HW_MODEL_HH__