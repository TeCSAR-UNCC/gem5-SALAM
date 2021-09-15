from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from m5.objects.ComputeUnit import ComputeUnit

class LLVMInterface(ComputeUnit):
    type = 'LLVMInterface'
    cxx_header = "hwacc/llvm_interface.hh"
    
    in_file = Param.String("LLVM Trace File")
    lockstep_mode = Param.Bool(True, "TRUE: Stall datapath if any operation stalls. FALSE: Only stall datapath regions with stalls")
    sched_threshold = Param.UInt32(10000, "Scheduling window threshold. Prevents scheduling windows size from exploding during regions of high loop parallelism")
    clock_period = Param.Int32(10, "System clock speed")
    top_name = Param.String("top", "Name of the top-level function for the accelerator")