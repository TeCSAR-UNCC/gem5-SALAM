import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *

def buildlenet5(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x10048213
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.master = clstr.local_bus.slave
	clstr._connect_caches(system, options, l2coherent=False)
	gic = system.realview.gic

	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x10020000, pio_size = 21, gic=gic, int_num=95)
	clstr.dma.cluster_dma = clstr.local_bus.slave
	clstr.dma.max_req_size = 4
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.dma.pio

	# top Definition
	acc = "top"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/top.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/top.ll"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020015, pio_size=9)
	AccConfig(clstr.top, config, ir)

	# conv0 Definition
	acc = "conv0"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv0.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv0.ll"
	clstr.conv0 = CommInterface(devicename=acc, gic=gic, pio_addr=0x1002001e, pio_size=1)
	AccConfig(clstr.conv0, config, ir)

	# pool0 Definition
	acc = "pool0"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/pool0.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/pool0.ll"
	clstr.pool0 = CommInterface(devicename=acc, gic=gic, pio_addr=0x1002b7cf, pio_size=1)
	AccConfig(clstr.pool0, config, ir)

	# conv1 Definition
	acc = "conv1"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv1.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv1.ll"
	clstr.conv1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10037050, pio_size=1)
	AccConfig(clstr.conv1, config, ir)

	# pool1 Definition
	acc = "pool1"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/pool1.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/pool1.ll"
	clstr.pool1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x1003d391, pio_size=1)
	AccConfig(clstr.pool1, config, ir)

	# conv2 Definition
	acc = "conv2"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv2.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv2.ll"
	clstr.conv2 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10041412, pio_size=1)
	AccConfig(clstr.conv2, config, ir)

	# top Config
	clstr.top.local = clstr.local_bus.slave
	clstr.top.pio = clstr.local_bus.master
	clstr.top.enable_debug_msgs = False

	# conv0 Config
	clstr.conv0.pio = clstr.local_bus.master
	clstr.conv0.enable_debug_msgs = False

	# Conv0Window (Variable)
	addr = 0x1002001f
	spmRange = AddrRange(addr, addr + 0x2000)
	clstr.conv0window = ScratchpadMemory(range = spmRange)
	clstr.conv0window.conf_table_reported = False
	clstr.conv0window.ready_mode = False
	clstr.conv0window.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv0.spm = clstr.conv0window.spm_ports

	# Conv0Weights (Variable)
	addr = 0x1002201f
	spmRange = AddrRange(addr, addr + 0x4b0)
	clstr.conv0weights = ScratchpadMemory(range = spmRange)
	clstr.conv0weights.conf_table_reported = False
	clstr.conv0weights.ready_mode = False
	clstr.conv0weights.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv0.spm = clstr.conv0weights.spm_ports

	# Conv0Output (Variable)
	addr = 0x100224cf
	spmRange = AddrRange(addr, addr + 0x9300)
	clstr.conv0output = ScratchpadMemory(range = spmRange)
	clstr.conv0output.conf_table_reported = False
	clstr.conv0output.ready_mode = False
	clstr.conv0output.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv0.spm = clstr.conv0output.spm_ports

	# pool0 Config
	clstr.pool0.pio = clstr.local_bus.master
	clstr.pool0.enable_debug_msgs = False

	# pool0Window (Variable)
	addr = 0x1002b7d0
	spmRange = AddrRange(addr, addr + 0x9300)
	clstr.pool0window = ScratchpadMemory(range = spmRange)
	clstr.pool0window.conf_table_reported = False
	clstr.pool0window.ready_mode = False
	clstr.pool0window.port = clstr.local_bus.master
	for i in range(1):
		clstr.pool0.spm = clstr.pool0window.spm_ports

	# pool0Weights (Variable)
	addr = 0x10034ad0
	spmRange = AddrRange(addr, addr + 0xc0)
	clstr.pool0weights = ScratchpadMemory(range = spmRange)
	clstr.pool0weights.conf_table_reported = False
	clstr.pool0weights.ready_mode = False
	clstr.pool0weights.port = clstr.local_bus.master
	for i in range(1):
		clstr.pool0.spm = clstr.pool0weights.spm_ports

	# pool0Output (Variable)
	addr = 0x10034b90
	spmRange = AddrRange(addr, addr + 0x24c0)
	clstr.pool0output = ScratchpadMemory(range = spmRange)
	clstr.pool0output.conf_table_reported = False
	clstr.pool0output.ready_mode = False
	clstr.pool0output.port = clstr.local_bus.master
	for i in range(1):
		clstr.pool0.spm = clstr.pool0output.spm_ports

	# conv1 Config
	clstr.conv1.pio = clstr.local_bus.master
	clstr.conv1.enable_debug_msgs = False

	# Conv1Window (Variable)
	addr = 0x10037051
	spmRange = AddrRange(addr, addr + 0x24c0)
	clstr.conv1window = ScratchpadMemory(range = spmRange)
	clstr.conv1window.conf_table_reported = False
	clstr.conv1window.ready_mode = False
	clstr.conv1window.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv1.spm = clstr.conv1window.spm_ports

	# Conv1Weights (Variable)
	addr = 0x10039511
	spmRange = AddrRange(addr, addr + 0xc80)
	clstr.conv1weights = ScratchpadMemory(range = spmRange)
	clstr.conv1weights.conf_table_reported = False
	clstr.conv1weights.ready_mode = False
	clstr.conv1weights.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv1.spm = clstr.conv1weights.spm_ports

	# Conv1Output (Variable)
	addr = 0x1003a191
	spmRange = AddrRange(addr, addr + 0x3200)
	clstr.conv1output = ScratchpadMemory(range = spmRange)
	clstr.conv1output.conf_table_reported = False
	clstr.conv1output.ready_mode = False
	clstr.conv1output.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv1.spm = clstr.conv1output.spm_ports

	# pool1 Config
	clstr.pool1.pio = clstr.local_bus.master
	clstr.pool1.enable_debug_msgs = False

	# pool1Window (Variable)
	addr = 0x1003d392
	spmRange = AddrRange(addr, addr + 0x3200)
	clstr.pool1window = ScratchpadMemory(range = spmRange)
	clstr.pool1window.conf_table_reported = False
	clstr.pool1window.ready_mode = False
	clstr.pool1window.port = clstr.local_bus.master
	for i in range(1):
		clstr.pool1.spm = clstr.pool1window.spm_ports

	# pool1Weights (Variable)
	addr = 0x10040592
	spmRange = AddrRange(addr, addr + 0x200)
	clstr.pool1weights = ScratchpadMemory(range = spmRange)
	clstr.pool1weights.conf_table_reported = False
	clstr.pool1weights.ready_mode = False
	clstr.pool1weights.port = clstr.local_bus.master
	for i in range(1):
		clstr.pool1.spm = clstr.pool1weights.spm_ports

	# pool1Output (Variable)
	addr = 0x10040792
	spmRange = AddrRange(addr, addr + 0xc80)
	clstr.pool1output = ScratchpadMemory(range = spmRange)
	clstr.pool1output.conf_table_reported = False
	clstr.pool1output.ready_mode = False
	clstr.pool1output.port = clstr.local_bus.master
	for i in range(1):
		clstr.pool1.spm = clstr.pool1output.spm_ports

	# conv2 Config
	clstr.conv2.pio = clstr.local_bus.master
	clstr.conv2.enable_debug_msgs = False

	# Conv2Window (Variable)
	addr = 0x10041413
	spmRange = AddrRange(addr, addr + 0xc80)
	clstr.conv2window = ScratchpadMemory(range = spmRange)
	clstr.conv2window.conf_table_reported = False
	clstr.conv2window.ready_mode = False
	clstr.conv2window.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv2.spm = clstr.conv2window.spm_ports

	# Conv2Weights (Variable)
	addr = 0x10042093
	spmRange = AddrRange(addr, addr + 0x5dc0)
	clstr.conv2weights = ScratchpadMemory(range = spmRange)
	clstr.conv2weights.conf_table_reported = False
	clstr.conv2weights.ready_mode = False
	clstr.conv2weights.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv2.spm = clstr.conv2weights.spm_ports

	# Conv2Output (Variable)
	addr = 0x10047e53
	spmRange = AddrRange(addr, addr + 0x3c0)
	clstr.conv2output = ScratchpadMemory(range = spmRange)
	clstr.conv2output.conf_table_reported = False
	clstr.conv2output.ready_mode = False
	clstr.conv2output.port = clstr.local_bus.master
	for i in range(1):
		clstr.conv2.spm = clstr.conv2output.spm_ports

def makeHWAcc(options, system):

	system.lenet5 = AccCluster()
	buildlenet5(options, system, system.lenet5)

