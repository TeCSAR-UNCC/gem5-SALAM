#ifndef __HWMODEL_HW_MODEL_HH__
#define __HWMODEL_HW_MODEL_HH__

#include "params/HWInterface.hh"
#include "sim/sim_object.hh"

#include "cycle_counts.hh"


class HWInterface : public SimObject
{
    private:
    uint32_t testParam;
    CycleCounts *cycleCounts;

    protected:

    public:
    HWInterface(HWInterfaceParams *params);

};

#endif //__HWMODEL_HW_MODEL_HH__