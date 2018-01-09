from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from IOAcc import IOAcc

class CompInterface(SimObject):
    type = 'CompInterface'
    cxx_header = "hwacc/comp_interface.hh"
    
    dummy_acc = Param.IOAcc(Parent.any, "Accelerator dummy to interface to")
