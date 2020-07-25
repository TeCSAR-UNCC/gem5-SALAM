#ifndef UTILIZATION_HH
#define UTILIZATION_HH
//------------------------------------------//
#include "functional_units.hh" 
#include "debug_flags.hh" 
#include "registers.hh"
#include <memory>
#include <vector>

//------------------------------------------//

class RegisterList;

struct Occupancy {
    int loadOnly; //
    int storeOnly; //
    int compOnly; //
    int loadStore; //
    int loadComp;
    int loadStoreComp;
    int storeComp;
    Occupancy():
        loadOnly(0),
        storeOnly(0),
        compOnly(0),
        loadStore(0),
        loadComp(0),
        loadStoreComp(0),
        storeComp(0) {}
};


class Hardware {
    public:
        std::vector<FunctionalUnit*> counter_list;
        std::vector<FunctionalUnit*> int_adder_list;
        std::vector<FunctionalUnit*> int_multiplier_list;
        std::vector<FunctionalUnit*> int_shifter_list;
        std::vector<FunctionalUnit*> int_bitwise_list;
        std::vector<FunctionalUnit*> fp_sp_adder_list;
        std::vector<FunctionalUnit*> fp_dp_adder_list;
        std::vector<FunctionalUnit*> fp_sp_multiplier_list;
        std::vector<FunctionalUnit*> fp_sp_division_list;
        std::vector<FunctionalUnit*> fp_dp_multiplier_list;
        std::vector<FunctionalUnit*> fp_dp_division_list;
        std::vector<FunctionalUnit*> comparison_list;
        std::vector<FunctionalUnit*> getelementptr_list;
        std::vector<FunctionalUnit*> conversion_list;
        std::vector<FunctionalUnit*> other_list;
        std::vector<FunctionalUnit*> runtime_list;
        std::vector<std::vector<FunctionalUnit*> > hardware_list;
        FunctionalUnit * registers;
        RegisterList * regList;
        int loads;
        uint64_t bytes_loaded;
        int stores;
        uint64_t bytes_stored;
        int control_flow;
        int pipelined;

        Occupancy occ_stalled;
        Occupancy occ_scheduled;
        Hardware(int Latency, int Pipelined);
        void activateUnits( int Units,
                            int Latency,
                            int Stages,
                            int Static_Limit,
                            uint8_t ID,
                            std::string Name,
                            std::vector<FunctionalUnit*> &FU_List);
        void reset();
        void update();
        void updateMax();
        void setIDs();
        void linkRegList(RegisterList * List) { regList = List; }
        int reserveFU(uint8_t HardwareUnit);
        bool isMultistaged(uint8_t HardwareUnit); 
        void updateParsed(uint8_t HardwareUnit);
        bool updateStage(uint8_t HardwareUnit, int CurrentStage, int ID);
        void memoryLoad(uint64_t Size) { loads++; bytes_loaded+=Size; }
        void memoryStore(uint64_t Size) { stores++; bytes_stored+=Size; }
        void controlFlow() { control_flow++; }
        void updateLimit(   int Counter, 
                            int IntAdder, 
                            int IntMul, 
                            int IntShift, 
                            int IntBit, 
                            int FPSPAdd, 
                            int FPDPAdd, 
                            int FPSPMul, 
                            int FPSPDiv, 
                            int FPDPMul, 
                            int FPDPDiv, 
                            int Compare, 
                            int GEP, 
                            int Conversion);
        void powerUpdate();
        void printFunctionalUnits();
        void printResults(); 
        void printOccupancyResults();
        void printLLVMFunctionalUnits();
        void printDefinedFunctionalUnits();
        void printDynamicFunctionalUnits();
        void printPowerAnalysis();
        void printAreaAnalysis();
        void printRegisterUsageAnalysis();
        void printRegisterPowerAnalysis();
        void printRegisterAreaAnalysis();
        void printTotals(); 
        void finalize();
                      

};

#endif //__UTILIZATION_HH__