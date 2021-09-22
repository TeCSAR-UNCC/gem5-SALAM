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

    ## Generated C++ Wrapper Argument List
    add_config = Param.Add(Parent.any, "Add config")

    
## Generated Instruction SimObjects
class Add(SimObject):
    type = "Add"
    cxx_header = "hwacc/HWModeling/src/instruction_config.hh"
    llvm_opcode_name = Param.String("Add", "LLVM Opcode Name")
    llvm_opcode_num = Param.UInt64(13, "Value from IR enumeration in LLVM Source")
    salam_opcode = Param.String("Add", "Salam OpCode Name")
    runtime_cycles = Param.UInt64(1, "Cycles to Complete Computation")
    stages = Param.UInt64(1, "Functional Unit Stages")
    functional_unit_limit = Param.UInt64(0, "Default is IR limited")
    functional_unit = Param.UInt64(1, "Enumeration of Available Functional Units")