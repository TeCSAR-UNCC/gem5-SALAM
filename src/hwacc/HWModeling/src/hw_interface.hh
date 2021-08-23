#ifndef __HWMODEL_HW_MODEL_HH__
#define __HWMODEL_HW_MODEL_HH__

#include "params/HWInterface.hh"
#include "sim/sim_object.hh"

#include "cycle_counts.hh"
#include <iostream>
#include <cstdlib>
#include <vector>

//namespace SALAM
//{

    class HWInterface : public CycleCounts
    {
        private:
        uint32_t testParam;
        //CycleCounts *cycleCounts;

        protected:

        public:
        HWInterface(HWInterfaceParams *params);
        uint32_t getTestParam() { return testParam; }



    };

    class InstModel
    {
        private:
            std::string LLVM_opcode;
            std::string SALAM_opcode;
            uint64_t runtime_cycles;

        protected:

        public:
            InstModel();
    };

    class PowerModel
    {
        private:
            struct units {
                std::string name;
                double exponent;
                // automate matching units with exponent
            } power_unit, energy_unit, time_unit, area_unit;
            
            double internal_power;
            double swtich_power;
            double dynmaic_power;
            double leakage_power;

            double dynamic_energy;
            double latency;
            double path_delay;
            double area;

        protected:

        public:
            PowerModel();
            // create constructor
            // getters/setters
        
    };


    class FunctionalUnit
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
            PowerModel power_model;

        protected:

        public:
            FunctionalUnit();
            // constructor
            // deconstructor

    };


//}

#endif //__HWMODEL_HW_MODEL_HH__