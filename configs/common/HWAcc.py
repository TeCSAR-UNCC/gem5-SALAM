import m5
from m5.objects import *
from m5.util import *

def makeHWAcc(options, system):
    system.vadd = CommInterface(pio_addr=0x2f000000, pio_size=25, gic=system.realview.gic)
    system.vadd.pio = system.iobus.master
    system.vadd.flags_size = 1;
    system.vadd.config_size = 0;
    system.vadd.mem_side = system.iobus.slave
#    system.l2cache = Cache(assoc=8, tag_latency=20, data_latency=20, response_latency=20, mshrs=20, size='1kB', tgts_per_mshr=12)
#    system.l2cache.mem_side = system.iobus.slave
#    system.vadd.mem_side = system.l2cache.cpu_side
    system.vadd.llvm_interface = LLVMInterface()
    system.vadd.llvm_interface.in_file = "/home/josh/gem5Work/src/hwacc/LLVMRead/Benchmarks/vadd/vadd/vadd.ll"
    system.vadd.int_num = 68
    system.vadd.clock_period = 10

    system.vadd.spad = SimpleMemory(range=AddrRange(0x2f000020, size='4kB'),
                                  conf_table_reported=False, latency='1ns')
    system.vadd.spad.port = system.iobus.master
