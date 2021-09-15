from m5.params import *
from m5.proxy import *
from m5.objects.AbstractMemory import AbstractMemory

class ScratchpadMemory(AbstractMemory):
    type = 'ScratchpadMemory'
    cxx_header = 'hwacc/scratchpad_memory.hh'

    port = ResponsePort("Generic slave port")
    spm_ports = VectorResponsePort("Slave ports for private acclerator SPM accesses")
    latency = Param.Latency('2ns', "Request to response latency")
    latency_var = Param.Latency('0ns', "Request to response latency variance")
    ready_mode = Param.Bool(False, "Use ready mode for scratchpad memory")
    read_on_invalid = Param.Bool(False, "Enable reads on invalid memory segments when ready mode is used")
    write_on_valid = Param.Bool(True, "Enable writes on valid memory sectors when ready mode is used")
    reset_on_scratchpad_read = Param.Bool(True, "Reset ready bit on private scratchpad memory read")
    bandwidth = Param.MemoryBandwidth('12GB/s', "Combined read and write bandwidth per port")