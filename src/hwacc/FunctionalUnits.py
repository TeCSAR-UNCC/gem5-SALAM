from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class FunctionalUnits(SimObject):
    # SimObject type
    type = 'FunctionalUnits'

    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/functional_units.hh"

    FU_counter = Param.Int32(-1, "Available counter functional units. -1 indicates unlimited resources")
    FU_int_adder = Param.Int32(-1, "Available integer addition/subtraction functional units. -1 indicates unlimited resources")
    FU_int_multiplier = Param.Int32(-1, "Available integer multiply/divide functional units. -1 indicates unlimited resources")
    FU_int_shifter = Param.Int32(-1, "Available integer shifter functional units. -1 indicates unlimited resources")
    FU_int_bit = Param.Int32(-1, "Available integer bitwise functional units. -1 indicates unlimited resources")
    FU_fp_sp_adder = Param.Int32(-1, "Available floating point single precision addition/subtraction functional units. -1 indicates unlimited resources")
    FU_fp_dp_adder = Param.Int32(-1, "Available floating point double precision addition/subtraction functional units. -1 indicates unlimited resources")
    FU_fp_sp_multiplier = Param.Int32(-1, "Available floating point single precision multiply functional units. -1 indicates unlimited resources")
    FU_fp_sp_divider = Param.Int32(-1, "Available floating point single precision divide functional units. -1 indicates unlimited resources")
    FU_fp_dp_multiplier = Param.Int32(-1, "Available floating point double precision multiply functional units. -1 indicates unlimited resources")
    FU_fp_dp_divider = Param.Int32(-1, "Available floating point single precision divide functional units. -1 indicates unlimited resources")
    FU_compare = Param.Int32(-1, "Available comparison functional units. -1 indicates unlimited resources")
    FU_GEP = Param.Int32(-1, "Available equivalent getelementptr functional units. -1 indicates unlimited resources")
    FU_conversion = Param.Int32(-1, "Available type conversion functional units. -1 indicates unlimited resources")
    FU_pipelined = Param.Int32(1, "Sets functional units to operate as pipelined (1) or not pipelined (0)")
    FU_clock_period = Param.Int32(10, "Sets the transitor type used for power calculations")