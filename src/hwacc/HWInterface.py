from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from CycleCounts import CycleCounts
from FunctionalUnits import FunctionalUnits
from SALAMStatistics import SALAMStatistics
from RuntimeParams import RuntimeParams

class HardwareInterface(SimObject):
    type = 'HardwareInterface'
    cxx_header = "hwacc/hw_interface.hh"

    cycle_counts = Param.CycleCounts(Parent.any, "Load instruction runtime cycle counts")