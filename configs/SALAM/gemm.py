import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *

class L1Cache(Cache):
	assoc = 2
	tag_latency = 2
	data_latency = 2
	response_latency = 2
	mshrs = 4
	tgts_per_mshr = 20
	
def __init__(self, size, options=None):
		self.size = size
		super(L1Cache, self).__init__()
		pass

def buildgemm(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x1003802f
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.master = clstr.local_bus.slave
	clstr._connect_caches(system, options, l2coherent=False)
	gic = system.realview.gic

	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x10020000, pio_size = 21, gic=gic, int_num=95)
	clstr.dma.cluster_dma = clstr.local_bus.slave
	clstr.dma.max_req_size = 64
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.dma.pio
	
	# top Definition
	acc = "top"
	config = "/home/he-man/gem5-SALAM/benchmarks/sys_validation/gemm/hw/top.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/sys_validation/gemm/hw/top.ll"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020015, pio_size=25, int_num=68)
	AccConfig(clstr.top, config, ir)
	
	# gemm Definition
	acc = "gemm"
	config = "/home/he-man/gem5-SALAM/benchmarks/sys_validation/gemm/hw/gemm.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/sys_validation/gemm/hw/gemm.ll"
	clstr.gemm = CommInterface(devicename=acc, gic=gic, pio_addr=0x1002002e, pio_size=1)
	AccConfig(clstr.gemm, config, ir)
	
	# top Config
	clstr.top.local = clstr.local_bus.slave
	clstr.top.pio = clstr.local_bus.master
	clstr.top.enable_debug_msgs = False
	
	# gemm Config
	clstr.gemm.pio = clstr.local_bus.master
	clstr.gemm.enable_debug_msgs = False
	
	# MATRIX1 (Variable)
	addr = 0x1002002f
	spmRange = AddrRange(addr, addr + 0x8000)
	clstr.matrix1 = ScratchpadMemory(range = spmRange)
	clstr.matrix1.conf_table_reported = False
	clstr.matrix1.ready_mode = False
	clstr.matrix1.port = clstr.local_bus.master
	
	# Connecting MATRIX1 to gemm
	for i in range(2):
		clstr.gemm.spm = clstr.matrix1.spm_ports
	
	# MATRIX2 (Variable)
	addr = 0x1002802f
	spmRange = AddrRange(addr, addr + 0x8000)
	clstr.matrix2 = ScratchpadMemory(range = spmRange)
	clstr.matrix2.conf_table_reported = False
	clstr.matrix2.ready_mode = False
	clstr.matrix2.port = clstr.local_bus.master
	
	# Connecting MATRIX2 to gemm
	for i in range(2):
		clstr.gemm.spm = clstr.matrix2.spm_ports
	
	# MATRIX3 (Variable)
	addr = 0x1003002f
	spmRange = AddrRange(addr, addr + 0x8000)
	clstr.matrix3 = ScratchpadMemory(range = spmRange)
	clstr.matrix3.conf_table_reported = False
	clstr.matrix3.ready_mode = False
	clstr.matrix3.port = clstr.local_bus.master
	
	# Connecting MATRIX3 to gemm
	for i in range(2):
		clstr.gemm.spm = clstr.matrix3.spm_ports
	
def makeHWAcc(options, system):

	system.gemm = AccCluster()
	buildgemm(options, system, system.gemm)

