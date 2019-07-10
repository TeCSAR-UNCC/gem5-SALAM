#include "data_collection.hh"

void
Results::print() {

    std::cout << "********************************************************************************" << std::endl;
    std::cout << "   ========= Performance Analysis =============" << std::endl;
    std::cout << "   Runtime (Cycles):                " << cycles << std::endl;
    std::cout << "   Runtime (Seconds):               " << runtime << std::endl;
    std::cout << "   Stalls  (Cycles):                " << stalls << std::endl;
    std::cout << "   Executed Nodes:                  " << nodes << std::endl;
    std::cout << "   ========= Runtime Functional Units =========" << std::endl;
    std::cout << "   Counter FU's:                    " << run_counter << std::endl;
    std::cout << "   Integer Add/Sub FU's:            " << run_add_sub << std::endl;
    std::cout << "   Integer Mul/Div FU's:            " << run_mul_div << std::endl;
    std::cout << "   Integer Shifter FU's:            " << run_shift << std::endl;
    std::cout << "   Integer Bitwise FU's:            " << run_bit << std::endl;
    std::cout << "   Floating Point Float Add/Sub:    " << run_float_add_sub << std::endl;
    std::cout << "   Floating Point Double Add/Sub:   " << run_doub_add_sub << std::endl;
    std::cout << "   Floating Point Float Mul/Div:    " << run_float_mul_div << std::endl;
    std::cout << "   Floating Point Double Mul/Div:   " << run_doub_mul_div << std::endl;
    std::cout << "   0 Cycle Compare FU's:            " << run_zero << std::endl;
    std::cout << "   GEP Instruction FU's:            " << run_gep << std::endl;
    std::cout << "   Type Conversion FU's:            " << run_conv << std::endl;
    std::cout << "   ========= Static Functional Units ========="  << std::endl;
    std::cout << "   Counter FU's:                    " << static_counter << std::endl;
    std::cout << "   Integer Add/Sub FU's:            " << static_add_sub << std::endl;
    std::cout << "   Integer Mul/Div FU's:            " << static_mul_div << std::endl;
    std::cout << "   Integer Shifter FU's:            " << static_shift << std::endl;
    std::cout << "   Integer Bitwise FU's:            " << static_bit << std::endl;
    std::cout << "   Floating Point Float Add/Sub:    " << static_float_add_sub << std::endl;
    std::cout << "   Floating Point Double Add/Sub:   " << static_doub_add_sub << std::endl;
    std::cout << "   Floating Point Float Mul/Div:    " << static_float_mul_div << std::endl;
    std::cout << "   Floating Point Double Mul/Div:   " << static_doub_mul_div << std::endl;
    std::cout << "   0 Cycle Compare FU's:            " << static_zero << std::endl;
    std::cout << "   GEP Instruction FU's:            " << static_gep << std::endl;
    std::cout << "   Type Conversion FU's:            " << static_conv << std::endl;
    std::cout << "   Other:                           " << static_other << std::endl;
    std::cout << "   ========= Register Usage ==================" << std::endl;
    std::cout << "   Total Number of Registers:       " << reg_total << std::endl;
    std::cout << "   Max Register Usage Per Cycle:    " << reg_max_usage << std::endl;
    std::cout << "   ========= Power Analysis ==================" << std::endl;
    std::cout << "   FU Leakage Power:                " << fu_leakage << " mW " << std::endl;
    std::cout << "   FU Dynamic Power:                " << fu_dynamic << " mW " << std::endl;
    std::cout << "   FU Total Power:                  " << fu_total_power << " mW " << std::endl;
    std::cout << "   Register Leakage Power:          " << reg_leakage  << " mW " << std::endl;
    std::cout << "   Register Dynamic Power:          " << reg_dynamic << " mW " << std::endl;
    std::cout << "       Total Register Reads:        " << reg_reads << std::endl;
    std::cout << "       Total Register Writes:       " << reg_writes << std::endl;
    std::cout << "   Register Total Power:            " << reg_total_power <<  " mW" << std::endl;
    std::cout << std::endl;
    std::cout << "   Total Power:                     " << total_power << std::endl;
    std::cout << "   ========= Area Analysis ==================" << std::endl;
    std::cout << "   FU Area:                         " << fu_area << " um^2 (" << fu_area/1000000 << " mm^2)" << std::endl;
    std::cout << "   Register Area:                   " << reg_area << " um^2 (" << reg_area/1000000 << " mm^2)" << std::endl;
    std::cout << "   Total Area:                      " << total_area << " um^2 (" << total_area/1000000 << " mm^2)" << std::endl;
}


void
Results::simpleStats() {
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "StatsStart:\n";
    std::cout << "\n" << cycles;
    std::cout << "\n," << runtime;
    std::cout << "\n," << stalls;
    std::cout << "\n," << nodes;
    std::cout << "\n," << run_counter;
    std::cout << "\n," << run_add_sub;
    std::cout << "\n," << run_mul_div;
    std::cout << "\n," << run_shift;
    std::cout << "\n," << run_bit;
    std::cout << "\n," << run_float_add_sub;
    std::cout << "\n," << run_doub_add_sub;
    std::cout << "\n," << run_float_mul_div;
    std::cout << "\n," << run_doub_mul_div;
    std::cout << "\n," << run_zero;
    std::cout << "\n," << run_gep;
    std::cout << "\n," << run_conv;
    std::cout << "\n," << static_counter;
    std::cout << "\n," << static_add_sub;
    std::cout << "\n," << static_mul_div;
    std::cout << "\n," << static_shift;
    std::cout << "\n," << static_bit;
    std::cout << "\n," << static_float_add_sub;
    std::cout << "\n," << static_doub_add_sub;
    std::cout << "\n," << static_float_mul_div;
    std::cout << "\n," << static_doub_mul_div;
    std::cout << "\n," << static_zero;
    std::cout << "\n," << static_gep;
    std::cout << "\n," << static_conv;
    std::cout << "\n," << static_other;
    std::cout << "\n," << reg_total;
    std::cout << "\n," << reg_max_usage;
    std::cout << "\n," << fu_leakage;
    std::cout << "\n," << fu_dynamic;
    std::cout << "\n," << fu_total_power;
    std::cout << "\n," << reg_leakage;
    std::cout << "\n," << reg_dynamic;
    std::cout << "\n," << reg_reads;
    std::cout << "\n," << reg_writes;
    std::cout << "\n," << reg_total_power;
    std::cout << "\n," << total_power;
    std::cout << "\n," << fu_area;
    std::cout << "\n," << reg_area;
    std::cout << "\n," << total_area;
}