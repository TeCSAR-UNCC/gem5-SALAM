import m5
from m5.objects import *
from m5.util import *

def makeHWAcc(options, system):

    # Construct our Accelerator Cluster and add a SPM of size 128kB
    system.acc_cluster = AccCluster()
    local_range = AddrRange(0x2f000000, 0x7fffffff)
    external_range = [AddrRange(0x00000000, 0x2effffff), AddrRange(0x80000000, 0xffffffff)]
    system.acc_cluster._attach_bridges(system, local_range, external_range)
    #system.acc_cluster._add_spm(AddrRange(0x2f100000, size='128kB'), '2ns')
    system.acc_cluster._connect_caches(system, options, '1kB')

    # Add an accelerator to the cluster and connect it
    #if (options.accbench != 'multiacc'):
#    	 system.acc_cluster.acc = CommMemInterface(pio_addr=0x2f000000, pio_size=64, gic=system.realview.gic)
#    	 system.acc_cluster.acc.flags_size = 1
#    	 system.acc_cluster.acc.config_size = 0
#    	 system.acc_cluster.acc.local_range = local_range
#    	 system.acc_cluster.acc.llvm_interface = LLVMInterface()
#    	 system.acc_cluster.acc.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + options.accbench + ".ll"
#    	 system.acc_cluster.acc.int_num = 68
#    	 system.acc_cluster.acc.clock_period = 10
#    	 system.acc_cluster.acc.private_range = AddrRange(0x2f100000, size='128kB')
#    	 system.acc_cluster.acc.private_memory = PrivateMemory(range=system.acc_cluster.acc.private_range, conf_table_reported=False, latency='2ns')
#    	 system.acc_cluster.acc.private_read_ports = 4
#    	 system.acc_cluster.acc.private_write_ports = 4
#    	 system.acc_cluster.acc.private_memory.ready_mode = True
#    	 system.acc_cluster._connect_hwacc(system.acc_cluster.acc)
#    	 system.acc_cluster._connect_spm(system.acc_cluster.acc.private_memory)

# HW Managed Configuration
# Convolution
    system.acc_cluster.acc_conv = CommMemInterface(pio_addr=0x2f000000, pio_size=64, gic=system.realview.gic)
    system.acc_cluster.acc_conv.flags_size = 1
    system.acc_cluster.acc_conv.config_size = 0
    system.acc_cluster.acc_conv.local_range = local_range
    system.acc_cluster.acc_conv.llvm_interface = LLVMInterface()
    system.acc_cluster.acc_conv.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "conv" + ".ll"
    system.acc_cluster.acc_conv.int_num = 68
    system.acc_cluster.acc_conv.clock_period = 10
    system.acc_cluster.acc_conv.private_range = AddrRange(0x2f100000, size='128kB')
    system.acc_cluster.acc_conv.private_memory = PrivateMemory(range=system.acc_cluster.acc_conv.private_range, conf_table_reported=False, latency='2ns')
    system.acc_cluster.acc_conv.private_read_ports = 4
    system.acc_cluster.acc_conv.private_write_ports = 4
    system.acc_cluster.acc_conv.private_memory.ready_mode = True
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc_conv)
    system.acc_cluster._connect_spm(system.acc_cluster.acc_conv.private_memory)

# Rectifier
    system.acc_cluster.acc_relu = CommMemInterface(pio_addr=0x2f000101, pio_size=64, gic=system.realview.gic)
    system.acc_cluster.acc_relu.flags_size = 1
    system.acc_cluster.acc_relu.config_size = 0
    system.acc_cluster.acc_relu.local_range = local_range
    system.acc_cluster.acc_relu.llvm_interface = LLVMInterface()
    system.acc_cluster.acc_relu.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "relu" + ".ll"
    system.acc_cluster.acc_relu.int_num = 69
    system.acc_cluster.acc_relu.clock_period = 10
    system.acc_cluster.acc_relu.private_range = AddrRange(0x2f200000, size='128kB')
    system.acc_cluster.acc_relu.private_memory = PrivateMemory(range=system.acc_cluster.acc_relu.private_range, conf_table_reported=False, latency='2ns')
    system.acc_cluster.acc_relu.private_read_ports = 4
    system.acc_cluster.acc_relu.private_write_ports = 4
    system.acc_cluster.acc_relu.private_memory.ready_mode = True
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc_relu)
    system.acc_cluster._connect_spm(system.acc_cluster.acc_relu.private_memory)

