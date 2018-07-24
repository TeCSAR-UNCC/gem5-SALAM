from m5.params import *
from m5.proxy import *
from Device import DmaDevice

class NoncoherentDma(DmaDevice) :
    type = 'NoncoherentDma'
    cxx_header = 'hwacc/noncoherent_dma.hh'
    devicename = Param.String("noncoherent_dma", "Name of DMA device")
    pio_addr = Param.Addr("Device Address")
    pio_delay = Param.Latency('100ns', "PIO Latency")
    pio_size = Param.Addr("MMR Size")
    buffer_size = Param.UInt64(1024, "Read buffer size")
    max_pending = Param.Unsigned(8, "Maximum number of pending DMA reads")
    max_req_size = Param.Unsigned(Parent.cache_line_size, "Maximum size of a DMA request")
    gic = Param.BaseGic(Parent.any, "Gic on which to trigger interrupts")
    int_num = Param.UInt32(200, "Interrupt number that connects to GIC")
