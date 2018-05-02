import m5
from m5.objects import *
from m5.util import *

def makeHWAcc(options, system):
    system.vadd = CommInterface(pio_addr=0x2f000000, pio_size=25, gic=system.realview.gic)
    system.vadd.pio = system.iobus.master
    system.vadd.flags_size = 1;
    system.vadd.config_size = 0;
#    system.vadd.mem_side0 = system.iobus.slave
#    system.vadd.mem_side1 = system.iobus.slave
#    system.vadd.mem_side2 = system.iobus.slave
#    system.vadd.mem_side3 = system.iobus.slave
    if options.acc_cache:
        system.acc_cache = Cache(assoc=8, tag_latency=20, data_latency=20, response_latency=20, mshrs=20, size='1kB', tgts_per_mshr=12)
        system.acc_cache.mem_side = system.membus.slave
        system.vadd.dram_side = system.acc_cache.cpu_side
    else:
        system.vadd.dram_side = system.membus.slave
    system.vadd.spm_side = system.iobus.slave
    system.vadd.dram_ranges = [AddrRange(0x80000000, '4GB')]
    #system.l2cache = Cache(assoc=8, tag_latency=20, data_latency=20, response_latency=20, mshrs=20, size='1kB', tgts_per_mshr=12)
    #system.l2cache.mem_side = system.membus.slave
    #system.vadd.dram_side = system.l2cache.cpu_side
    system.vadd.llvm_interface = LLVMInterface()
    system.vadd.llvm_interface.in_file = options.accpath + "/vadd/vadd/vadd.ll"
    system.vadd.int_num = 68
    system.vadd.clock_period = 10

    system.vadd.spad = SimpleMemory(range=AddrRange(0x2f000020, size='4kB'),
                                  conf_table_reported=False, latency='10ns')
    system.vadd.spad.port = system.iobus.master