# Max Pooling
    system.acc_cluster.acc_pool = CommMemInterface(pio_addr=0x2f000202, pio_size=64, gic=system.realview.gic)
    system.acc_cluster.acc_pool.flags_size = 1
    system.acc_cluster.acc_pool.config_size = 0
    system.acc_cluster.acc_pool.local_range = local_range
    system.acc_cluster.acc_pool.llvm_interface = LLVMInterface()
    system.acc_cluster.acc_pool.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "pool" + ".ll"
    system.acc_cluster.acc_pool.int_num = 70
    system.acc_cluster.acc_pool.clock_period = 10
    system.acc_cluster.acc_pool.private_range = AddrRange(0x2f300000, size='128kB')
    system.acc_cluster.acc_pool.private_memory = PrivateMemory(range=system.acc_cluster.acc_pool.private_range, conf_table_reported=False, latency='2ns')
    system.acc_cluster.acc_pool.private_read_ports = 4
    system.acc_cluster.acc_pool.private_write_ports = 4
    system.acc_cluster.acc_pool.private_memory.ready_mode = True
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc_pool)
    system.acc_cluster._connect_spm(system.acc_cluster.acc_pool.private_memory)

## Combined
#    system.acc_cluster.acc_comb = CommMemInterface(pio_addr=0x2f000003, pio_size=570, gic=system.realview.gic)
#    system.acc_cluster.acc_comb.flags_size = 1
#    system.acc_cluster.acc_comb.config_size = 0
#    system.acc_cluster.acc_comb.local_range = local_range
#    system.acc_cluster.acc_comb.llvm_interface = LLVMInterface()
#    system.acc_cluster.acc_comb.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "combined" + ".ll"
#    system.acc_cluster.acc_comb.int_num = 68
#    system.acc_cluster.acc_comb.clock_period = 10
#    system.acc_cluster.acc_comb.private_range = AddrRange(0x2f100000, size='128kB')
#    system.acc_cluster.acc_comb.private_memory = PrivateMemory(range=system.acc_cluster.acc_comb.private_range, conf_table_reported=False, latency='2ns')
#    system.acc_cluster.acc_comb.private_read_ports = 4
#    system.acc_cluster.acc_comb.private_write_ports = 4
#    system.acc_cluster.acc_comb.private_memory.ready_mode = False
#    system.acc_cluster._connect_hwacc(system.acc_cluster.acc_comb)
#    system.acc_cluster._connect_spm(system.acc_cluster.acc_comb.private_memory)

#    else:
#        system.acc_cluster.gemm = CommMemInterface(pio_addr=0x2f000000, pio_size=25, gic=system.realview.gic)
#        system.acc_cluster.gemm.flags_size = 1
#        system.acc_cluster.gemm.config_size = 0
#        system.acc_cluster.gemm.local_range = local_range
#        system.acc_cluster.gemm.llvm_interface = LLVMInterface()
#        system.acc_cluster.gemm.llvm_interface.in_file = options.accpath + "/" + "multiacc" + "/bench/" + "gemm" + ".ll"
#        system.acc_cluster.gemm.int_num = 68
#        system.acc_cluster.gemm.clock_period = 10
#        system.acc_cluster.gemm.private_range = AddrRange(0x2f100000, size='128kB')
#        system.acc_cluster.gemm.private_memory = PrivateMemory(range=system.acc_cluster.gemm.private_range, conf_table_reported=False, latency='2ns')
#        system.acc_cluster.gemm.private_read_ports = 4
#        system.acc_cluster.gemm.private_write_ports = 4
#        system.acc_cluster._connect_hwacc(system.acc_cluster.gemm)
#        system.acc_cluster._connect_spm(system.acc_cluster.gemm.private_memory)

#        system.acc_cluster.fir = CommMemInterface(pio_addr=0x2f000019, pio_size=64, gic=system.realview.gic)
#        system.acc_cluster.fir.flags_size = 1
#        system.acc_cluster.fir.config_size = 0
#        system.acc_cluster.fir.local_range = local_range
#        system.acc_cluster.fir.llvm_interface = LLVMInterface()
#        system.acc_cluster.fir.llvm_interface.in_file = options.accpath + "/" + "multiacc" + "/bench/" + "fir" + ".ll"
#        system.acc_cluster.fir.int_num = 69
#        system.acc_cluster.fir.clock_period = 10
#        system.acc_cluster.fir.private_range = AddrRange(0x2f200000, size='128kB')
#        system.acc_cluster.fir.private_memory = PrivateMemory(range=system.acc_cluster.fir.private_range, conf_table_reported=False, latency='2ns')
#        system.acc_cluster.fir.private_read_ports = 4
#        system.acc_cluster.fir.private_write_ports = 4
#        system.acc_cluster._connect_hwacc(system.acc_cluster.fir)
#        system.acc_cluster._connect_spm(system.acc_cluster.fir.private_memory)


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

