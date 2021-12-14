from m5.params import *
from m5.proxy import *
from m5.objects.AbstractMemory import AbstractMemory

class RegisterBank(AbstractMemory):
    type = 'RegisterBank'
    cxx_header = 'hwacc/register_bank.hh'

    reg_port = ResponsePort("Responder port for private acclerator accesses")
    delta_time = Param.Latency('10ns', "Request to response latency")