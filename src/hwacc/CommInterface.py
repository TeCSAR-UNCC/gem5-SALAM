from m5.params import *
from m5.proxy import *
from Device import BasicPioDevice
from AbstractMemory import AbstractMemory

class CommInterface(BasicPioDevice):
    type = 'CommInterface'
    cxx_header = 'hwacc/comm_interface.hh'

    flags_size = Param.Addr(0x4, "Size of the address range dedicated to device flags")
    config_size = Param.Addr(0x0, "Size of the addess range dedicated to device configuration")
    pio_size = Param.Addr(0x8, "Size of MMRs. Should be large enough to support flags, config, and global var addresses")
    devicename = Param.String("comm_interface", "Name of comm_interface device")
    local = VectorMasterPort("Master points connected to the local cluster xbar")
    acp = VectorMasterPort("Master ports connected to the cluster coherency xbar")
    stream = VectorMasterPort("Master ports connected to streaming devices")
    spm = VectorMasterPort("Master ports connected to private scratchpad memory")
    system = Param.System(Parent.any, "Parent system of the device")
    cache_line_size = Param.Unsigned(Parent.cache_line_size, "Cache line size in bytes")
    gic = Param.BaseGic(Parent.any, "Gic on which to trigger interrupts")
    int_num = Param.Int32(-1, "Interrupt number that connects to GIC")
    clock_period = Param.Int(10, "Clock period in ns")
    premap_data = Param.Bool(False, "Whether or not the memory read/write locations for data predefined")
    data_bases = VectorParam.Addr([0x0], "Base addresses for data if they are predefined")
    enable_debug_msgs = Param.Bool(False, "Whether or not this device will display debug messages")