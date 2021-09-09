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

    