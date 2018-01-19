from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject
from IOAcc import IOAcc

class LLVMInterface(SimObject):
    type = 'LLVMInterface'
    cxx_header = "hwacc/llvm_interface.hh"

    dummy_acc = Param.IOAcc(Parent.any, "Accelerator dummy to interface to")
    in_file = Param.String("LLVM Trace File")
