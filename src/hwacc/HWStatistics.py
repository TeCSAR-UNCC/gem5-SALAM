from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class HWStatistics(SimObject):
    # SimObject type
    type = 'HWStatistics'
    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/hw_statistics.hh"
    ### --- Do Not Modify Below This Line --- ###
    ### Templates
    ### YML Type: statistics
    ## 'quick_stats' = Param.Bool(quick_stats, "Optimized for Runtime Performance")
    ## 'detailed_stats' = Param.Bool(detailed_stats, "Generate full Runtime Statistics, Impacts Performance")
    ### YML Type: statistics.output_format
    ## 'terminal' = Param.Bool(terminal, "Print Results to Terminal")
    ## 'to_file' = Param.Bool(file, "Print Results to File")
    ## 'to_csv' = Param.Bool(csv, "Print Results in CSV Format")
    ### YML Type: statistics.results
    ## 'runtime' = Param.Bool(runtime, "Simulation Real and CPU Runtime Results")
    ## 'performance' = Param.Bool(performance, "Simulation Cycle Performance Results")
    ## 'power' = Param.Bool(power, "Simulation Power Results")
    ## 'area' = Param.Bool(area, "Simulation Area Results")
    ## 'fu_occupancy' = Param.Bool(occupancy.function_units, "Functional Unit Occupancy Results")
    ## 'runtime_queues' = Param.Bool(occupancy.runtime_queues, "Runtime Queue Occupancy Results")
    ## 'full_trace' = Param.Bool(occupancy.full_trace, "Detailed Occupancy Tracking, Cycle Accurate")
    ## 'params' = Param.Bool(params, "Print All Defined Configurations")
    ## 'inst_usage' = Param.Bool(inst_usage, "Usage count of each Instruction")
    ## 'memory' = Param.Bool(memory, "Memory Usage Results")
    ### -- Code Auto-Generated Below This Line -- ###