import m5
from m5.objects import *
from m5.util import *

def makeHWAcc(options, system):
    system.acc = CommInterface(pio_addr=0x2f000000, pio_size=64, gic=system.realview.gic)
    system.acc.pio = system.iobus.master
    system.acc.flags_size = 1;
    system.acc.config_size = 0;
    if options.acc_cache:
        system.acc_cache = Cache(assoc=4, tag_latency=1, data_latency=1, response_latency=1, mshrs=16, size='1kB', tgts_per_mshr=20)
#        system.acc_cache.prefetch_on_access = 'true'
#        system.acc_cache.prefetcher = TaggedPrefetcher(degree=8, latency=1)
        if options.l2cache:
            system.acc_cache.mem_side = system.tol2bus.slave
        else:
            system.acc_cache.mem_side = system.membus.slave
        system.acc.dram_side = system.acc_cache.cpu_side
    else:
        if options.l2cache:
            system.acc.dram_side = system.tol2bus.slave
        else:
            system.acc.dram_side = system.membus.slave
    system.acc.spm_side = system.iobus.slave
    system.acc.dram_ranges = [AddrRange(0x80000000, '4GB')]
    system.acc.llvm_interface = LLVMInterface()
    system.acc.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + options.accbench + ".ll"
#    system.acc.llvm_interface.in_file = options.accpath + "/fft/bench/fft.ll"
#    system.acc.llvm_interface.in_file = options.accpath + "/nw/bench/nw.ll"
#    system.acc.llvm_interface.in_file = options.accpath + "/spmv/bench/spmv.ll"
#    system.acc.llvm_interface.in_file = options.accpath + "/gemm/bench/gemm.ll"
#    system.acc.llvm_interface.in_file = options.accpath + "/stencil2d/bench/stencil.ll"
#    system.acc.llvm_interface.in_file = options.accpath + "/stencil3d/bench/stencil.ll"
#    system.acc.llvm_interface.in_file = options.accpath + "/acc/bench/acc.ll"
    system.acc.int_num = 68
    system.acc.clock_period = 10

    system.acc.spad = SimpleMemory(range=AddrRange(0x2f100000, size='128kB'),
                                  conf_table_reported=False, latency='2ns')
    system.acc.spad.port = system.iobus.master

    system.accdma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32)
    system.accdma.pio = system.iobus.master
    system.accdma.dma = system.iobus.slave
