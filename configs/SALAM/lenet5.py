import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *

def buildlenet5(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x1006bedc
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
	clstr.pool0 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10025bfb, pio_size=1)
	AccConfig(clstr.pool0, config, ir)
	
	# conv1 Definition
	acc = "conv1"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv1.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/conv1.ll"
	clstr.conv1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x1002b7dc, pio_size=1)
	AccConfig(clstr.conv1, config, ir)
	
	# pool1 Definition
	acc = "pool1"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/pool1.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/pool1.ll"
	clstr.pool1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x100308c1, pio_size=1)
	AccConfig(clstr.pool1, config, ir)
	
	# fc0 Definition
	acc = "fc0"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/fc0.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/fc0.ll"
	clstr.fc0 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10032802, pio_size=1)
	AccConfig(clstr.fc0, config, ir)
	
	# fc1 Definition
	acc = "fc1"
	config = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/fc1.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/lenet5/hw/fc1.ll"
	clstr.fc1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10061e27, pio_size=1)
	AccConfig(clstr.fc1, config, ir)
	
	# top Config
	clstr.top.local = clstr.local_bus.slave
	clstr.top.pio = clstr.local_bus.master
	clstr.top.enable_debug_msgs = False
	
	# conv0 Config
	clstr.conv0.pio = clstr.local_bus.master
	clstr.conv0.enable_debug_msgs = False
	
	# Conv0Input (Variable)
	addr = 0x1002001f
	spmRange = AddrRange(addr, addr + 0x1000)
	clstr.conv0input = ScratchpadMemory(range = spmRange)
	clstr.conv0input.conf_table_reported = False
	clstr.conv0input.ready_mode = False
	clstr.conv0input.port = clstr.local_bus.master
	for i in range(150):
		clstr.conv0.spm = clstr.conv0input.spm_ports
	
	# Conv0Weights (Variable)
	addr = 0x1002101f
	spmRange = AddrRange(addr, addr + 0x258)
	clstr.conv0weights = ScratchpadMemory(range = spmRange)
	clstr.conv0weights.conf_table_reported = False
	clstr.conv0weights.ready_mode = False
	clstr.conv0weights.port = clstr.local_bus.master
	for i in range(150):
		clstr.conv0.spm = clstr.conv0weights.spm_ports
	
	# Conv0Output (Variable)
	addr = 0x10021277
	spmRange = AddrRange(addr, addr + 0x4980)
	clstr.conv0output = ScratchpadMemory(range = spmRange)
	clstr.conv0output.conf_table_reported = False
	clstr.conv0output.ready_mode = False
	clstr.conv0output.port = clstr.local_bus.master
	for i in range(6):
		clstr.conv0.spm = clstr.conv0output.spm_ports
	
	# Conv0LUT (Variable)
	addr = 0x10025bf7
	spmRange = AddrRange(addr, addr + 0x4)
	clstr.conv0lut = ScratchpadMemory(range = spmRange)
	clstr.conv0lut.conf_table_reported = False
	clstr.conv0lut.ready_mode = False
	clstr.conv0lut.port = clstr.local_bus.master
	for i in range(6):
		clstr.conv0.spm = clstr.conv0lut.spm_ports
	
	# pool0 Config
	clstr.pool0.pio = clstr.local_bus.master
	clstr.pool0.enable_debug_msgs = False
	
	# pool0Input (Variable)
	addr = 0x10025bfc
	spmRange = AddrRange(addr, addr + 0x4980)
	clstr.pool0input = ScratchpadMemory(range = spmRange)
	clstr.pool0input.conf_table_reported = False
	clstr.pool0input.ready_mode = False
	clstr.pool0input.port = clstr.local_bus.master
	for i in range(24):
		clstr.pool0.spm = clstr.pool0input.spm_ports
	
	# pool0Output (Variable)
	addr = 0x1002a57c
	spmRange = AddrRange(addr, addr + 0x1260)
	clstr.pool0output = ScratchpadMemory(range = spmRange)
	clstr.pool0output.conf_table_reported = False
	clstr.pool0output.ready_mode = False
	clstr.pool0output.port = clstr.local_bus.master
	for i in range(24):
		clstr.pool0.spm = clstr.pool0output.spm_ports
	
	# conv1 Config
	clstr.conv1.pio = clstr.local_bus.master
	clstr.conv1.enable_debug_msgs = False
	
	# Conv1Input (Variable)
	addr = 0x1002b7dd
	spmRange = AddrRange(addr, addr + 0x1260)
	clstr.conv1input = ScratchpadMemory(range = spmRange)
	clstr.conv1input.conf_table_reported = False
	clstr.conv1input.ready_mode = False
	clstr.conv1input.port = clstr.local_bus.master
	for i in range(150):
		clstr.conv1.spm = clstr.conv1input.spm_ports
	
	# Conv1Weights (Variable)
	addr = 0x1002ca3d
	spmRange = AddrRange(addr, addr + 0x2580)
	clstr.conv1weights = ScratchpadMemory(range = spmRange)
	clstr.conv1weights.conf_table_reported = False
	clstr.conv1weights.ready_mode = False
	clstr.conv1weights.port = clstr.local_bus.master
	for i in range(150):
		clstr.conv1.spm = clstr.conv1weights.spm_ports
	
	# Conv1Output (Variable)
	addr = 0x1002efbd
	spmRange = AddrRange(addr, addr + 0x1900)
	clstr.conv1output = ScratchpadMemory(range = spmRange)
	clstr.conv1output.conf_table_reported = False
	clstr.conv1output.ready_mode = False
	clstr.conv1output.port = clstr.local_bus.master
	for i in range(16):
		clstr.conv1.spm = clstr.conv1output.spm_ports
	
	# Conv1LUT (Variable)
	addr = 0x100308bd
	spmRange = AddrRange(addr, addr + 0x4)
	clstr.conv1lut = ScratchpadMemory(range = spmRange)
	clstr.conv1lut.conf_table_reported = False
	clstr.conv1lut.ready_mode = False
	clstr.conv1lut.port = clstr.local_bus.master
	for i in range(16):
		clstr.conv1.spm = clstr.conv1lut.spm_ports
	
	# pool1 Config
	clstr.pool1.pio = clstr.local_bus.master
	clstr.pool1.enable_debug_msgs = False
	
	# pool1Input (Variable)
	addr = 0x100308c2
	spmRange = AddrRange(addr, addr + 0x1900)
	clstr.pool1input = ScratchpadMemory(range = spmRange)
	clstr.pool1input.conf_table_reported = False
	clstr.pool1input.ready_mode = False
	clstr.pool1input.port = clstr.local_bus.master
	for i in range(64):
		clstr.pool1.spm = clstr.pool1input.spm_ports
	
	# pool1Output (Variable)
	addr = 0x100321c2
	spmRange = AddrRange(addr, addr + 0x640)
	clstr.pool1output = ScratchpadMemory(range = spmRange)
	clstr.pool1output.conf_table_reported = False
	clstr.pool1output.ready_mode = False
	clstr.pool1output.port = clstr.local_bus.master
	for i in range(64):
		clstr.pool1.spm = clstr.pool1output.spm_ports
	
	# fc0 Config
	clstr.fc0.pio = clstr.local_bus.master
	clstr.fc0.enable_debug_msgs = False
	
	# fc0Input (Variable)
	addr = 0x10032803
	spmRange = AddrRange(addr, addr + 0x640)
	clstr.fc0input = ScratchpadMemory(range = spmRange)
	clstr.fc0input.conf_table_reported = False
	clstr.fc0input.ready_mode = False
	clstr.fc0input.port = clstr.local_bus.master
	for i in range(80):
		clstr.fc0.spm = clstr.fc0input.spm_ports
	
	# fc0Weights (Variable)
	addr = 0x10032e43
	spmRange = AddrRange(addr, addr + 0x2ee00)
	clstr.fc0weights = ScratchpadMemory(range = spmRange)
	clstr.fc0weights.conf_table_reported = False
	clstr.fc0weights.ready_mode = False
	clstr.fc0weights.port = clstr.local_bus.master
	for i in range(80):
		clstr.fc0.spm = clstr.fc0weights.spm_ports
	
	# fc0Output (Variable)
	addr = 0x10061c43
	spmRange = AddrRange(addr, addr + 0x1e0)
	clstr.fc0output = ScratchpadMemory(range = spmRange)
	clstr.fc0output.conf_table_reported = False
	clstr.fc0output.ready_mode = False
	clstr.fc0output.port = clstr.local_bus.master
	for i in range(1):
		clstr.fc0.spm = clstr.fc0output.spm_ports
	
	# fc0LUT (Variable)
	addr = 0x10061e23
	spmRange = AddrRange(addr, addr + 0x4)
	clstr.fc0lut = ScratchpadMemory(range = spmRange)
	clstr.fc0lut.conf_table_reported = False
	clstr.fc0lut.ready_mode = False
	clstr.fc0lut.port = clstr.local_bus.master
	for i in range(1):
		clstr.fc0.spm = clstr.fc0lut.spm_ports
	
	# fc1 Config
	clstr.fc1.pio = clstr.local_bus.master
	clstr.fc1.enable_debug_msgs = False
	
	# fc1Input (Variable)
	addr = 0x10061e28
	spmRange = AddrRange(addr, addr + 0x1e0)
	clstr.fc1input = ScratchpadMemory(range = spmRange)
	clstr.fc1input.conf_table_reported = False
	clstr.fc1input.ready_mode = False
	clstr.fc1input.port = clstr.local_bus.master
	for i in range(120):
		clstr.fc1.spm = clstr.fc1input.spm_ports
	
	# fc1Weights (Variable)
	addr = 0x10062008
	spmRange = AddrRange(addr, addr + 0x9d80)
	clstr.fc1weights = ScratchpadMemory(range = spmRange)
	clstr.fc1weights.conf_table_reported = False
	clstr.fc1weights.ready_mode = False
	clstr.fc1weights.port = clstr.local_bus.master
	for i in range(120):
		clstr.fc1.spm = clstr.fc1weights.spm_ports
	
	# fc1Output (Variable)
	addr = 0x1006bd88
	spmRange = AddrRange(addr, addr + 0x150)
	clstr.fc1output = ScratchpadMemory(range = spmRange)
	clstr.fc1output.conf_table_reported = False
	clstr.fc1output.ready_mode = False
	clstr.fc1output.port = clstr.local_bus.master
	for i in range(1):
		clstr.fc1.spm = clstr.fc1output.spm_ports
	
	# fc1LUT (Variable)
	addr = 0x1006bed8
	spmRange = AddrRange(addr, addr + 0x4)
	clstr.fc1lut = ScratchpadMemory(range = spmRange)
	clstr.fc1lut.conf_table_reported = False
	clstr.fc1lut.ready_mode = False
	clstr.fc1lut.port = clstr.local_bus.master
	for i in range(1):
		clstr.fc1.spm = clstr.fc1lut.spm_ports
	
def makeHWAcc(options, system):

	system.lenet5 = AccCluster()
	buildlenet5(options, system, system.lenet5)