#     Set functional unit constraits for accelerator
#    if (options.accbench != 'multiacc'):
#        system.acc_cluster.acc.llvm_interface.FU_fp_sp_adder = -1
#        system.acc_cluster.acc.llvm_interface.FU_fp_dp_adder = -1
#        system.acc_cluster.acc.llvm_interface.FU_fp_sp_multiplier = -1
#        system.acc_cluster.acc.llvm_interface.FU_fp_dp_multiplier = -1
#        system.acc_cluster.acc.llvm_interface.FU_counter = -1
#        system.acc_cluster.acc.llvm_interface.FU_compare = -1
#        system.acc_cluster.acc.llvm_interface.FU_GEP = -1
#        system.acc_cluster.acc.llvm_interface.FU_pipelined = 1
#        system.acc_cluster.acc.llvm_interface.sched_threshold = 1000
#        system.acc_cluster.acc.llvm_interface.FU_clock_period = 10000

    if (options.accbench != 'multiacc'):
        system.acc_cluster.acc_conv.llvm_interface.FU_fp_sp_adder = -1
        system.acc_cluster.acc_conv.llvm_interface.FU_fp_dp_adder = -1
        system.acc_cluster.acc_conv.llvm_interface.FU_fp_sp_multiplier = -1
        system.acc_cluster.acc_conv.llvm_interface.FU_fp_dp_multiplier = -1
        system.acc_cluster.acc_conv.llvm_interface.FU_counter = -1
        system.acc_cluster.acc_conv.llvm_interface.FU_compare = -1
        system.acc_cluster.acc_conv.llvm_interface.FU_GEP = -1
        system.acc_cluster.acc_conv.llvm_interface.FU_pipelined = 1
        system.acc_cluster.acc_conv.llvm_interface.sched_threshold = 1000
        system.acc_cluster.acc_conv.llvm_interface.FU_clock_period = 10000

        system.acc_cluster.acc_relu.llvm_interface.FU_fp_sp_adder = -1
        system.acc_cluster.acc_relu.llvm_interface.FU_fp_dp_adder = -1
        system.acc_cluster.acc_relu.llvm_interface.FU_fp_sp_multiplier = -1
        system.acc_cluster.acc_relu.llvm_interface.FU_fp_dp_multiplier = -1
        system.acc_cluster.acc_relu.llvm_interface.FU_counter = -1
        system.acc_cluster.acc_relu.llvm_interface.FU_compare = -1
        system.acc_cluster.acc_relu.llvm_interface.FU_GEP = -1
        system.acc_cluster.acc_relu.llvm_interface.FU_pipelined = 1
        system.acc_cluster.acc_relu.llvm_interface.sched_threshold = 1000
        system.acc_cluster.acc_relu.llvm_interface.FU_clock_period = 10000

        system.acc_cluster.acc_pool.llvm_interface.FU_fp_sp_adder = -1
        system.acc_cluster.acc_pool.llvm_interface.FU_fp_dp_adder = -1
        system.acc_cluster.acc_pool.llvm_interface.FU_fp_sp_multiplier = -1
        system.acc_cluster.acc_pool.llvm_interface.FU_fp_dp_multiplier = -1
        system.acc_cluster.acc_pool.llvm_interface.FU_counter = -1
        system.acc_cluster.acc_pool.llvm_interface.FU_compare = -1
        system.acc_cluster.acc_pool.llvm_interface.FU_GEP = -1
        system.acc_cluster.acc_pool.llvm_interface.FU_pipelined = 1
        system.acc_cluster.acc_pool.llvm_interface.sched_threshold = 1000
        system.acc_cluster.acc_pool.llvm_interface.FU_clock_period = 10000

#    if (options.accbench != 'multiacc'):
#        system.acc_cluster.acc_comb.llvm_interface.FU_fp_sp_adder = -1
#        system.acc_cluster.acc_comb.llvm_interface.FU_fp_dp_adder = -1
#        system.acc_cluster.acc_comb.llvm_interface.FU_fp_sp_multiplier = -1
#        system.acc_cluster.acc_comb.llvm_interface.FU_fp_dp_multiplier = -1
#        system.acc_cluster.acc_comb.llvm_interface.FU_counter = -1
#        system.acc_cluster.acc_comb.llvm_interface.FU_compare = -1
#        system.acc_cluster.acc_comb.llvm_interface.FU_GEP = -1
#        system.acc_cluster.acc_comb.llvm_interface.FU_pipelined = 1
#        system.acc_cluster.acc_comb.llvm_interface.sched_threshold = 1000
#        system.acc_cluster.acc_comb.llvm_interface.FU_clock_period = 10000


