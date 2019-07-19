#ifndef __DATA_COLLECTION_HH__
#define __DATA_COLLECTION_HH__
#include <iostream>
#include <iomanip>
#include <fstream>

class Results {

    private:
        //Performance
        int cycles;
        double runtime;
        int stalls;
        int nodes;
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
        Results(    int Cycles,
                    double Runtime,
                    int Stalls,
                    int Nodes,
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
                    cycles(Cycles),
                    runtime(Runtime),
                    stalls(Stalls),
                    nodes(Nodes),
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
        void simpleStats();
                
};


#endif //__DATA_COLLECTION_HH__