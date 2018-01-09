from m5.params import *
from m5.proxy import *
from Device import BasicPioDevice

class IOAcc(BasicPioDevice):
    type = 'IOAcc'
    cxx_header = 'hwacc/io_acc.hh'
    
    pio_size = Param.Addr(0x8, "Size of address range")
    devicename = Param.String("ioacc", "Name of ioacc device")
    mem_side = MasterPort("Memory side port, sends requests")
    system = Param.System(Parent.any, "Parent system of the device")
    cache_line_size = Param.Unsigned(Parent.cache_line_size, "Cache line size in bytes")
    gic = Param.BaseGic(Parent.any, "Gic on which to trigger interrupts")
    int_num = Param.UInt32(320, "Interrupt number that connects to GIC")
    process_delay = Param.Int(10, "Clock cycles it takes to finish executing")
