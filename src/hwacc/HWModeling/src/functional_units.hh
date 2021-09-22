#ifndef __HWMODEL_FUNCTIONAL_UNITS_HH__
#define __HWMODEL_FUNCTIONAL_UNITS_HH__

#include "params/FunctionalUnits.hh"
#include "sim/sim_object.hh"

// GENERATED HEADERS - DO NOT MODIFY
#include "../generated/functionalunits/adder.hh"
// END OF GENERATED HEADERS

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace gem5;

class FunctionalUnits : public SimObject
{
    private:
        // GENERATED CLASS MEMBERS - DO NOT MODIFY
        Adder* int_adder;
        // END OF GENERATED CLASS MEMBERS

    protected:

    public:
        FunctionalUnits();
        // DEFAULT CONSTRUCTOR - DO NOT MODIFY
        FunctionalUnits(const FunctionalUnitsParams &params);
        // END DEFAULT CONSTRUCTOR
};


#endif //__HWMODEL_FUNCTIONAL_UNITS_HH__