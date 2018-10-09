import m5
from m5.objects import *
from m5.util import *

def makeHWAcc(options, system):
    system.acc_cluster = AccCluster()
    system.acc_cluster.bus = NoncoherentXBar(width=16, frontend_latency=1, forward_latency=0, response_latency=1)
    system.acc_cluster.acc = CommInterface(pio_addr=0x2f000000, pio_size=64, gic=system.realview.gic)
    system.acc_cluster.acc.pio = system.acc_cluster.bus.master
    system.acc_cluster.acc.flags_size = 1
    system.acc_cluster.acc.config_size = 0
    system.acc_cluster.cache_bus = NoncoherentXBar(width=16, frontend_latency=1, forward_latency=0, response_latency=1)
    system.acc_cluster.acc.dram_side = system.acc_cluster.cache_bus.slave
    if options.acc_cache:
        system.acc_cluster_cache = Cache(assoc=4, tag_latency=1, data_latency=1, response_latency=1, mshrs=16, size='1kB', tgts_per_mshr=20)
#        system.acc_cluster_cache.prefetch_on_access = 'true'
#        system.acc_cluster_cache.prefetcher = TaggedPrefetcher(degree=8, latency=1)
        if options.l2cache:
            system.acc_cluster_cache.mem_side = system.tol2bus.slave
        else:
            system.acc_cluster_cache.mem_side = system.membus.slave
        system.acc_cluster.cache_bus.master = system.acc_cluster_cache.cpu_side
    else:
        if options.l2cache:
            system.acc_cluster.cache_bus.master = system.tol2bus.slave
        else:
            system.acc_cluster.cache_bus.master = system.membus.slave
    system.acc_cluster.acc.spm_side = system.acc_cluster.bus.slave
    system.acc_cluster.acc.dram_ranges = [AddrRange(0x80000000, '4GB')]
    system.acc_cluster.llvm_interface = LLVMInterface()
    system.acc_cluster.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + options.accbench + ".ll"


    system.acc_cluster.acc.int_num = 68
    system.acc_cluster.acc.clock_period = 10

    system.mem_to_hwacc = Bridge(delay='50ns')
    system.mem_to_hwacc.ranges = AddrRange(0x2f000000, 0x7fffffff)
    system.mem_to_hwacc.master = system.acc_cluster.bus.slave
    system.mem_to_hwacc.slave = system.membus.master

    system.hwacc_to_mem = Bridge(delay='50ns')
    system.hwacc_to_mem.ranges = AddrRange(0x80000000, 0xffffffff)
    system.hwacc_to_mem.master = system.membus.slave
    system.hwacc_to_mem.slave = system.acc_cluster.bus.master

    system.acc_cluster.spm = SimpleMemory(range=AddrRange(0x2f100000, size='128kB'),
                                  conf_table_reported=False, latency='2ns')
    system.acc_cluster.spm.port = system.acc_cluster.bus.master

    system.acc_cluster.dma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.dma.pio = system.acc_cluster.bus.master
    system.acc_cluster.dma.dma = system.membus.slave

    system.acc_cluster.stream_dma_0 = StreamDma(pio_addr=0x2ff10000, pio_size=40, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_0.pio_delay = '1ns'
    system.acc_cluster.stream_dma_0.pio = system.acc_cluster.bus.master
    system.acc_cluster.stream_dma_0.dma = system.membus.slave
    system.acc_cluster.stream_dma_0.rd_int = 210
    system.acc_cluster.stream_dma_0.wr_int = 211
    system.acc_cluster.stream_dma_1 = StreamDma(pio_addr=0x2ff20000, pio_size=40, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_1.pio_delay = '1ns'
    system.acc_cluster.stream_dma_1.pio = system.acc_cluster.bus.master
    system.acc_cluster.stream_dma_1.dma = system.membus.slave
    system.acc_cluster.stream_dma_1.rd_int = 212
    system.acc_cluster.stream_dma_1.wr_int = 213
