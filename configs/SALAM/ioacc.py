import m5
from m5.objects import *
from m5.util import *


def makeHWAcc(options, system):
    system.test = IOAcc(pio_addr=0x10020000)
    system.test.gic = system.realview.gic
    system.test.mem_side = system.membus.cpu_side_ports
    system.test.pio = system.membus.mem_side_ports
    system.test.int_num = 68