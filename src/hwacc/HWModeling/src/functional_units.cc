#include "functional_units.hh"

FunctionalUnits::FunctionalUnits(FunctionalUnitsParams *params) :
    SimObject(params) { }

FunctionalUnits*
FunctionalUnitsParams::create() {
    //if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return new FunctionalUnits(this);
}