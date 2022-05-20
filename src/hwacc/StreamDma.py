from m5.params import *
from m5.proxy import *
from m5.objects.Device import DmaDevice

class StreamDma(DmaDevice) :
    type = 'StreamDma'
    cxx_header = 'hwacc/stream_dma.hh'
    devicename = Param.String("stream_dma", "Name of DMA device")
    pio_addr = Param.Addr("Device Address")
    pio_delay = Param.Latency('100ns', "PIO Latency")
    pio_size = Param.Addr(32, "MMR Size")
    stream_in = ResponsePort("Stream buffer access port for S2MM")
    stream_out = ResponsePort("Stream buffer access port for MM2S")
    stream_addr = Param.Addr("Stream interface address")
    stream_size = Param.Addr(8, "Stream width")
    status_in = ResponsePort("Stream buffer status port")
    status_out = ResponsePort("Stream buffer status port")
    status_addr = Param.Addr("Address for accessing buffer status")
    status_size = Param.Addr(4, "Size of the buffer status register")
    mem_delay = Param.Latency('1ns', "Buffer read time")
    read_buffer_size = Param.UInt64(1024, "Read buffer size")
    write_buffer_size = Param.UInt64(1024, "Write buffer size")
    max_pending = Param.Unsigned(8, "Maximum number of pending DMA reads")
    max_req_size = Param.Unsigned(Parent.cache_line_size, "Maximum size of a DMA request")
    gic = Param.BaseGic(Parent.any, "Gic on which to trigger interrupts")
    rd_int = Param.UInt32(210, "Interrupt for read buffer")
    wr_int = Param.UInt32(211, "Interrupt for write buffer")

    bandwidth = Param.MemoryBandwidth('12.6GB/s', "Combined read and write bandwidth")