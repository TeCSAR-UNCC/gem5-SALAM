#include "hwacc/compute_unit.hh"

ComputeUnit::ComputeUnit(ComputeUnitParams *p) :
    SimObject(p),
    comm(p->comm_int) {}

ComputeUnit*
ComputeUnitParams::create() {
    return new ComputeUnit(this);
}
