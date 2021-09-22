from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from m5.objects.CycleCounts import CycleCounts
from m5.objects.FunctionalUnits import FunctionalUnits
from m5.objects.HWStatistics import HWStatistics
from m5.objects.InstConfig import InstConfig
from m5.objects.SALAMPowerModel import SALAMPowerModel
from m5.objects.SimulatorConfig import SimulatorConfig
from m5.objects.InstOpCodes import InstOpCodes

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
    opcodes = Param.InstOpCodes(Parent.any, "Instruction LLVM OpCode Enumeration to SALAM Type")
    
