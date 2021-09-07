from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

#Instruction runtime cycles
class SALAMPowerModel(SimObject):

    # SimObject type
    type = "SALAMPowerModel"

    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/salam_power_model.hh"