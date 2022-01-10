#include "hw_statistics.hh"

HWStatistics::HWStatistics(const HWStatisticsParams &params) :
    SimObject(params) { 
        
        statBufferSize = 10000;
        statBufferPreDefine = 2;
        dbg = false;

        for (int i=0 ; i<statBufferPreDefine; i++ ) {
            std::vector<HW_Cycle_Stats> hw_cycle_buffer;
            hw_cycle_buffer.reserve(statBufferSize);
            hw_buffer_list.push_back(hw_cycle_buffer);
        }
        hw_buffer = hw_buffer_list.begin();
        cycle_buffer = hw_buffer->begin();
        clearStats();
    }


void
HWStatistics::updateHWStatsCycleStart() {
    if (dbg) DPRINTF(SALAM_Debug, "Updating Cycle Statistics Buffer\n");
    (*hw_buffer).insert(cycle_buffer, current_cycle_stats);
    clearStats();
    updateBuffer();
    
}

void
HWStatistics::clearStats() {
    if (dbg) DPRINTF(SALAM_Debug, "Clearing Cycle Statistics\n");
    current_cycle_stats.reset();
    
}

void
HWStatistics::updateHWStatsCycleEnd(int curr_cycle) {
    if (dbg) DPRINTF(SALAM_Debug, "Updating Cycle Statistics\n");
    current_cycle_stats.cycle = curr_cycle;

}

void
HWStatistics::updateBuffer() {
    if (dbg) DPRINTF(SALAM_Debug, "Checking Buffer[%i][%i]\n", current_buffer_index, hw_buffer_list.at(current_buffer_index).size());
    if (hw_buffer_list.at(current_buffer_index).size() == statBufferSize) {
        current_buffer_index++;
        if(current_buffer_index == statBufferPreDefine) {
            if (dbg) DPRINTF(SALAM_Debug, "Creating New Buffer Window\n");
            std::vector<HW_Cycle_Stats> hw_cycle_buffer;
            hw_cycle_buffer.reserve(statBufferSize);
            hw_buffer_list.push_back(hw_cycle_buffer);
            hw_buffer = hw_buffer_list.end();
            cycle_buffer = hw_buffer->begin();
        } else {
            if (dbg) DPRINTF(SALAM_Debug, "Next Buffer Window\n");
            hw_buffer++;
            cycle_buffer = hw_buffer->begin();
        }
    } else {
        cycle_buffer = hw_buffer->end();
    }
}


