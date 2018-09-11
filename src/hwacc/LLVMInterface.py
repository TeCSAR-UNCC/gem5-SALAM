from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from ComputeUnit import ComputeUnit

class LLVMInterface(ComputeUnit):
    type = 'LLVMInterface'
    cxx_header = "hwacc/llvm_interface.hh"

    in_file = Param.String("LLVM Trace File")
    sched_threshold = Param.UInt32(1000, "Scheduling window threshold. Prevents scheduling windows size from exploding during regions of high loop parallelism")
