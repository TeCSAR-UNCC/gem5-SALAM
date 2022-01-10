#ifndef __HWMODEL_HW_STATISTICS_HH__
#define __HWMODEL_HW_STATISTICS_HH__

#include "params/HWStatistics.hh"
#include "sim/sim_object.hh"

#include "hwacc/LLVMRead/src/debug_flags.hh"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace gem5;


// Things here are output only once at end of simulation
struct HW_Params {
    int run_end;

    void reset() {
        run_end = 0;
    }

};

// These are outputs that are stored each cycle
struct HW_Cycle_Stats {
    int cycle;

    int resInFlight;

    int loadInFlight;
    int loadInternal;
    int loadAcitve;
    int loadRawStall;

    int storeInFlight;
    int storeActive;

    int compInFlight;
    int compLaunched;
    int compActive;
    int compFUStall;
    int compCommited;



    void reset() {
        cycle = 0;
        resInFlight = 0;
        loadInFlight = 0;
        storeInFlight = 0;
        compInFlight = 0;
    }
};

class HWStatistics : public SimObject
{
    private:
        HW_Params hw_params;
        HW_Cycle_Stats current_cycle_stats;
        std::vector<HW_Cycle_Stats>::iterator cycle_buffer;
        std::vector<std::vector<HW_Cycle_Stats>> hw_buffer_list;
        std::vector<std::vector<HW_Cycle_Stats>>::iterator hw_buffer;

        // Make Into SimObjects to pass from config.yml
        bool cycle_tracking = false;
        bool dbg;
        int statBufferSize;
        int statBufferPreDefine;


        // Class Only
        int current_buffer_index = 0;


    public:
        HWStatistics();
        HWStatistics(const HWStatisticsParams &params);
        bool use_cycle_tracking() { return cycle_tracking; }

        void print();
        void simpleStats();
        void unitCorrections();
        void updateHWStatsCycleStart();
        void updateHWStatsCycleEnd(int curr_cycle);
        void updateBuffer();
        void clearStats();
};

#endif //__HWMODEL_HW_STATISTICS_HH__

/*
        //Performance
        double setupTime;
        double simTime;
        int clock_period;
        double sys_clock;
        int fu_clock_period;
        int cycles;
        double runtime;
        int stalls;
        int nodes;
        int loadOnly; //
        int storeOnly; //
        int compOnly; //
        int loadStore; //
        int loadComp;
        int loadStoreComp;
        int storeComp;
        int loadOnlyStall; //
        int storeOnlyStall; //
        int compOnlyStall;
        int loadStoreStall; //
        int loadCompStall;
        int loadStoreCompStall;
        int storeCompStall;        
        //Memory Stats
        int cache_ports;
        int local_ports;
        int cache_size;
        int spm_size;
        int read_ports;
        int write_ports;
        int read_bus_width;
        int write_bus_width;
        double spm_leakage;
        double spm_read_dynamic;
        double spm_write_dynamic;
        double spm_area;
        double spm_opt_leakage;
        double spm_opt_area;
        double cache_leakage;
        double cache_read_dynamic;
        double cache_write_dynamic;
        double cache_area;
        // Runtime Functional Units
        int run_counter;
        double occ_counter;
        int run_add_sub;
        double occ_add_sub;
        int run_mul_div;
        double occ_mul_div;
        int run_shift;
        double occ_shift;
        int run_bit;
        double occ_bit;
        int run_float_add_sub;
        double occ_float_add_sub;
        int run_doub_add_sub;
        double occ_doub_add_sub;
        int run_float_mul_div;
        double occ_float_mul_div;
        int run_doub_mul_div;
        double occ_doub_mul_div;
        int run_zero;
        double occ_zero;
        int run_gep;
        double occ_gep;
        int run_conv;
        double occ_conv;
        // Static Functional Units
        int static_counter;
        int static_add_sub;
        int static_mul_div;
        int static_shift;
        int static_bit;
        int static_float_add_sub;
        int static_doub_add_sub;
        int static_float_mul_div;
        int static_doub_mul_div;
        int static_zero;
        int static_gep;
        int static_conv;
        int static_other;
        // Registers
        int reg_total;
        int reg_max_usage;
        double reg_avg_usage;
        double reg_avg_size;
        int reg_reads;
        int reg_writes;
        int mem_reads;
        int mem_writes;
        int dma_reads;
        int dma_writes;
        // Power
        double fu_leakage;
        double fu_dynamic;
        double fu_total_power;
        double reg_leakage;
        double reg_dynamic;
        double reg_total_power;
        double total_power;
        // Area
        double fu_area;
        double reg_area;
        double total_area;
        // Totals
        double spm_total_power;
        double cache_total_power;
        double acc_spm_total_power;
        double acc_cache_total_power;
        double acc_spm_total_area;
        double acc_cache_total_area;
        */