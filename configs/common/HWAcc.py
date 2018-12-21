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
    if (options.accbench != 'conv'):
    	 system.acc_cluster.acc = CommMemInterface(pio_addr=0x2f000000, pio_size=64, gic=system.realview.gic)
    	 system.acc_cluster.acc.flags_size = 1
    	 system.acc_cluster.acc.config_size = 0
    	 system.acc_cluster.acc.local_range = local_range
    	 system.acc_cluster.acc.llvm_interface = LLVMInterface()
    	 system.acc_cluster.acc.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + options.accbench + ".ll"
    	 system.acc_cluster.acc.int_num = 68
    	 system.acc_cluster.acc.clock_period = 10
    	 system.acc_cluster.acc.private_range = AddrRange(0x2f100000, size='128kB')
    	 system.acc_cluster.acc.private_memory = PrivateMemory(range=system.acc_cluster.acc.private_range, conf_table_reported=False, latency='2ns')
    	 system.acc_cluster.acc.private_read_ports = 4
    	 system.acc_cluster.acc.private_write_ports = 4
    	 system.acc_cluster.acc.private_memory.ready_mode = True
    	 system.acc_cluster._connect_hwacc(system.acc_cluster.acc)
    	 system.acc_cluster._connect_spm(system.acc_cluster.acc.private_memory)
    elif (options.accbench == 'conv'):
    # HW Managed Configuration
        private_range = AddrRange(0x2f100000, size='2MB')
        private_mem = PrivateMemory(range=private_range, conf_table_reported=False, latency='2ns')
    # Convolution
        system.acc_cluster.acc_conv = CommMemInterface(pio_addr=0x2f000000, pio_size=25, gic=system.realview.gic)
        system.acc_cluster.acc_conv.flags_size = 1
        system.acc_cluster.acc_conv.config_size = 0
        system.acc_cluster.acc_conv.local_range = local_range
        system.acc_cluster.acc_conv.llvm_interface = LLVMInterface()
        system.acc_cluster.acc_conv.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "conv" + ".ll"
        system.acc_cluster.acc_conv.int_num = 300
        system.acc_cluster.acc_conv.clock_period = 10
        system.acc_cluster.acc_conv.private_range = private_range
        system.acc_cluster.acc_conv.private_memory = private_mem
        system.acc_cluster.acc_conv.private_read_ports = 8
        system.acc_cluster.acc_conv.private_write_ports = 8
        #system.acc_cluster.acc_conv.private_memory.ready_mode = True
        system.acc_cluster._connect_hwacc(system.acc_cluster.acc_conv)
        system.acc_cluster._connect_spm(system.acc_cluster.acc_conv.private_memory)

    # Rectifier
        system.acc_cluster.acc_relu = CommMemInterface(pio_addr=0x2f000100, pio_size=17, gic=system.realview.gic)
        system.acc_cluster.acc_relu.flags_size = 1
        system.acc_cluster.acc_relu.config_size = 0
        system.acc_cluster.acc_relu.local_range = local_range
        system.acc_cluster.acc_relu.llvm_interface = LLVMInterface()
        system.acc_cluster.acc_relu.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "relu" + ".ll"
        system.acc_cluster.acc_relu.int_num = 301
        system.acc_cluster.acc_relu.clock_period = 10
        system.acc_cluster.acc_relu.private_range = private_range
        system.acc_cluster.acc_relu.private_memory = private_mem
        system.acc_cluster.acc_relu.private_read_ports = 8
        system.acc_cluster.acc_relu.private_write_ports = 8
        #system.acc_cluster.acc_relu.private_memory.ready_mode = True
        system.acc_cluster._connect_hwacc(system.acc_cluster.acc_relu)
        #system.acc_cluster._connect_spm(system.acc_cluster.acc_relu.private_memory)

    # Max Pooling
        system.acc_cluster.acc_pool = CommMemInterface(pio_addr=0x2f000200, pio_size=17, gic=system.realview.gic)
        system.acc_cluster.acc_pool.flags_size = 1
        system.acc_cluster.acc_pool.config_size = 0
        system.acc_cluster.acc_pool.local_range = local_range
        system.acc_cluster.acc_pool.llvm_interface = LLVMInterface()
        system.acc_cluster.acc_pool.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "max_pool" + ".ll"
        system.acc_cluster.acc_pool.int_num = 302
        system.acc_cluster.acc_pool.clock_period = 10
        system.acc_cluster.acc_pool.private_range = private_range
        system.acc_cluster.acc_pool.private_memory = private_mem
        system.acc_cluster.acc_pool.private_read_ports = 8
        system.acc_cluster.acc_pool.private_write_ports = 8
        #system.acc_cluster.acc_pool.private_memory.ready_mode = True
        system.acc_cluster._connect_hwacc(system.acc_cluster.acc_pool)
        #system.acc_cluster._connect_spm(system.acc_cluster.acc_pool.private_memory)
    else:
    # Combined
        system.acc_cluster.acc_comb = CommMemInterface(pio_addr=0x2f000000, pio_size=570, gic=system.realview.gic)
        system.acc_cluster.acc_comb.flags_size = 1
        system.acc_cluster.acc_comb.config_size = 0
        system.acc_cluster.acc_comb.local_range = local_range
        system.acc_cluster.acc_comb.llvm_interface = LLVMInterface()
        system.acc_cluster.acc_comb.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + "combined" + ".ll"
        system.acc_cluster.acc_comb.int_num = 68
        system.acc_cluster.acc_comb.clock_period = 10
        system.acc_cluster.acc_comb.private_range = AddrRange(0x2f100000, size='1MB')
        system.acc_cluster.acc_comb.private_memory = PrivateMemory(range=system.acc_cluster.acc_comb.private_range, conf_table_reported=False, latency='2ns')
        system.acc_cluster.acc_comb.private_read_ports = 4
        system.acc_cluster.acc_comb.private_write_ports = 4
        system.acc_cluster.acc_comb.private_memory.ready_mode = False
        system.acc_cluster._connect_hwacc(system.acc_cluster.acc_comb)
        system.acc_cluster._connect_spm(system.acc_cluster.acc_comb.private_memory)

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

