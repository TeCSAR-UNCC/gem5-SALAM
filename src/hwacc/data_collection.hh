#ifndef __DATA_COLLECTION_HH__
#define __DATA_COLLECTION_HH__
#include <iostream>
#include <iomanip>
#include <fstream>

class Results {

    private:
        //Performance
        int clock_period;
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
        // Runtime Functional Units
        int run_counter;
        int run_add_sub;
        int run_mul_div;
        int run_shift;
        int run_bit;
        int run_float_add_sub;
        int run_doub_add_sub;
        int run_float_mul_div;
        int run_doub_mul_div;
        int run_zero;
        int run_gep;
        int run_conv;
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

    public:
        Results();
        Results(    int Clock_Period,
                    int FU_Clock_Period,
                    int Cycles,
                    double Runtime,
                    int Stalls,
                    int Nodes,
                    int LoadOnly,
                    int StoreOnly,
                    int CompOnly,
                    int LoadStore,
                    int LoadComp,
                    int LoadStoreComp,
                    int StoreComp,
                    int LoadOnlyStall,
                    int StoreOnlyStall,
                    int CompOnlyStall,
                    int LoadStoreStall,
                    int LoadCompStall,
                    int LoadStoreCompStall,
                    int StoreCompStall,
                    int SPM_Size,
                    int Read_Ports,
                    int Write_Ports,
                    int Read_Bus_Width,
                    int Write_Bus_Width,
                    double SPM_Leakage,
                    double SPM_Read_Dynamic,
                    double SPM_Write_Dynamic,
                    double SPM_Area,
                    double SPM_Opt_Leakage,
                    double SPM_Opt_Area,
                    int Run_counter,
                    int Run_add_sub,
                    int Run_mul_div,
                    int Run_shift,
                    int Run_bit,
                    int Run_float_add_sub,
                    int Run_doub_add_sub,
                    int Run_float_mul_div,
                    int Run_doub_mul_div,
                    int Run_zero,
                    int Run_gep,
                    int Run_conv,
                    int Static_counter,
                    int Static_add_sub,
                    int Static_mul_div,
                    int Static_shift,
                    int Static_bit,
                    int Static_float_add_sub,
                    int Static_doub_add_sub,
                    int Static_float_mul_div,
                    int Static_doub_mul_div,
                    int Static_zero,
                    int Static_gep,
                    int Static_conv,
                    int Static_other,
                    int Reg_total,
                    int Reg_max_usage,
                    double Reg_avg_usage,
                    double Reg_avg_size,
                    int Reg_reads,
                    int Reg_writes,
                    int Memory_Loads,
                    int Memory_Stores,
                    int DMA_Reads,
                    int DMA_Writes,
                    double Fu_leakage,
                    double Fu_dynamic,
                    double Fu_total_power,
                    double Reg_leakage,
                    double Reg_dynamic,
                    double Reg_total_power,
                    double Total_power,
                    double Fu_area,
                    double Reg_area,
                    double Total_area):
                    clock_period(Clock_Period),
                    fu_clock_period(FU_Clock_Period),
                    cycles(Cycles),
                    runtime(Runtime),
                    stalls(Stalls),
                    nodes(Nodes),
                    loadOnly(LoadOnly),
                    storeOnly(StoreOnly),
                    compOnly(CompOnly),
                    loadStore(LoadStore),
                    loadComp(LoadComp),
                    loadStoreComp(LoadStoreComp),
                    storeComp(StoreComp),
                    loadOnlyStall(LoadOnlyStall),
                    storeOnlyStall(StoreOnlyStall),
                    compOnlyStall(CompOnlyStall),
                    loadStoreStall(LoadStoreStall),
                    loadCompStall(LoadCompStall),
                    loadStoreCompStall(LoadStoreCompStall),
                    storeCompStall(StoreCompStall),
                    spm_size(SPM_Size),
                    read_ports(Read_Ports),
                    write_ports(Write_Ports),
                    read_bus_width(Read_Bus_Width),
                    write_bus_width(Write_Bus_Width),
                    spm_leakage(SPM_Leakage),
                    spm_read_dynamic(SPM_Read_Dynamic),
                    spm_write_dynamic(SPM_Write_Dynamic),
                    spm_area(SPM_Area),
                    spm_opt_leakage(SPM_Opt_Leakage),
                    spm_opt_area(SPM_Opt_Area),
                    run_counter(Run_counter),
                    run_add_sub(Run_add_sub),
                    run_mul_div(Run_mul_div),
                    run_shift(Run_shift),
                    run_bit(Run_bit),
                    run_float_add_sub(Run_float_add_sub),
                    run_doub_add_sub(Run_doub_add_sub),
                    run_float_mul_div(Run_float_mul_div),
                    run_doub_mul_div(Run_doub_mul_div),
                    run_zero(Run_zero),
                    run_gep(Run_gep),
                    run_conv(Run_conv),
                    static_counter(Static_counter),
                    static_add_sub(Static_add_sub),
                    static_mul_div(Static_mul_div),
                    static_shift(Static_shift),
                    static_bit(Static_bit),
                    static_float_add_sub(Static_float_add_sub),
                    static_doub_add_sub(Static_doub_add_sub),
                    static_float_mul_div(Static_float_mul_div),
                    static_doub_mul_div(Static_doub_mul_div),
                    static_zero(Static_zero),
                    static_gep(Static_gep),
                    static_conv(Static_conv),
                    static_other(Static_other),
                    reg_total(Reg_total),
                    reg_max_usage(Reg_max_usage),
                    reg_avg_usage(Reg_avg_usage),
                    reg_avg_size(Reg_avg_size),
                    reg_reads(Reg_reads),
                    reg_writes(Reg_writes),
                    mem_reads(Memory_Loads),
                    mem_writes(Memory_Stores),
                    dma_reads(DMA_Reads),
                    dma_writes(DMA_Writes),
                    fu_leakage(Fu_leakage),
                    fu_dynamic(Fu_dynamic),
                    fu_total_power(Fu_total_power),
                    reg_leakage(Reg_leakage),
                    reg_dynamic(Reg_dynamic),
                    reg_total_power(Reg_total_power),
                    total_power(Total_power),
                    fu_area(Fu_area),
                    reg_area(Reg_area),
                    total_area(Total_area) { }

        void print();
};


#endif //__DATA_COLLECTION_HH__