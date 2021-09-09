from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from CycleCounts import CycleCounts
from FunctionalUnits import FunctionalUnits
from HWStatistics import HWStatistics
from InstConfig import InstConfig
from SALAMPowerModel import SALAMPowerModel
from SimulatorConfig import SimulatorConfig
from OpCodes import OpCodes

#Cycle Counts
class HWInterface(SimObject):
    type = "HWInterface"
    cxx_header = "hwacc/HWModeling/src/hw_interface.hh"

    cycle_counts = Param.CycleCounts(Parent.any, "Cycle Counts")
    functional_units = Param.FunctionalUnits(Parent.any, "Functional Units")
    hw_statistics = Param.HWStatistics(Parent.any, "Hardware Statistics")
    inst_config = Param.InstConfig(Parent.any, "Instruction Configuration")
    salam_power_model = Param.SALAMPowerModel(Parent.any, "SALAM Power Model")
    simulator_config = Param.SimulatorConfig(Parent.any, "Simulation Configuration")
    opcodes = Param.OpCodes(Parent.any, "Instruction LLVM OpCode Enumeration to SALAM Type")
    
