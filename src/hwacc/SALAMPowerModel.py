from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class SALAMPowerModel(SimObject):
    # SimObject type
    type = "SALAMPowerModel"
    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/salam_power_model.hh"
    ### --- Do Not Modify Below This Line --- ###
    ### Templates
    ### YML Type: functional_unit.power_model
    ## 'power_units' = Param.String(units.power, "Hardware Model Power Units")
    ## 'energy_units' = Param.String(units.energy, "Hardware Model Energy Units")
    ## 'time_units' = Param.String(units.time, "Hardware Model Time Units")
    ## 'area_units' = Param.String(units.area, "Hardware Model Area Units")
    ## 'latency' = Params.UInt32(latency, "Hardware Model Functional Unit Latency")
    ## 'internal_power' = Params.Double(internal_power, "Measured Power Metric")
    ## 'switch_power' = Params.Double(switch_power, "Measured Power Metric")
    ## 'dynamic_power' = Params.Double(dynamic_power, "Measured Power Metric")
    ## 'dynamic_energy' = Params.Double(dynamic_energy, "Measured Energy Metric")
    ## 'leakage_power' = Params.Double(leakage_power, "Measured Power Metric")
    ## 'area' = Params.Double(area, "Measure Area Metric")
    ## 'path_delay' = Params.Double(path_delay, "Measured Path Delay Metric")
    ### -- Code Auto-Generated Below This Line -- ###
