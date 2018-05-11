import m5
from m5.objects import *
from m5.util import *

def makeHWAcc(options, system):
    system.vadd = CommInterface(pio_addr=0x2f000000, pio_size=25, gic=system.realview.gic)
    system.vadd.pio = system.iobus.master
    system.vadd.flags_size = 1;
    system.vadd.config_size = 0;
    if options.acc_cache:
        system.acc_cache = Cache(assoc=4, tag_latency=4, data_latency=2, response_latency=2, mshrs=4, size='1kB', tgts_per_mshr=20)
        if options.l2cache:
            system.acc_cache.mem_side = system.tol2bus.slave
        else:
            system.acc_cache.mem_side = system.membus.slave
        system.vadd.dram_side = system.acc_cache.cpu_side
    else:
        if options.l2cache:
            system.vadd.dram_side = system.tol2bus.slave
        else:
            system.vadd.dram_side = system.membus.slave
    system.vadd.spm_side = system.iobus.slave
    system.vadd.dram_ranges = [AddrRange(0x80000000, '4GB')]
    system.vadd.llvm_interface = LLVMInterface()
    system.vadd.llvm_interface.in_file = options.accpath + "/vadd/vadd/vadd.ll"
    system.vadd.int_num = 68
    system.vadd.clock_period = 10

    system.vadd.spad = SimpleMemory(range=AddrRange(0x2f000020, size='64kB'),
                                  conf_table_reported=False, latency='2ns')
    system.vadd.spad.port = system.iobus.master
