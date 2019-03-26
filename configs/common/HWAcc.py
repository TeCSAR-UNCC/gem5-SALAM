import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *


def makeHWAcc(options, system):
    ############################# Creating an Accelerator Cluster #################################
    # Create a new Accelerator Cluster
    system.acc_cluster = AccCluster()

    # The local_range is used for communication within the cluster. All memory mapped devices
    # (i.e. accelerators and SPMs) for the cluster should appear in this range.
    local_range = AddrRange(0x2f000000, 0x7fffffff)

    # The external_range is used for communication to devices outside of the cluster
    external_range = [AddrRange(0x00000000, 0x2effffff), AddrRange(0x80000000, 0xffffffff)]

    # Generate bridges to connect local cluster bus to system membus
    system.acc_cluster._attach_bridges(system, local_range, external_range)

    # Add a shared cache for the accelerator cluster
    system.acc_cluster._connect_caches(system, options, '1kB')

    ############################# Adding Accelerators to Cluster ##################################
    # Add a shared scratchpad memory to the cluster
    # spm_range = AddrRange()
    # spm_latency = '2ns'
    # system.acc_cluster._add_spm(spm_range, spm_latency)
    # system.acc_cluster._connect_spm(system.acc_cluster.spm)

    # Specify the path to the benchmark file for an accelerator
    # acc_bench = <Absolute path to benchmark LLVM file>
    acc_bench = options.accpath + "/" + options.accbench + "/bench/" + options.accbench + ".ll"

    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config = options.accpath + "/" + options.accbench + "/config.ini"

    # Add an accelerator to the cluster
    # system.acc_cluster.acc = CommMemInterface()
    # AccConfig(options, system.acc_cluster.acc, local_range, acc_config, acc_bench)

    # Add an accelerator with a private SPM to the cluster
    system.acc_cluster.acc = CommMemInterface()
    AccConfig(system.acc_cluster.acc, local_range, acc_config, acc_bench)
    AccPmemConfig(system.acc_cluster.acc, acc_config)

    # Connect the accelerator to the system's interrupt controller
    system.acc_cluster.acc.gic = system.realview.gic

    # Connect HWAcc to cluster buses
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc)

    # Connect accelerator's private SPM to cluster buses
    system.acc_cluster._connect_spm(system.acc_cluster.acc.private_memory)

    ################################## Adding DMAs to Cluster #####################################
    # Add DMA devices to the cluster and connect them
    system.acc_cluster.dma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32, int_num=95)
    system.acc_cluster._connect_dma(system, system.acc_cluster.dma)

    system.acc_cluster.stream_dma_0 = StreamDma(pio_addr=0x2ff10000, pio_size=40, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_0.pio_delay = '1ns'
    system.acc_cluster.stream_dma_0.rd_int = 210
    system.acc_cluster.stream_dma_0.wr_int = 211
    system.acc_cluster._connect_dma(system, system.acc_cluster.stream_dma_0)

    system.acc_cluster.stream_dma_1 = StreamDma(pio_addr=0x2ff20000, pio_size=40, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_1.pio_delay = '1ns'
    system.acc_cluster.stream_dma_1.rd_int = 212
    system.acc_cluster.stream_dma_1.wr_int = 213
    system.acc_cluster._connect_dma(system, system.acc_cluster.stream_dma_1)

