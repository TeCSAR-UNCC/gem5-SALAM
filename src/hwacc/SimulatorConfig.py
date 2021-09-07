from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

#Instruction runtime cycles
class SimulatorConfig(SimObject):
    # SimObject type
    type = "SimulatorConfig"

    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/simulator_config.hh"