#ifndef __HWMODEL_INSTRUCTION_CONFIG_HH__
#define __HWMODEL_INSTRUCTION_CONFIG_HH__

#include "params/InstConfig.hh"
#include "sim/sim_object.hh"

#include <iostream>
#include <cstdlib>
#include <vector>

class InstConfig : public SimObject
{
    private:
        std::string LLVM_opcode;
        std::string SALAM_opcode;
        uint64_t runtime_cycles;

    protected:

    public:
        InstConfig(InstConfigParams *params);
};

#endif //__HWMODEL_INSTRUCTION_CONFIG_HH__