void
HWStatistics::print() {
    if (dbg) DPRINTF(SALAM_Debug," Buffers: %i\n", (current_buffer_index + 1));
    for (auto buffers : hw_buffer_list) {
        for (auto cycles : buffers) {
            // This loops through the full runtime, starting at cycle 1 to completion
            //std::cout << " Cycle: " << cycles.cycle;
        }
    }


/*
    std::cout << "********************************************************************************" << std::endl;
    std::cout << "   ========= Performance Analysis =================" << std::endl;
    std::cout << "   Setup Time:                      " << setupTime << "ns" << std::endl;
    std::cout << "   Simulation Time:                 " << simTime << "ns" << std::endl;
    std::cout << "   System Clock:                    " << sys_clock << "GHz" << std::endl;
    std::cout << "   Transistor Latency:              " << fu_clock_period << "ns" << std::endl;
    std::cout << "   Runtime:                         " << cycles << " cycles" << std::endl;
    std::cout << "   Runtime:                         " << runtime << " seconds" << std::endl;
    std::cout << "   Stalls:                          " << stalls << " cycles" << std::endl;
    std::cout << "       Load Only:                   " << loadOnlyStall << " cycles" << std::endl;
    std::cout << "       Store Only:                  " << storeOnlyStall << " cycles" << std::endl;
    std::cout << "       Compute Only:                " << compOnlyStall << " cycles" << std::endl;
    std::cout << "       Compute & Store:             " << storeCompStall << " cycles" << std::endl;
    std::cout << "       Load & Store:                " << loadStoreStall << " cycles" << std::endl;
    std::cout << "       Load & Compute:              " << loadCompStall << " cycles" << std::endl;
    std::cout << "       Load & Compute & Store:      " << loadStoreCompStall << " cycles" << std::endl;
    std::cout << "   Executed Nodes:                  " << nodes << " cycles" << std::endl;
    std::cout << "       Load Only:                   " << loadOnly << " cycles" << std::endl;
    std::cout << "       Store Only:                  " << storeOnly << " cycles" << std::endl;
    std::cout << "       Compute Only:                " << compOnly << " cycles" << std::endl;
    std::cout << "       Compute & Store:             " << storeComp << " cycles" << std::endl;
    std::cout << "       Load & Store:                " << loadStore << " cycles" << std::endl;
    std::cout << "       Load & Compute:              " << loadComp << " cycles" << std::endl;
    std::cout << "       Load & Compute & Store:      " << loadStoreComp << " cycles" << std::endl;
    std::cout << std::fixed << std::setprecision(6) << std::endl;
    std::cout << "   ========= Runtime FU's ========= (Max | Avg) ===" << std::endl;
    std::cout << "   Counter FU's:                    " << std::setw(4) << run_counter << " | " << occ_counter << std::endl;
    std::cout << "   Integer Add/Sub FU's:            " << std::setw(4) << run_add_sub << " | " << occ_add_sub << std::endl;
    std::cout << "   Integer Mul/Div FU's:            " << std::setw(4) << run_mul_div << " | " << occ_mul_div << std::endl;
    std::cout << "   Integer Shifter FU's:            " << std::setw(4) << run_shift << " | " << occ_shift << std::endl;
    std::cout << "   Integer Bitwise FU's:            " << std::setw(4) << run_bit << " | " << occ_bit << std::endl;
    std::cout << "   Floating Point Float Add/Sub:    " << std::setw(4) << run_float_add_sub << " | " << occ_float_add_sub << std::endl;
    std::cout << "   Floating Point Double Add/Sub:   " << std::setw(4) << run_doub_add_sub << " | " << occ_doub_add_sub << std::endl;
    std::cout << "   Floating Point Float Mul/Div:    " << std::setw(4) << run_float_mul_div << " | " << occ_float_mul_div << std::endl;
    std::cout << "   Floating Point Double Mul/Div:   " << std::setw(4) << run_doub_mul_div << " | " << occ_doub_mul_div << std::endl;
    std::cout << "   0 Cycle Compare FU's:            " << std::setw(4) << run_zero << " | " << occ_zero << std::endl;
    std::cout << "   GEP Instruction FU's:            " << std::setw(4) << run_gep << " | " << occ_gep << std::endl;
    std::cout << "   Type Conversion FU's:            " << std::setw(4) << run_conv << " | " << occ_conv << std::endl;
    std::cout << std::endl;
    std::cout << "   ========= Static FU's =========================="  << std::endl;
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
    std::cout << std::endl;
    std::cout << "   ========= Pipeline Register Usage =============" << std::endl;
    std::cout << "   Total Number of Registers:       " << reg_total << std::endl;
    std::cout << "   Max Register Usage Per Cycle:    " << reg_max_usage << std::endl;
    std::cout << "   Avg Register Usage Per Cycle:    " << reg_avg_usage << std::endl;
    std::cout << "   Avg Register Size (Bytes):       " << reg_avg_size << std::endl;
    std::cout << std::endl;
    std::cout << "   ========= Memory Configuration =================" << std::endl;
    std::cout << "   Cache Bus Ports:                 " << cache_ports << std::endl;
    std::cout << "   Shared Cache Size:               " << cache_size << "kB" << std::endl;
    std::cout << "   Local Bus Ports:                 " << local_ports << std::endl;
    std::cout << "   Private SPM Size:                " << spm_size << "kB" << std::endl;
    std::cout << "   Private Read Ports:              " << read_ports << std::endl;
    std::cout << "   Private Write Ports:             " << write_ports << std::endl;
    std::cout << "   Private Read Bus Width:          " << read_bus_width << std::endl;
    std::cout << "   Private Write Bus Width:         " << write_bus_width << std::endl;
    std::cout << "       Memory Reads:                " << dma_reads << std::endl;
    std::cout << "       Memory Writes:               " << dma_writes << std::endl;    
    std::cout << "   ========= Power Analysis ======================" << std::endl;
    std::cout << "   FU Leakage Power:                " << fu_leakage << " mW " << std::endl;
    std::cout << "   FU Dynamic Power:                " << fu_dynamic << " mW " << std::endl;
    std::cout << "   FU Total Power:                  " << fu_total_power << " mW " << std::endl;
    std::cout << std::endl;
    std::cout << "   Registers Leakage Power:          " << reg_leakage  << " mW " << std::endl;
    std::cout << "   Registers Dynamic Power:          " << reg_dynamic << " mW " << std::endl;
    std::cout << "       Register Reads (Bits):        " << reg_reads << std::endl;
    std::cout << "       Register Writes (Bits):       " << reg_writes << std::endl;
    std::cout << "   Registers Total Power:            " << reg_total_power <<  " mW" << std::endl;
    std::cout << std::endl;
    std::cout << "   SPM Leakage Power:               " << spm_leakage << " mW" << std::endl;
    std::cout << "   SPM Read Dynamic Power:          " << spm_read_dynamic << " mW" << std::endl;
    std::cout << "   SPM Write Dynamic Power:         " << spm_write_dynamic << " mW" << std::endl;
    std::cout << "   SPM Total Power:                 " << spm_total_power << " mW" << std::endl;
    std::cout << std::endl;
    std::cout << "   Cache Leakage Power:             " << cache_leakage << " mW" << std::endl;
    std::cout << "   Cache Read Dynamic Power:        " << cache_read_dynamic << " mW" << std::endl;
    std::cout << "   Cache Write Dynamic Power:       " << cache_write_dynamic << " mW" << std::endl;
    std::cout << "   Cache Total Power:               " << cache_total_power << " mW" << std::endl;
    std::cout << std::endl;
    std::cout << "   Accelerator Power:               " << total_power << " mW" << std::endl;
    std::cout << "   Accelerator Power (SPM):         " << acc_spm_total_power << " mW" << std::endl;
    std::cout << "   Accelerator Power (Cache):       " << acc_cache_total_power << " mW" << std::endl;
    std::cout << std::endl;
    std::cout << "   ========= Area Analysis =======================" << std::endl;
    std::cout << "   FU Area:                         " << fu_area << " um^2 (" << fu_area/1000000 << " mm^2)" << std::endl;
    std::cout << "   Register Area:                   " << reg_area << " um^2 (" << reg_area/1000000 << " mm^2)" << std::endl;
    std::cout << "   SPM Area:                        " << spm_area << " um^2 (" << spm_area/1000000 << " mm^2)" << std::endl;
    std::cout << "   Cache Area:                      " << cache_area << " um^2 (" << cache_area/1000000 << " mm^2)" << std::endl; 
    std::cout << std::endl;
    std::cout << "   Accelerator Area:                " << total_area << " um^2 (" << total_area/1000000 << " mm^2)" << std::endl;
    std::cout << "   Accelerator Area (SPM):          " << acc_spm_total_area << " um^2 (" << acc_spm_total_area/1000000 << " mm^2)" << std::endl;
    std::cout << "   Accelerator Area (Cache):        " << acc_cache_total_area << " um^2 (" << acc_cache_total_area/1000000 << " mm^2)" << std::endl;
    std::cout << std::endl;
    std::cout << "   ========= SPM Resizing  =======================" << std::endl;
    std::cout << "   SPM Optimized Leakage Power:     " << spm_opt_leakage << " mW" << std::endl;
    std::cout << "   SPM Opt Area:                    " << spm_opt_area << " um^2" << std::endl;
    std::cout << std::endl;
*/

}