#    if (options.accbench != 'multiacc'):
#        system.acc_cluster.acc_conv.llvm_interface.FU_fp_sp_adder = -1
#        system.acc_cluster.acc_conv.llvm_interface.FU_fp_dp_adder = -1
#        system.acc_cluster.acc_conv.llvm_interface.FU_fp_sp_multiplier = -1
#        system.acc_cluster.acc_conv.llvm_interface.FU_fp_dp_multiplier = -1
#        system.acc_cluster.acc_conv.llvm_interface.FU_counter = -1
#        system.acc_cluster.acc_conv.llvm_interface.FU_compare = -1
#        system.acc_cluster.acc_conv.llvm_interface.FU_GEP = -1
#        system.acc_cluster.acc_conv.llvm_interface.FU_pipelined = 1
#        system.acc_cluster.acc_conv.llvm_interface.sched_threshold = 1000
#        system.acc_cluster.acc_conv.llvm_interface.FU_clock_period = 10000

#        system.acc_cluster.acc_relu.llvm_interface.FU_fp_sp_adder = -1
#        system.acc_cluster.acc_relu.llvm_interface.FU_fp_dp_adder = -1
#        system.acc_cluster.acc_relu.llvm_interface.FU_fp_sp_multiplier = -1
#        system.acc_cluster.acc_relu.llvm_interface.FU_fp_dp_multiplier = -1
#        system.acc_cluster.acc_relu.llvm_interface.FU_counter = -1
#        system.acc_cluster.acc_relu.llvm_interface.FU_compare = -1
#        system.acc_cluster.acc_relu.llvm_interface.FU_GEP = -1
#        system.acc_cluster.acc_relu.llvm_interface.FU_pipelined = 1
#        system.acc_cluster.acc_relu.llvm_interface.sched_threshold = 1000
#        system.acc_cluster.acc_relu.llvm_interface.FU_clock_period = 10000

#        system.acc_cluster.acc_pool.llvm_interface.FU_fp_sp_adder = -1
#        system.acc_cluster.acc_pool.llvm_interface.FU_fp_dp_adder = -1
#        system.acc_cluster.acc_pool.llvm_interface.FU_fp_sp_multiplier = -1
#        system.acc_cluster.acc_pool.llvm_interface.FU_fp_dp_multiplier = -1
#        system.acc_cluster.acc_pool.llvm_interface.FU_counter = -1
#        system.acc_cluster.acc_pool.llvm_interface.FU_compare = -1
#        system.acc_cluster.acc_pool.llvm_interface.FU_GEP = -1
#        system.acc_cluster.acc_pool.llvm_interface.FU_pipelined = 1
#        system.acc_cluster.acc_pool.llvm_interface.sched_threshold = 1000
#        system.acc_cluster.acc_pool.llvm_interface.FU_clock_period = 10000

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


