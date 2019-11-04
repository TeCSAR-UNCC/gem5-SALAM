import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *

def makeHWAcc(options, system):
    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config = options.accpath + "/" + options.accbench + "/config.ini"
    acc_bench = options.accpath + "/" + options.accbench + "/combined/bench/combined.ll"

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
    system.acc_cluster._resize_bus(system.acc_cluster.cache_ports, system.acc_cluster.local_ports)
    
    # Generate bridges to connect local cluster bus to system membus
    system.acc_cluster._attach_bridges(system, local_range, external_range)
    
    # Add a shared cache for the accelerator cluster
    system.acc_cluster._connect_caches(system, options, system.acc_cluster.cache_size)

    ############################# Adding Accelerators to Cluster ##################################
    
    system.acc_cluster.acc = CommMemInterface()
    AccConfig(system.acc_cluster.acc, local_range, acc_config, acc_bench)
    AccPmemConfig(system.acc_cluster.acc, acc_config)
    system.acc_cluster.acc.gic = system.realview.gic
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc)
    
    ############################# conv ##################################
    # Specify the path to the benchmark file for an accelerator
    # acc_bench = <Absolute path to benchmark LLVM file>
    acc_bench_conv = options.accpath + "/multiple_acc/conv/bench/" + "conv" + ".ll"

    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config_conv = options.accpath + "/multiple_acc/conv" + "/config.ini"
    
    
    # Add an accelerator with a private SPM to the cluster
    system.acc_cluster.acc_conv = CommMemInterface()
    AccConfig(system.acc_cluster.acc_conv, local_range, acc_config_conv, acc_bench_conv)
    #AccPmemConfig(system.acc_cluster.acc_conv, acc_config_conv)
    system.acc_cluster.acc_conv.private_range = system.acc_cluster.acc.private_range
    system.acc_cluster.acc_conv.private_memory = system.acc_cluster.acc.private_memory
    
    # Connect the accelerator to the system's interrupt controller
    system.acc_cluster.acc_conv.gic = system.realview.gic
    
    # Connect HWAcc to cluster buses
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc_conv)

    # Connect accelerator's private SPM to cluster buses
    system.acc_cluster._connect_spm(system.acc_cluster.acc_conv.private_memory)
    
    ############################# relu ##################################
    # Specify the path to the benchmark file for an accelerator
    # acc_bench = <Absolute path to benchmark LLVM file>
    acc_bench_relu = options.accpath + "/multiple_acc/relu/bench/" + "relu" + ".ll"

    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config_relu = options.accpath + "/multiple_acc/relu" + "/config.ini"
    
    
    # Add an accelerator with a private SPM to the cluster
    system.acc_cluster.acc_relu = CommMemInterface()
    AccConfig(system.acc_cluster.acc_relu, local_range, acc_config_relu, acc_bench_relu)
    #AccPmemConfig(system.acc_cluster.acc_relu, acc_config_relu)
    system.acc_cluster.acc_relu.private_range = system.acc_cluster.acc.private_range
    system.acc_cluster.acc_relu.private_memory = system.acc_cluster.acc.private_memory
    
    # Connect the accelerator to the system's interrupt controller
    system.acc_cluster.acc_relu.gic = system.realview.gic
    
    # Connect HWAcc to cluster buses
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc_relu)

    # Connect accelerator's private SPM to cluster buses
    #system.acc_cluster._connect_spm(system.acc_cluster.acc_relu.private_memory)
    
    ############################# max_pool ##################################
    # Specify the path to the benchmark file for an accelerator
    # acc_bench = <Absolute path to benchmark LLVM file>
    acc_bench_max_pool = options.accpath + "/multiple_acc/max_pool/bench/" + "max_pool" + ".ll"

    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config_max_pool = options.accpath + "/multiple_acc/max_pool" + "/config.ini"
    
    
    # Add an accelerator with a private SPM to the cluster
    system.acc_cluster.acc_max_pool = CommMemInterface()
    AccConfig(system.acc_cluster.acc_max_pool, local_range, acc_config_max_pool, acc_bench_max_pool)
    #AccPmemConfig(system.acc_cluster.acc_max_pool, acc_config_max_pool)
    system.acc_cluster.acc_max_pool.private_range = system.acc_cluster.acc.private_range
    system.acc_cluster.acc_max_pool.private_memory = system.acc_cluster.acc.private_memory
    
    # Connect the accelerator to the system's interrupt controller
    system.acc_cluster.acc_max_pool.gic = system.realview.gic
    
    # Connect HWAcc to cluster buses
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc_max_pool)

    # Connect accelerator's private SPM to cluster buses
    #system.acc_cluster._connect_spm(system.acc_cluster.acc_max_pool.private_memory)    
    
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
    
    
    
    
    
    
    
    
    
