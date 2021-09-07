#include "hw_statistics.hh"

HWStatistics::HWStatistics(HWStatisticsParams *params) :
    SimObject(params) { }

HWStatistics*
HWStatisticsParams::create() {
    //if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return new HWStatistics(this);
}