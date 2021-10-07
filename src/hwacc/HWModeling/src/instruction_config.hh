#ifndef __HWMODEL_INSTRUCTION_CONFIG_HH__
#define __HWMODEL_INSTRUCTION_CONFIG_HH__

#include "params/InstConfig.hh"
#include "sim/sim_object.hh"

// GENERATED HEADERS - DO NOT MODIFY
// #include "../generated/instructions/add.hh"
// END OF GENERATED HEADERS

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace gem5;

class InstConfig : public SimObject
{
    private:
        // GENERATED CLASS MEMBERS - DO NOT MODIFY
        // END OF GENERATED CLASS MEMBERS

    protected:

    public:
        InstConfig();
        // DEFAULT CONSTRUCTOR - DO NOT MODIFY
        InstConfig(const InstConfigParams &params);
        // END DEFAULT CONSTRUCTOR
};

#endif //__HWMODEL_INSTRUCTION_CONFIG_HH__