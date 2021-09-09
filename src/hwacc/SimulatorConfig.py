from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class SimulatorConfig(SimObject):
    # SimObject type
    type = "SimulatorConfig"
    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/simulator_config.hh"
    ### --- Do Not Modify Below This Line --- ###
    ### Templates
    ### YML Type: simulator_config.functional_unit
    ## alias = Param.UInt32(limit, "Max Concurrent Functional Units, 0 = IR Defined")
    ### YML Type: simulator_config
    ## 'output_to_file' = Param.Bool(output_to_file, "Print Terminal Runtime Output to File")
    ### -- Code Auto-Generated Below This Line -- ###
