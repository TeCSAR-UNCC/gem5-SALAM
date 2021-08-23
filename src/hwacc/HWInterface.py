from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from CycleCounts import CycleCounts
#from FunctionalUnits import FunctionalUnits
#from SALAMStatistics import SALAMStatistics
#from RuntimeParams import RuntimeParams

#Cycle Counts
class HWInterface(CycleCounts):
    type = "HWInterface"
    cxx_header = "hwacc/HWModeling/src/hw_interface.hh"
    
    cycleCounts = Param.CycleCounts(Parent.any, "Instruction runtime cycle counts.")
    testParam = Param.UInt32(1234, "Test Parameter.")


#class HardwareInterface(SimObject):
#    type = 'HardwareInterface'
#    cxx_header = "hwacc/hw_interface.hh"
#    cycle_counts = Param.CycleCounts(Parent.any, "Load instruction runtime cycle counts")
