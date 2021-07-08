from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from CycleCounts import CycleCounts

#Cycle Counts
class HWInterface(SimObject):
    type = "HWInterface"
    cxx_header = "hwacc/HWModeling/src/hw_interface.hh"
    
    cycleCounts = Param.CycleCounts("Instruction runtime cycle counts.")
    testParam = Param.UInt32(1234, "Test Parameter.")
