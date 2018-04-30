import m5
from m5.objects import *
from m5.util import *

def makeHWAcc(options, system):
    system.vadd = CommInterface(pio_addr=0x2f000000, pio_size=25, gic=system.realview.gic)
    system.vadd.pio = system.iobus.master
    system.vadd.flags_size = 1;
    system.vadd.config_size = 0;
    system.vadd.mem_side = system.iobus.slave
    system.vadd.llvm_interface = LLVMInterface()
    system.vadd.llvm_interface.in_file = "/home/samerogers/gem5/src/hwacc/LLVMRead/Benchmarks/vadd/vadd/vadd.ll"
    system.vadd.int_num = 320
    system.vadd.clock_period = 10
