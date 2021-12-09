from m5.params import *
from m5.proxy import *
from m5.objects.AbstractMemory import AbstractMemory

class RegisterBank(AbstractMemory):
    type = 'RegisterBank'
    cxx_header = 'hwacc/register_bank.hh.hh'

    reg_ports = VectorResponsePort("Responder ports for private acclerator accesses")
    delta_time = Param.Latency('2ns', "Request to response latency")