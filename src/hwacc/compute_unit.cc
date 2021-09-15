//------------------------------------------//
#include "hwacc/compute_unit.hh"
//------------------------------------------//

ComputeUnit::ComputeUnit(const ComputeUnitParams &p) :
    SimObject(p),
    comm(p.comm_int),
    hw(p.hw_int),
    tickEvent(this) {}

// ComputeUnit*
// ComputeUnitParams::create() {
//     return new ComputeUnit(this);
// }
