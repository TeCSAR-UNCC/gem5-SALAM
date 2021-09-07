from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class HWStatistics(SimObject):
    # SimObject type
    type = 'HWStatistics'

    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/hw_statistics.hh"