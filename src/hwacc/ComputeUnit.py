from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from CommInterface import CommInterface

class ComputeUnit(SimObject):
    type = 'ComputeUnit'
    cxx_header = "hwacc/compute_unit.hh"

    comm_int = Param.CommInterface(Parent.any, "Communication interface to connect to")
