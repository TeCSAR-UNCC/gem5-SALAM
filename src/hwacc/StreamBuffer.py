from m5.params import *
from m5.proxy import *
from m5.objects.ClockedObject import ClockedObject
# from Device import BasicPioDevice

class StreamBuffer(ClockedObject):
    type = 'StreamBuffer'
    cxx_header = 'hwacc/stream_buffer.hh'
    system = Param.System(Parent.any, "System this devices is part of")
    stream_in = ResponsePort("Stream buffer access port for pushing to stream")
    stream_out = ResponsePort("Stream buffer access port for pulling from stream")
    status_in = ResponsePort("Stream buffer status port")
    status_out = ResponsePort("Stream buffer status port")
    buffer_size = Param.UInt64(256, "Stream buffer depth in bytes")
    stream_address = Param.Addr("Address for accessing stream data")
    stream_size = Param.Addr("Stream buffer width in bytes")
    status_address = Param.Addr("Address for accessing buffer status")
    status_size = Param.Addr(4, "Size of the buffer status register")
    stream_latency = Param.Latency('1ns', 'Stream W/R latency')
    bandwidth = Param.MemoryBandwidth('12.6GB/s', "Combined read and write bandwidth")