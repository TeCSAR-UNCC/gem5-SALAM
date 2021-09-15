from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from m5.objects.CommInterface import CommInterface
from m5.objects.HWInterface import HWInterface

class ComputeUnit(SimObject):
    type = 'ComputeUnit'
    cxx_header = "hwacc/compute_unit.hh"

    comm_int = Param.CommInterface(Parent.any, "Communication interface to connect to")
    hw_int = Param.HWInterface(Parent.any, "Hardware model interface to connect to")
    