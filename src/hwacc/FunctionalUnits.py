from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class FunctionalUnits(SimObject):
    # SimObject type
    type = 'FunctionalUnits'
    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/functional_units.hh"
    ### --- Do Not Modify Below This Line --- ###
    ### Templates
    ### YML Type: functional_unit.parameters
    ## 'fu_type' = Param.String(alias, "Functional Unit Type")
    ## '[datatype]' = Param.String([datatype], "Datatypes Operated on by Functional Unit")
    ## '[instructions]' = Param.String([instructions], "Instructions Operated on by Functional Unit")
    ## 'limit' = Param.UInt32(limit, "Max Concurrent Instances of Functional Unit")
    ### -- Code Auto-Generated Below This Line -- ###

    ## Generated C++ Wrapper Argument List
    #int_adder = Param.Adder(Parent.any, "Integer Adder")

## Generated Instruction SimObjects
# class Adder(SimObject):
#     type = "Adder"
#     cxx_header = "hwacc/HWModeling/generated/functionalunits/adder.hh"
#     alias = Param.String("integer_adder", "Type of functional Unit")
#     stages = Param.UInt64(1, "Runtime stages")
#     enum_type = Param.UInt64(1, "From enumerated functional unit list")
#     cycles = Param.UInt64(1, "Runtime cycle counts")
#     int_type = Param.Bool(True, "Operates on integers")
#     floating_point_type = Param.Bool(False, "Operates on floating")
#     arbitrary_precision = Param.Bool(True, "Can use AP mode")
#     signed_values = Param.Bool(True, "Operates on signed values")
#     unsigned_values = Param.Bool(True, "Operates on unsigned values")