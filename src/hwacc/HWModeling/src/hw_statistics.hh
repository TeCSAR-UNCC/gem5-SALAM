#ifndef __HWMODEL_HW_STATISTICS_HH__
#define __HWMODEL_HW_STATISTICS_HH__

#include "params/HWStatistics.hh"
#include "sim/sim_object.hh"

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace gem5;

class HWStatistics : public SimObject
{
    private:
 
    protected:

    public:
        HWStatistics();
        HWStatistics(const HWStatisticsParams &params);
    
};

#endif //__HWMODEL_HW_STATISTICS_HH__