void
HWStatistics::simpleStats() {
    /*
    std::cout << std::fixed << std::setprecision(6) << std::endl;
    std::cout << "StatsStart:";
    std::cout << "\n" << setupTime;
    std::cout << ",\n" << simTime;
    std::cout << ",\n" << sys_clock;
    std::cout << ",\n" << fu_clock_period;
    std::cout << ",\n" << cycles;
    std::cout << ",\n" << runtime;
    std::cout << ",\n" << stalls;
    std::cout << ",\n" << loadOnlyStall;
    std::cout << ",\n" << storeOnlyStall;
    std::cout << ",\n" << compOnlyStall;
    std::cout << ",\n" << storeCompStall;
    std::cout << ",\n" << loadStoreStall;
    std::cout << ",\n" << loadCompStall;
    std::cout << ",\n" << loadStoreCompStall;
    std::cout << ",\n" << nodes;
    std::cout << ",\n" << loadOnly;
    std::cout << ",\n" << storeOnly;
    std::cout << ",\n" << compOnly;
    std::cout << ",\n" << storeComp;
    std::cout << ",\n" << loadStore;
    std::cout << ",\n" << loadComp;
    std::cout << ",\n" << loadStoreComp;
    std::cout << ",\n" << run_counter;
    std::cout << ",\n" << occ_counter;
    std::cout << ",\n" << run_add_sub;
    std::cout << ",\n" << occ_add_sub;
    std::cout << ",\n" << run_mul_div;
    std::cout << ",\n" << occ_mul_div;
    std::cout << ",\n" << run_shift;
    std::cout << ",\n" << occ_shift;
    std::cout << ",\n" << run_bit;
    std::cout << ",\n" << occ_bit;
    std::cout << ",\n" << run_float_add_sub;
    std::cout << ",\n" << occ_float_add_sub;
    std::cout << ",\n" << run_doub_add_sub;
    std::cout << ",\n" << occ_doub_add_sub;
    std::cout << ",\n" << run_float_mul_div;
    std::cout << ",\n" << occ_float_mul_div;
    std::cout << ",\n" << run_doub_mul_div;
    std::cout << ",\n" << occ_doub_mul_div;
    std::cout << ",\n" << run_zero;
    std::cout << ",\n" << occ_zero;
    std::cout << ",\n" << run_gep;
    std::cout << ",\n" << occ_gep;
    std::cout << ",\n" << run_conv;
    std::cout << ",\n" << occ_conv;
    std::cout << ",\n" << static_counter;
    std::cout << ",\n" << static_add_sub;
    std::cout << ",\n" << static_mul_div;
    std::cout << ",\n" << static_shift;
    std::cout << ",\n" << static_bit;
    std::cout << ",\n" << static_float_add_sub;
    std::cout << ",\n" << static_doub_add_sub;
    std::cout << ",\n" << static_float_mul_div;
    std::cout << ",\n" << static_doub_mul_div;
    std::cout << ",\n" << static_zero;
    std::cout << ",\n" << static_gep;
    std::cout << ",\n" << static_conv;
    std::cout << ",\n" << static_other;
    std::cout << ",\n" << reg_total;
    std::cout << ",\n" << reg_max_usage;
    std::cout << ",\n" << reg_avg_usage;
    std::cout << ",\n" << reg_avg_size;
    std::cout << ",\n" << cache_ports;
    std::cout << ",\n" << cache_size;
    std::cout << ",\n" << local_ports;
    std::cout << ",\n" << spm_size;
    std::cout << ",\n" << read_ports;
    std::cout << ",\n" << write_ports;
    std::cout << ",\n" << read_bus_width;
    std::cout << ",\n" << write_bus_width;
    std::cout << ",\n" << dma_reads;
    std::cout << ",\n" << dma_writes;
    std::cout << ",\n" << fu_leakage;
    std::cout << ",\n" << fu_dynamic;
    std::cout << ",\n" << fu_total_power;
    std::cout << ",\n" << reg_leakage;
    std::cout << ",\n" << reg_dynamic;
    std::cout << ",\n" << reg_reads;
    std::cout << ",\n" << reg_writes;
    std::cout << ",\n" << reg_total_power;
    std::cout << ",\n" << spm_leakage;
    std::cout << ",\n" << spm_read_dynamic;
    std::cout << ",\n" << spm_write_dynamic;
    std::cout << ",\n" << spm_total_power;
    std::cout << ",\n" << cache_leakage;
    std::cout << ",\n" << cache_read_dynamic;
    std::cout << ",\n" << cache_write_dynamic;
    std::cout << ",\n" << cache_total_power;
    std::cout << ",\n" << total_power;
    std::cout << ",\n" << acc_spm_total_power;
    std::cout << ",\n" << acc_cache_total_power;
    std::cout << ",\n" << fu_area;
    std::cout << ",\n" << reg_area;
    std::cout << ",\n" << spm_area;
    std::cout << ",\n" << cache_area;
    std::cout << ",\n" << total_area;
    std::cout << ",\n" << acc_spm_total_area;
    std::cout << ",\n" << acc_cache_total_area;
    std::cout << ",\n" << spm_opt_leakage;
    std::cout << ",\n" << spm_opt_area;
    std::cout << "\nStatsEnd:\n";
    */
}

void
HWStatistics::unitCorrections() {
    /*
    sys_clock = 1.0/(clock_period/1000);
    cache_size = cache_size/1024;
    spm_size = spm_size/1024;
    spm_total_power = spm_leakage+spm_read_dynamic+spm_write_dynamic;
    cache_total_power = cache_leakage + cache_read_dynamic + cache_write_dynamic;
    acc_spm_total_power = total_power + spm_total_power;
    acc_cache_total_power = total_power + cache_total_power;
    spm_area = spm_area/1000;
    cache_area = cache_area/1000;
    spm_opt_area = spm_opt_area/1000;
    acc_spm_total_area = total_area + spm_area;
    acc_cache_total_area = total_area + cache_area;
    */
}