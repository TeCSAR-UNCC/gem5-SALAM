from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

#Cycle Counts
class HWInterface(SimObject):

    type = "HWInterface"
    cxx_header = "hwacc/HWModeling/src/hw_interface.hh"
    
