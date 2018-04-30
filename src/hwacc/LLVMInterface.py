from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from ComputeUnit import ComputeUnit

class LLVMInterface(ComputeUnit):
    type = 'LLVMInterface'
    cxx_header = "hwacc/llvm_interface.hh"

    in_file = Param.String("LLVM Trace File")
    proc_elem = Param.UInt32(1, "The number of processing elements available in the device")
