import m5
from m5.objects import *
from m5.util import *
from configparser import ConfigParser
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

def buildriscvtest_clstr(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x10020040
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.mem_side_ports = clstr.local_bus.cpu_side_ports
	clstr._connect_caches(system, options, l2coherent=False)
	gic = system.realview.gic

	# top Definition
	acc = "top"
	ir = "/home/samerogers/WorkingDir/gem5-SALAM/benchmarks/test-cases/geptest/hw/top.ll"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020000, pio_size=64, int_num=68)
	AccConfig(clstr.top, ir)
	
	# top Config
	clstr.top.pio = clstr.local_bus.mem_side_ports
	clstr.top.enable_debug_msgs = False
	
def makeHWAcc(args, system):

	system.riscvtest_clstr = AccCluster()
	buildriscvtest_clstr(args, system, system.riscvtest_clstr)

