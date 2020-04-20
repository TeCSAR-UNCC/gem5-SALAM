import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *


def makeHWAcc(options, system):
    hw_path = options.accpath + "/" + options.accbench + "/hw/"

    ############################# Creating the Accelerator Cluster #################################
    # Create a new Accelerator Cluster
    system.acctest    = AccCluster()
    local_low       = 0x2F000000
    local_high      = 0x2FFFFFFF
    local_range     = AddrRange(local_low, local_high)
    external_range  = [AddrRange(0x00000000, local_low-1),
                       AddrRange(local_high+1, 0xFFFFFFFF)]
    system.acctest._attach_bridges(system, local_range, external_range)
    system.acctest._connect_caches(system, options, l2coherent=False)
    gic = system.realview.gic

    ############################# Adding Devices to Cluster ##################################
    # Add the top function
    # The top function manages the DMA and bench accelerator, and is used to measure the
    # total system execution time, including data movement.
    acc = "top"
    config = hw_path + acc + ".ini"
    ir = hw_path + acc + ".ll"
    system.acctest.top = CommInterface(devicename=acc, gic=gic)
    AccConfig(system.acctest.top, config, ir)
    system.acctest._connect_hwacc(system.acctest.top)

    # Add the benchmark function
    acc = options.accbench
    config = hw_path + acc + ".ini"
    ir = hw_path + acc + ".ll"
    system.acctest.bench = CommInterface(devicename=acc, gic=gic, reset_spm=False)
    AccConfig(system.acctest.bench, config, ir)
    system.acctest.bench.pio = system.acctest.top.local
    system.acctest.spm = ScratchpadMemory()
    AccSPMConfig(system.acctest.bench, system.acctest.spm, config)
    system.acctest._connect_spm(system.acctest.spm)
    # system.acctest.bench.enable_debug_msgs = True

    if acc == "fft":
        max_req_size = 8
        buffer_size = 48
    elif acc == "gemm":
        max_req_size = 8
        buffer_size = 64
    elif acc == "md-knn":
        max_req_size = 4
        buffer_size = 24
    elif acc == "stencil2d":
        max_req_size = 4
        buffer_size = 24
    elif acc == "stencil3d":
        max_req_size = 4
        buffer_size = 32
    else:
        max_req_size = 4
        buffer_size = 16

    # Add the cluster DMA
    system.acctest.dma = NoncoherentDma(pio_addr=0x2FF00000, pio_size=21, gic=gic, int_num=98)
    system.acctest.dma.cluster_dma = system.acctest.local_bus.slave
    system.acctest.dma.dma = system.acctest.coherency_bus.slave
    system.acctest.dma.pio = system.acctest.top.local
    system.acctest.dma.max_req_size = max_req_size
    system.acctest.dma.buffer_size = buffer_size