#ifndef __HWMODEL_HW_STATISTICS_HH__
#define __HWMODEL_HW_STATISTICS_HH__

#include "params/HWStatistics.hh"
#include "sim/sim_object.hh"

#include <iostream>
#include <cstdlib>
#include <vector>

class HWStatistics : public SimObject
{
    private:
 
    protected:

    public:
        HWStatistics();
        HWStatistics(HWStatisticsParams *params);
    
};

#endif //__HWMODEL_HW_STATISTICS_HH__