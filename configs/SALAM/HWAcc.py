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

    ############################# Creating an Accelerator Cluster #################################
    # Create a new Accelerator Cluster
    system.acc_cluster = AccCluster()

    # The local_range is used for communication within the cluster. All memory mapped devices
    # (i.e. accelerators and SPMs) for the cluster should appear in this range.
    local_range = AddrRange(system.acc_cluster.local_range_min, system.acc_cluster.local_range_max)

    # The external_range is used for communication to devices outside of the cluster
    external_range = [AddrRange(system.acc_cluster.external_range_low_min, \
    							system.acc_cluster.external_range_low_max), \
    				  AddrRange(system.acc_cluster.external_range_hi_min, \
    				            system.acc_cluster.external_range_hi_max)]

	# Configure the cache
    CacheConfig(system.acc_cluster, acc_config)

	# Resize local and cache bus from config file specification 
    # system.acc_cluster._resize_bus(system.acc_cluster.cache_ports, system.acc_cluster.local_ports)

    # Generate bridges to connect local cluster bus to system membus
    system.acc_cluster._attach_bridges(system, local_range, external_range)

    # Add a shared cache for the accelerator cluster
    system.acc_cluster._connect_caches(system, options, system.acc_cluster.cache_size)

    ############################# Adding Accelerators to Cluster ##################################
    # Add a shared scratchpad memory to the cluster
    # spm_range = AddrRange()
    # spm_latency = '2ns'
    # system.acc_cluster._add_spm(spm_range, spm_latency)
    # system.acc_cluster._connect_spm(system.acc_cluster.spm)

    # Add an accelerator to the cluster
    system.acc_cluster.acc = CommInterface(devicename=options.accbench)
    AccConfig(system.acc_cluster.acc, acc_config, acc_bench)
    system.acc_cluster.acc_spm = ScratchpadMemory()
    AccSPMConfig(system.acc_cluster.acc, system.acc_cluster.acc_spm, acc_config)
    system.acc_cluster._connect_spm(system.acc_cluster.acc_spm)

    # Add an accelerator with a private SPM to the cluster
    # system.acc_cluster.acc = CommMemInterface(devicename=options.accbench)
    # AccConfig(system.acc_cluster.acc, local_range, acc_config, acc_bench)
    # AccPmemConfig(system.acc_cluster.acc, acc_config)

    # Add a shared cache for the accelerator cluster
    #system.acc_cluster._connect_caches(system, options, system.acc_cluster.acc.cache_size)

    # Connect the accelerator to the system's interrupt controller
    system.acc_cluster.acc.gic = system.realview.gic

    # Connect HWAcc to cluster buses
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc)
    system.acc_cluster.acc.local = system.acc_cluster.local_bus.slave
    system.acc_cluster.acc.acp = system.acc_cluster.coherency_bus.slave

    # Connect accelerator's private SPM to cluster buses
    # system.acc_cluster._connect_spm(system.acc_cluster.acc.private_memory)

    ################################## Adding DMAs to Cluster #####################################
    # Add DMA devices to the cluster and connect them
    system.acc_cluster.dma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32, int_num=95)
    system.acc_cluster._connect_dma(system, system.acc_cluster.dma)
    # system.acc_cluster.dma.dma = system.membus.slave
    # system.acc_cluster.dma.pio = system.acc_cluster.local_bus.master

    system.acc_cluster.stream_dma_0 = StreamDma(pio_addr=0x2ff10000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_0.stream = system.acc_cluster.acc.stream
    system.acc_cluster.stream_dma_0.stream_addr=0x2ff10020
    system.acc_cluster.stream_dma_0.stream_size=8
    system.acc_cluster.stream_dma_0.pio_delay = '1ns'
    system.acc_cluster.stream_dma_0.rd_int = 210
    system.acc_cluster.stream_dma_0.wr_int = 211
    system.acc_cluster._connect_dma(system, system.acc_cluster.stream_dma_0)

    system.acc_cluster.stream_dma_1 = StreamDma(pio_addr=0x2ff20000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acc_cluster.stream_dma_1.stream = system.acc_cluster.acc.stream
    system.acc_cluster.stream_dma_1.stream_addr=0x2ff20020
    system.acc_cluster.stream_dma_1.stream_size=8
    system.acc_cluster.stream_dma_1.pio_delay = '1ns'
    system.acc_cluster.stream_dma_1.rd_int = 212
    system.acc_cluster.stream_dma_1.wr_int = 213
    system.acc_cluster._connect_dma(system, system.acc_cluster.stream_dma_1)

