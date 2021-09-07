#ifndef __HWMODEL_FUNCTIONAL_UNITS_HH__
#define __HWMODEL_FUNCTIONAL_UNITS_HH__

#include "params/FunctionalUnits.hh"
#include "sim/sim_object.hh"

#include <iostream>
#include <cstdlib>
#include <vector>


class FunctionalUnits : public SimObject
{
    private:
        std::string type;
        //uint65_t type; create #DEFINE list
        std::string alias;
        std::string datatype;
        //uint64_t datatype; // create #DEFINE list
        uint64_t stages;
        std::vector<std::string> inst_str; // Enumerate instructions list
        std::vector<uint64_t> inst; // Enumerate instructions list
        //PowerModel power_model;

    protected:

    public:
        FunctionalUnits(FunctionalUnitsParams *params);
        // constructor
        // deconstructor

};

#endif //__HWMODEL_FUNCTIONAL_UNITS_HH__