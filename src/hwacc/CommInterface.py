from m5.params import *
from m5.proxy import *
from Device import BasicPioDevice
from SimpleMemory import SimpleMemory

class CommInterface(BasicPioDevice):
    type = 'CommInterface'
    cxx_header = 'hwacc/comm_interface.hh'

    flags_size = Param.Addr(0x4, "Size of the address range dedicated to device flags")
    config_size = Param.Addr(0x0, "Size of the addess range dedicated to device configuration")
    pio_size = Param.Addr(0x8, "Size of MMRs. Should be large enough to support flags, config, and global var addresses")
    devicename = Param.String("comm_interface", "Name of comm_interface device")
    dram_side = MasterPort("Memory side port attached to bus containing DRAM")
    spm_side = MasterPort("Memory side port attached to bus containing local scratchpad memory")
    local_range = Param.AddrRange("Address Ranges of DRAM")
    system = Param.System(Parent.any, "Parent system of the device")
    cache_line_size = Param.Unsigned(Parent.cache_line_size, "Cache line size in bytes")
    gic = Param.BaseGic(Parent.any, "Gic on which to trigger interrupts")
    int_num = Param.UInt32(320, "Interrupt number that connects to GIC")
    clock_period = Param.Int(10, "Clock period in ns")

class PrivateMemory(SimpleMemory):
    type = 'PrivateMemory'
    cxx_header = 'hwacc/comm_interface.hh'

    ready_mode = Param.Bool(False, "Use ready mode for private memory")

class CommMemInterface(CommInterface):
    type = 'CommMemInterface'
    cxx_header = 'hwacc/comm_interface.hh'

    private_memory = Param.PrivateMemory("Private scratchpad memory for the device")
    private_range = Param.AddrRange("Address range of private memory")
    private_read_ports = Param.Int("The number of internal Read ports for the private SPM")
    private_write_ports = Param.Int("The number of internal Write ports for the private SPM")
