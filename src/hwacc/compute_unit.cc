//------------------------------------------//
#include "hwacc/compute_unit.hh"
//------------------------------------------//

ComputeUnit::ComputeUnit(ComputeUnitParams *p) :
    SimObject(p),
    comm(p->comm_int),
    tickEvent(this) {}

ComputeUnit*
ComputeUnitParams::create() {
    return new ComputeUnit(this);
}
