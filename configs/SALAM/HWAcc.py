import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *


def makeHWAcc(options, system):
    # Specify the path to the benchmark file for an accelerator
    # acc_bench = <Absolute path to benchmark LLVM file>
    acc_bench = options.accpath + "/" + options.accbench + "/bench/" + options.accbench + ".ll"

    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config = options.accpath + "/" + options.accbench + "/config.ini"

    ############################# Creating the Accelerator Cluster #################################
    # Create a new Accelerator Cluster
    system.acctest    = AccCluster()
    local_low       = 0x2F000000
    local_high      = 0x2FFFFFFF
    local_range     = AddrRange(local_low, local_high)
    external_range  = [AddrRange(0x00000000, local_low-1),
                       AddrRange(local_high+1, 0xFFFFFFFF)]
    system.acctest._attach_bridges(system, local_range, external_range)
    system.acctest._connect_caches(system, options)

    ############################# Adding Accelerators to Cluster ##################################
    # Add an accelerator to the cluster
    system.acc_cluster.acc = CommInterface(devicename=options.accbench)
    AccConfig(system.acc_cluster.acc, acc_config, acc_bench)

    # Add an SPM for the accelerator
    system.acc_cluster.acc_spm = ScratchpadMemory()
    AccSPMConfig(system.acc_cluster.acc, system.acc_cluster.acc_spm, acc_config)
    system.acc_cluster._connect_spm(system.acc_cluster.acc_spm)

    # Connect the accelerator to the system's interrupt controller
    system.acc_cluster.acc.gic = system.realview.gic

    # Connect HWAcc to cluster buses
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc)
    system.acc_cluster.acc.local = system.acc_cluster.local_bus.slave
    system.acc_cluster.acc.acp = system.acc_cluster.coherency_bus.slave

    # Enable display of debug messages for the accelerator
    system.acc_cluster.acc.enable_debug_msgs = True

    ################################## Adding DMAs to Cluster #####################################
    # Add DMA devices to the cluster and connect them
    system.acc_cluster.dma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32, int_num=95)
    system.acc_cluster._connect_cluster_dma(system, system.acc_cluster.dma)
    # system.acc_cluster.dma.dma = system.membus.slave
    # system.acc_cluster.dma.pio = system.acc_cluster.local_bus.master

    system.acc_cluster.stream_dma_0 = StreamDma(pio_addr=0x2ff10000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_0.stream_in = system.acc_cluster.acc.stream
    system.acc_cluster.stream_dma_0.stream_out = system.acc_cluster.acc.stream
    system.acc_cluster.stream_dma_0.stream_addr=0x2ff10020
    system.acc_cluster.stream_dma_0.stream_size=8
    system.acc_cluster.stream_dma_0.pio_delay = '1ns'
    system.acc_cluster.stream_dma_0.rd_int = 210
    system.acc_cluster.stream_dma_0.wr_int = 211
    system.acc_cluster._connect_dma(system, system.acc_cluster.stream_dma_0)

    system.acc_cluster.stream_dma_1 = StreamDma(pio_addr=0x2ff20000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_1.stream_in = system.acc_cluster.acc.stream
    system.acc_cluster.stream_dma_1.stream_out = system.acc_cluster.acc.stream
    system.acc_cluster.stream_dma_1.stream_addr=0x2ff20020
    system.acc_cluster.stream_dma_1.stream_size=8
    system.acc_cluster.stream_dma_1.pio_delay = '1ns'
    system.acc_cluster.stream_dma_1.rd_int = 212
    system.acc_cluster.stream_dma_1.wr_int = 213
    system.acc_cluster._connect_dma(system, system.acc_cluster.stream_dma_1)

