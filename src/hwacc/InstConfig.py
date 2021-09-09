from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class InstConfig(SimObject):
    # SimObject type
    type = 'InstConfig'
    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/instruction_config.hh"
    ### --- Do Not Modify Below This Line --- ###
    ### Templates
    ## YML Type: instruction
    ## 'llvm_opcodenum' = Param.UInt32(LLVM_OpCodeNum, "LLVM OpCode Enumerated Value")
    ## 'llvm_opcodename' = Param.String(LLVM_OpCodeName, "LLVM OpCode Name")
    ## 'salam_opcode' = Param.String(SALAM_OpCode, "SALAM OpCode Name")
    ## 'runtime_cycles' = Param.UInt32(runtime_cycles, "Instruction Runtime Cycles")
    ## 'stages' = Param.UInt32(stages, "Instruction Functional Unit Stage Count")
    ## '[stage_cycles]' = Param.UInt32([stage_cycles], "Cycles in the Current Stage")
    ## 'functional_unit_limit' = Param.UInt32(functional_unit_limit, "Max Concurrent Functional Unit Scheduling")
    ### -- Code Auto-Generated Below This Line -- ###