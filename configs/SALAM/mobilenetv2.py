# type: ignore
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

def buildhead(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x100211de
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
	
	# Stream DMA
	clstr.stream_dma0 = StreamDma(pio_addr=0x10020015, pio_size = 32, gic=gic, max_pending = 32)
	clstr.stream_dma0.stream_addr = 0x10020015 + 32
	clstr.stream_dma0.stream_size = 8
	clstr.stream_dma0.pio_delay = '1ns'
	clstr.stream_dma0.rd_int = 210
	clstr.stream_dma0.wr_int = 211
	clstr.stream_dma0.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.stream_dma0.pio
	
	# top Definition
	acc = "top"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/head/top.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/head/top.ll"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x1002003d, pio_size=65, int_num=68)
	AccConfig(clstr.top, config, ir)
	
	# normalconv Definition
	acc = "normalconv"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/head/NormalConv.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/head/NormalConv.ll"
	clstr.normalconv = CommInterface(devicename=acc, gic=gic, pio_addr=0x1002007e, pio_size=1)
	AccConfig(clstr.normalconv, config, ir)
	
	# dwconv Definition
	acc = "dwconv"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/head/DWConv.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/head/DWConv.ll"
	clstr.dwconv = CommInterface(devicename=acc, gic=gic, pio_addr=0x100205f3, pio_size=1)
	AccConfig(clstr.dwconv, config, ir)
	
	# pwconv Definition
	acc = "pwconv"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/head/PWConv.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/head/PWConv.ll"
	clstr.pwconv = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020fe5, pio_size=1)
	AccConfig(clstr.pwconv, config, ir)
	
	# top Config
	clstr.top.local = clstr.local_bus.slave
	clstr.top.pio = clstr.local_bus.master
	clstr.top.enable_debug_msgs = False
	
	# normalconv Config
	clstr.normalconv.pio = clstr.local_bus.master
	clstr.normalconv.stream = clstr.stream_dma0.stream_out
	clstr.normalconv.enable_debug_msgs = False
	
	# NormalConvWindow (Variable)
	addr = 0x1002007f
	spmRange = AddrRange(addr, addr + 0x1b)
	clstr.normalconvwindow = ScratchpadMemory(range = spmRange)
	clstr.normalconvwindow.conf_table_reported = False
	clstr.normalconvwindow.ready_mode = False
	clstr.normalconvwindow.port = clstr.local_bus.master
	
	# Connecting NormalConvWindow to NormalConv
	for i in range(27):
		clstr.normalconv.spm = clstr.normalconvwindow.spm_ports
	
	# NormalConvWeights (Variable)
	addr = 0x1002009a
	spmRange = AddrRange(addr, addr + 0x288)
	clstr.normalconvweights = ScratchpadMemory(range = spmRange)
	clstr.normalconvweights.conf_table_reported = False
	clstr.normalconvweights.ready_mode = False
	clstr.normalconvweights.port = clstr.local_bus.master
	
	# Connecting NormalConvWeights to NormalConv
	for i in range(27):
		clstr.normalconv.spm = clstr.normalconvweights.spm_ports
	
	# NormalConvQParams (Variable)
	addr = 0x10020322
	spmRange = AddrRange(addr, addr + 0x90)
	clstr.normalconvqparams = ScratchpadMemory(range = spmRange)
	clstr.normalconvqparams.conf_table_reported = False
	clstr.normalconvqparams.ready_mode = False
	clstr.normalconvqparams.port = clstr.local_bus.master
	
	# Connecting NormalConvQParams to NormalConv
	for i in range(1):
		clstr.normalconv.spm = clstr.normalconvqparams.spm_ports
	
	# NormalConvQBuffer (Variable)
	addr = 0x100203b2
	spmRange = AddrRange(addr, addr + 0x240)
	clstr.normalconvqbuffer = ScratchpadMemory(range = spmRange)
	clstr.normalconvqbuffer.conf_table_reported = False
	clstr.normalconvqbuffer.ready_mode = False
	clstr.normalconvqbuffer.port = clstr.local_bus.master
	
	# Connecting NormalConvQBuffer to NormalConv
	for i in range(2):
		clstr.normalconv.spm = clstr.normalconvqbuffer.spm_ports
	
	# NormalConvOut (Stream Variable)
	addr = 0x100205f2
	clstr.normalconvout = StreamBuffer(stream_address = addr, stream_size = 1, buffer_size = 8)
	clstr.normalconv.stream = clstr.normalconvout.stream_in
	clstr.dwconv.stream = clstr.normalconvout.stream_out
	
	
	# dwconv Config
	clstr.dwconv.pio = clstr.local_bus.master
	clstr.dwconv.enable_debug_msgs = False
	
	# DWConvWindow (Variable)
	addr = 0x100205f4
	spmRange = AddrRange(addr, addr + 0xd8)
	clstr.dwconvwindow = ScratchpadMemory(range = spmRange)
	clstr.dwconvwindow.conf_table_reported = False
	clstr.dwconvwindow.ready_mode = False
	clstr.dwconvwindow.port = clstr.local_bus.master
	
	# Connecting DWConvWindow to DWConv
	for i in range(9):
		clstr.dwconv.spm = clstr.dwconvwindow.spm_ports
	
	# DWConvWeights (Variable)
	addr = 0x100206cc
	spmRange = AddrRange(addr, addr + 0xf0)
	clstr.dwconvweights = ScratchpadMemory(range = spmRange)
	clstr.dwconvweights.conf_table_reported = False
	clstr.dwconvweights.ready_mode = False
	clstr.dwconvweights.port = clstr.local_bus.master
	
	# Connecting DWConvWeights to DWConv
	for i in range(9):
		clstr.dwconv.spm = clstr.dwconvweights.spm_ports
	
	# DWConvQParams (Variable)
	addr = 0x100207bc
	spmRange = AddrRange(addr, addr + 0x90)
	clstr.dwconvqparams = ScratchpadMemory(range = spmRange)
	clstr.dwconvqparams.conf_table_reported = False
	clstr.dwconvqparams.ready_mode = False
	clstr.dwconvqparams.port = clstr.local_bus.master
	
	# Connecting DWConvQParams to DWConv
	for i in range(1):
		clstr.dwconv.spm = clstr.dwconvqparams.spm_ports
	
	# DWConvQBuffer (Variable)
	addr = 0x1002084c
	spmRange = AddrRange(addr, addr + 0x780)
	clstr.dwconvqbuffer = ScratchpadMemory(range = spmRange)
	clstr.dwconvqbuffer.conf_table_reported = False
	clstr.dwconvqbuffer.ready_mode = False
	clstr.dwconvqbuffer.port = clstr.local_bus.master
	
	# Connecting DWConvQBuffer to DWConv
	for i in range(2):
		clstr.dwconv.spm = clstr.dwconvqbuffer.spm_ports
	
	# DWConvOutBuffer (Variable)
	addr = 0x10020fcc
	spmRange = AddrRange(addr, addr + 0x18)
	clstr.dwconvoutbuffer = ScratchpadMemory(range = spmRange)
	clstr.dwconvoutbuffer.conf_table_reported = False
	clstr.dwconvoutbuffer.ready_mode = False
	clstr.dwconvoutbuffer.port = clstr.local_bus.master
	
	# Connecting DWConvOutBuffer to DWConv
	for i in range(1):
		clstr.dwconv.spm = clstr.dwconvoutbuffer.spm_ports
	
	# DWConvOut (Stream Variable)
	addr = 0x10020fe4
	clstr.dwconvout = StreamBuffer(stream_address = addr, stream_size = 1, buffer_size = 8)
	clstr.dwconv.stream = clstr.dwconvout.stream_in
	clstr.pwconv.stream = clstr.dwconvout.stream_out
	
	
	# pwconv Config
	clstr.pwconv.pio = clstr.local_bus.master
	clstr.pwconv.stream = clstr.stream_dma0.stream_in
	clstr.pwconv.enable_debug_msgs = False
	
	# PWConvLocalFeatSize (Variable)
	addr = 0x10020fe6
	spmRange = AddrRange(addr, addr + 0x18)
	clstr.pwconvlocalfeatsize = ScratchpadMemory(range = spmRange)
	clstr.pwconvlocalfeatsize.conf_table_reported = False
	clstr.pwconvlocalfeatsize.ready_mode = False
	clstr.pwconvlocalfeatsize.port = clstr.local_bus.master
	
	# Connecting PWConvLocalFeatSize to PWConv
	for i in range(24):
		clstr.pwconv.spm = clstr.pwconvlocalfeatsize.spm_ports
	
	# PWConvWeights (Variable)
	addr = 0x10020ffe
	spmRange = AddrRange(addr, addr + 0x180)
	clstr.pwconvweights = ScratchpadMemory(range = spmRange)
	clstr.pwconvweights.conf_table_reported = False
	clstr.pwconvweights.ready_mode = False
	clstr.pwconvweights.port = clstr.local_bus.master
	
	# Connecting PWConvWeights to PWConv
	for i in range(24):
		clstr.pwconv.spm = clstr.pwconvweights.spm_ports
	
	# PWConvQParams (Variable)
	addr = 0x1002117e
	spmRange = AddrRange(addr, addr + 0x60)
	clstr.pwconvqparams = ScratchpadMemory(range = spmRange)
	clstr.pwconvqparams.conf_table_reported = False
	clstr.pwconvqparams.ready_mode = False
	clstr.pwconvqparams.port = clstr.local_bus.master
	
	# Connecting PWConvQParams to PWConv
	for i in range(1):
		clstr.pwconv.spm = clstr.pwconvqparams.spm_ports
	
def buildbody(options, system, clstr):

	local_low = 0x100211df
	local_high = 0x10081824
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.master = clstr.local_bus.slave
	clstr._connect_caches(system, options, l2coherent=False)
	gic = system.realview.gic

	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x100211df, pio_size = 21, gic=gic, int_num=96)
	clstr.dma.cluster_dma = clstr.local_bus.slave
	clstr.dma.max_req_size = 64
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.dma.pio
	
	# Stream DMA
	clstr.stream_dma0 = StreamDma(pio_addr=0x100211f4, pio_size = 32, gic=gic, max_pending = 32)
	clstr.stream_dma0.stream_addr = 0x100211f4 + 32
	clstr.stream_dma0.stream_size = 8
	clstr.stream_dma0.pio_delay = '1ns'
	clstr.stream_dma0.rd_int = 212
	clstr.stream_dma0.wr_int = 213
	clstr.stream_dma0.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.stream_dma0.pio
	
	# Stream DMA
	clstr.stream_dma1 = StreamDma(pio_addr=0x1002121c, pio_size = 32, gic=gic, max_pending = 32)
	clstr.stream_dma1.stream_addr = 0x1002121c + 32
	clstr.stream_dma1.stream_size = 8
	clstr.stream_dma1.pio_delay = '1ns'
	clstr.stream_dma1.rd_int = 214
	clstr.stream_dma1.wr_int = 215
	clstr.stream_dma1.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.stream_dma1.pio
	
	# top Definition
	acc = "top"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/body/top.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/body/top.ll"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x10021244, pio_size=81, int_num=68)
	AccConfig(clstr.top, config, ir)
	
	# residual Definition
	acc = "residual"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/body/Residual.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/body/Residual.ll"
	clstr.residual = CommInterface(devicename=acc, gic=gic, pio_addr=0x10021295, pio_size=17)
	AccConfig(clstr.residual, config, ir)
	
	# pwconv0 Definition
	acc = "pwconv0"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/body/PWConv0.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/body/PWConv0.ll"
	clstr.pwconv0 = CommInterface(devicename=acc, gic=gic, pio_addr=0x100212a7, pio_size=57)
	AccConfig(clstr.pwconv0, config, ir)
	
	# dwconv Definition
	acc = "dwconv"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/body/DWConv.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/body/DWConv.ll"
	clstr.dwconv = CommInterface(devicename=acc, gic=gic, pio_addr=0x100375b9, pio_size=65)
	AccConfig(clstr.dwconv, config, ir)
	
	# pwconv1 Definition
	acc = "pwconv1"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/body/PWConv1.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/body/PWConv1.ll"
	clstr.pwconv1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10056c7b, pio_size=57)
	AccConfig(clstr.pwconv1, config, ir)
	
	# top Config
	clstr.top.local = clstr.local_bus.slave
	clstr.top.pio = clstr.local_bus.master
	clstr.top.enable_debug_msgs = False
	
	# residual Config
	clstr.residual.pio = clstr.local_bus.master
	clstr.residual.stream = clstr.stream_dma0.stream_out
	clstr.residual.stream = clstr.stream_dma1.stream_out
	clstr.residual.enable_debug_msgs = False
	
	# ResidualOut (Stream Variable)
	addr = 0x100212a6
	clstr.residualout = StreamBuffer(stream_address = addr, stream_size = 1, buffer_size = 8)
	clstr.residual.stream = clstr.residualout.stream_in
	clstr.pwconv0.stream = clstr.residualout.stream_out
	
	
	# pwconv0 Config
	clstr.pwconv0.pio = clstr.local_bus.master
	clstr.pwconv0.enable_debug_msgs = False
	
	# PWConv0LocalFeatSize (Variable)
	addr = 0x100212e0
	spmRange = AddrRange(addr, addr + 0x78)
	clstr.pwconv0localfeatsize = ScratchpadMemory(range = spmRange)
	clstr.pwconv0localfeatsize.conf_table_reported = False
	clstr.pwconv0localfeatsize.ready_mode = False
	clstr.pwconv0localfeatsize.port = clstr.local_bus.master
	
	# Connecting PWConv0LocalFeatSize to PWConv0
	for i in range(120):
		clstr.pwconv0.spm = clstr.pwconv0localfeatsize.spm_ports
	
	# PWConv0Weights (Variable)
	addr = 0x10021358
	spmRange = AddrRange(addr, addr + 0x15180)
	clstr.pwconv0weights = ScratchpadMemory(range = spmRange)
	clstr.pwconv0weights.conf_table_reported = False
	clstr.pwconv0weights.ready_mode = False
	clstr.pwconv0weights.port = clstr.local_bus.master
	
	# Connecting PWConv0Weights to PWConv0
	for i in range(120):
		clstr.pwconv0.spm = clstr.pwconv0weights.spm_ports
	
	# PWConv0QParams (Variable)
	addr = 0x100364d8
	spmRange = AddrRange(addr, addr + 0x10e0)
	clstr.pwconv0qparams = ScratchpadMemory(range = spmRange)
	clstr.pwconv0qparams.conf_table_reported = False
	clstr.pwconv0qparams.ready_mode = False
	clstr.pwconv0qparams.port = clstr.local_bus.master
	
	# Connecting PWConv0QParams to PWConv0
	for i in range(1):
		clstr.pwconv0.spm = clstr.pwconv0qparams.spm_ports
	
	# PWConv0Out (Stream Variable)
	addr = 0x100375b8
	clstr.pwconv0out = StreamBuffer(stream_address = addr, stream_size = 1, buffer_size = 8)
	clstr.pwconv0.stream = clstr.pwconv0out.stream_in
	clstr.dwconv.stream = clstr.pwconv0out.stream_out
	
	
	# dwconv Config
	clstr.dwconv.pio = clstr.local_bus.master
	clstr.dwconv.enable_debug_msgs = False
	
	# DWConvWindow (Variable)
	addr = 0x100375fa
	spmRange = AddrRange(addr, addr + 0x1c20)
	clstr.dwconvwindow = ScratchpadMemory(range = spmRange)
	clstr.dwconvwindow.conf_table_reported = False
	clstr.dwconvwindow.ready_mode = False
	clstr.dwconvwindow.port = clstr.local_bus.master
	
	# Connecting DWConvWindow to DWConv
	for i in range(9):
		clstr.dwconv.spm = clstr.dwconvwindow.spm_ports
	
	# DWConvWeights (Variable)
	addr = 0x1003921a
	spmRange = AddrRange(addr, addr + 0x4b0)
	clstr.dwconvweights = ScratchpadMemory(range = spmRange)
	clstr.dwconvweights.conf_table_reported = False
	clstr.dwconvweights.ready_mode = False
	clstr.dwconvweights.port = clstr.local_bus.master
	
	# Connecting DWConvWeights to DWConv
	for i in range(9):
		clstr.dwconv.spm = clstr.dwconvweights.spm_ports
	
	# DWConvQParams (Variable)
	addr = 0x100396ca
	spmRange = AddrRange(addr, addr + 0x10e0)
	clstr.dwconvqparams = ScratchpadMemory(range = spmRange)
	clstr.dwconvqparams.conf_table_reported = False
	clstr.dwconvqparams.ready_mode = False
	clstr.dwconvqparams.port = clstr.local_bus.master
	
	# Connecting DWConvQParams to DWConv
	for i in range(1):
		clstr.dwconv.spm = clstr.dwconvqparams.spm_ports
	
	# DWConvBuffer (Variable)
	addr = 0x1003a7aa
	spmRange = AddrRange(addr, addr + 0x1c200)
	clstr.dwconvbuffer = ScratchpadMemory(range = spmRange)
	clstr.dwconvbuffer.conf_table_reported = False
	clstr.dwconvbuffer.ready_mode = False
	clstr.dwconvbuffer.port = clstr.local_bus.master
	
	# Connecting DWConvBuffer to DWConv
	for i in range(2):
		clstr.dwconv.spm = clstr.dwconvbuffer.spm_ports
	
	# DWConvOutBuffer (Variable)
	addr = 0x100569aa
	spmRange = AddrRange(addr, addr + 0x2d0)
	clstr.dwconvoutbuffer = ScratchpadMemory(range = spmRange)
	clstr.dwconvoutbuffer.conf_table_reported = False
	clstr.dwconvoutbuffer.ready_mode = False
	clstr.dwconvoutbuffer.port = clstr.local_bus.master
	
	# Connecting DWConvOutBuffer to DWConv
	for i in range(1):
		clstr.dwconv.spm = clstr.dwconvoutbuffer.spm_ports
	
	# DWConvOut (Stream Variable)
	addr = 0x10056c7a
	clstr.dwconvout = StreamBuffer(stream_address = addr, stream_size = 1, buffer_size = 8)
	clstr.dwconv.stream = clstr.dwconvout.stream_in
	clstr.pwconv1.stream = clstr.dwconvout.stream_out
	
	
	# pwconv1 Config
	clstr.pwconv1.pio = clstr.local_bus.master
	clstr.pwconv1.stream = clstr.stream_dma0.stream_in
	clstr.pwconv1.enable_debug_msgs = False
	
	# PWConv1LocalFeatSize (Variable)
	addr = 0x10056cb4
	spmRange = AddrRange(addr, addr + 0x2d0)
	clstr.pwconv1localfeatsize = ScratchpadMemory(range = spmRange)
	clstr.pwconv1localfeatsize.conf_table_reported = False
	clstr.pwconv1localfeatsize.ready_mode = False
	clstr.pwconv1localfeatsize.port = clstr.local_bus.master
	
	# Connecting PWConv1LocalFeatSize to PWConv1
	for i in range(120):
		clstr.pwconv1.spm = clstr.pwconv1localfeatsize.spm_ports
	
	# PWConv1Weights (Variable)
	addr = 0x10056f84
	spmRange = AddrRange(addr, addr + 0x2a300)
	clstr.pwconv1weights = ScratchpadMemory(range = spmRange)
	clstr.pwconv1weights.conf_table_reported = False
	clstr.pwconv1weights.ready_mode = False
	clstr.pwconv1weights.port = clstr.local_bus.master
	
	# Connecting PWConv1Weights to PWConv1
	for i in range(120):
		clstr.pwconv1.spm = clstr.pwconv1weights.spm_ports
	
	# PWConv1QParams (Variable)
	addr = 0x10081284
	spmRange = AddrRange(addr, addr + 0x5a0)
	clstr.pwconv1qparams = ScratchpadMemory(range = spmRange)
	clstr.pwconv1qparams.conf_table_reported = False
	clstr.pwconv1qparams.ready_mode = False
	clstr.pwconv1qparams.port = clstr.local_bus.master
	
	# Connecting PWConv1QParams to PWConv1
	for i in range(1):
		clstr.pwconv1.spm = clstr.pwconv1qparams.spm_ports
	
def buildtail(options, system, clstr):

	local_low = 0x10081825
	local_high = 0x100d6497
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.master = clstr.local_bus.slave
	clstr._connect_caches(system, options, l2coherent=False)
	gic = system.realview.gic

	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x10081825, pio_size = 21, gic=gic, int_num=95)
	clstr.dma.cluster_dma = clstr.local_bus.slave
	clstr.dma.max_req_size = 64
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.dma.pio
	
	# Stream DMA
	clstr.stream_dma0 = StreamDma(pio_addr=0x1008183a, pio_size = 32, gic=gic, max_pending = 32)
	clstr.stream_dma0.stream_addr = 0x1008183a + 32
	clstr.stream_dma0.stream_size = 8
	clstr.stream_dma0.pio_delay = '1ns'
	clstr.stream_dma0.rd_int = 210
	clstr.stream_dma0.wr_int = 211
	clstr.stream_dma0.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.stream_dma0.pio
	
	# top Definition
	acc = "top"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/tail/top.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/tail/top.ll"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x10081862, pio_size=65, int_num=68)
	AccConfig(clstr.top, config, ir)
	
	# pwconv Definition
	acc = "pwconv"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/tail/PWConv.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/tail/PWConv.ll"
	clstr.pwconv = CommInterface(devicename=acc, gic=gic, pio_addr=0x100818a3, pio_size=1)
	AccConfig(clstr.pwconv, config, ir)
	
	# reshape Definition
	acc = "reshape"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/tail/Reshape.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/tail/Reshape.ll"
	clstr.reshape = CommInterface(devicename=acc, gic=gic, pio_addr=0x100ce795, pio_size=1)
	AccConfig(clstr.reshape, config, ir)
	
	# avgpool Definition
	acc = "avgpool"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/tail/AvgPool.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/tail/AvgPool.ll"
	clstr.avgpool = CommInterface(devicename=acc, gic=gic, pio_addr=0x100d6496, pio_size=1)
	AccConfig(clstr.avgpool, config, ir)
	
	# top Config
	clstr.top.local = clstr.local_bus.slave
	clstr.top.pio = clstr.local_bus.master
	clstr.top.enable_debug_msgs = False
	
	# pwconv Config
	clstr.pwconv.pio = clstr.local_bus.master
	clstr.pwconv.stream = clstr.stream_dma0.stream_out
	clstr.pwconv.enable_debug_msgs = False
	
	# PWConvLocalFeatSize (Variable)
	addr = 0x100818a4
	spmRange = AddrRange(addr, addr + 0xf0)
	clstr.pwconvlocalfeatsize = ScratchpadMemory(range = spmRange)
	clstr.pwconvlocalfeatsize.conf_table_reported = False
	clstr.pwconvlocalfeatsize.ready_mode = False
	clstr.pwconvlocalfeatsize.port = clstr.local_bus.master
	
	# Connecting PWConvLocalFeatSize to PWConv
	for i in range(24):
		clstr.pwconv.spm = clstr.pwconvlocalfeatsize.spm_ports
	
	# PWConvWeights (Variable)
	addr = 0x10081994
	spmRange = AddrRange(addr, addr + 0x4b000)
	clstr.pwconvweights = ScratchpadMemory(range = spmRange)
	clstr.pwconvweights.conf_table_reported = False
	clstr.pwconvweights.ready_mode = False
	clstr.pwconvweights.port = clstr.local_bus.master
	
	# Connecting PWConvWeights to PWConv
	for i in range(24):
		clstr.pwconv.spm = clstr.pwconvweights.spm_ports
	
	# PWConvQParams (Variable)
	addr = 0x100cc994
	spmRange = AddrRange(addr, addr + 0x1e00)
	clstr.pwconvqparams = ScratchpadMemory(range = spmRange)
	clstr.pwconvqparams.conf_table_reported = False
	clstr.pwconvqparams.ready_mode = False
	clstr.pwconvqparams.port = clstr.local_bus.master
	
	# Connecting PWConvQParams to PWConv
	for i in range(1):
		clstr.pwconv.spm = clstr.pwconvqparams.spm_ports
	
	# PWConvOut (Stream Variable)
	addr = 0x100ce794
	clstr.pwconvout = StreamBuffer(stream_address = addr, stream_size = 1, buffer_size = 8)
	clstr.pwconv.stream = clstr.pwconvout.stream_in
	clstr.reshape.stream = clstr.pwconvout.stream_out
	
	
	# reshape Config
	clstr.reshape.pio = clstr.local_bus.master
	clstr.reshape.enable_debug_msgs = False
	
	# ReshapeOut (Variable)
	addr = 0x100ce796
	spmRange = AddrRange(addr, addr + 0x7d00)
	clstr.reshapeout = ScratchpadMemory(range = spmRange)
	clstr.reshapeout.conf_table_reported = False
	clstr.reshapeout.ready_mode = False
	clstr.reshapeout.port = clstr.local_bus.master
	
	# Connecting ReshapeOut to Reshape
	for i in range(1):
		clstr.reshape.spm = clstr.reshapeout.spm_ports
	
	# Connecting ReshapeOut to AvgPool
	for i in range(1):
		clstr.avgpool.spm = clstr.reshapeout.spm_ports
	
	# avgpool Config
	clstr.avgpool.pio = clstr.local_bus.master
	clstr.avgpool.stream = clstr.stream_dma0.stream_in
	clstr.avgpool.enable_debug_msgs = False
	
def buildclassifier(options, system, clstr):

	local_low = 0x100d6498
	local_high = 0x10210d6f
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.master = clstr.local_bus.slave
	clstr._connect_caches(system, options, l2coherent=False)
	gic = system.realview.gic

	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x100d6498, pio_size = 21, gic=gic, int_num=95)
	clstr.dma.cluster_dma = clstr.local_bus.slave
	clstr.dma.max_req_size = 64
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.dma.pio
	
	# Stream DMA
	clstr.stream_dma0 = StreamDma(pio_addr=0x100d64ad, pio_size = 32, gic=gic, max_pending = 32)
	clstr.stream_dma0.stream_addr = 0x100d64ad + 32
	clstr.stream_dma0.stream_size = 8
	clstr.stream_dma0.pio_delay = '1ns'
	clstr.stream_dma0.rd_int = 210
	clstr.stream_dma0.wr_int = 211
	clstr.stream_dma0.dma = clstr.coherency_bus.slave
	clstr.local_bus.master = clstr.stream_dma0.pio
	
	# top Definition
	acc = "top"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/classifier/top.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/classifier/top.ll"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x100d64d5, pio_size=65, int_num=68)
	AccConfig(clstr.top, config, ir)
	
	# linear Definition
	acc = "linear"
	config = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/configs/classifier/Linear.ini"
	ir = "/home/he-man/gem5-SALAM/benchmarks/mobilenetv2/hw/ir/classifier/Linear.ll"
	clstr.linear = CommInterface(devicename=acc, gic=gic, pio_addr=0x100d6516, pio_size=1)
	AccConfig(clstr.linear, config, ir)
	
	# top Config
	clstr.top.local = clstr.local_bus.slave
	clstr.top.pio = clstr.local_bus.master
	clstr.top.enable_debug_msgs = False
	
	# linear Config
	clstr.linear.local = clstr.local_bus.slave
	clstr.linear.pio = clstr.local_bus.master
	clstr.linear.stream = clstr.stream_dma0.stream_in
	clstr.linear.enable_debug_msgs = False
	
	# LinearFeats (Variable)
	addr = 0x100d6517
	spmRange = AddrRange(addr, addr + 0x500)
	clstr.linearfeats = ScratchpadMemory(range = spmRange)
	clstr.linearfeats.conf_table_reported = False
	clstr.linearfeats.ready_mode = False
	clstr.linearfeats.port = clstr.local_bus.master
	
	# Connecting LinearFeats to Linear
	for i in range(1):
		clstr.linear.spm = clstr.linearfeats.spm_ports
	
	# LinearWeights (Variable)
	addr = 0x100d6a17
	spmRange = AddrRange(addr, addr + 0x138800)
	clstr.linearweights = ScratchpadMemory(range = spmRange)
	clstr.linearweights.conf_table_reported = False
	clstr.linearweights.ready_mode = False
	clstr.linearweights.port = clstr.local_bus.master
	
	# Connecting LinearWeights to Linear
	for i in range(1):
		clstr.linear.spm = clstr.linearweights.spm_ports
	
	# LinearQParams (Variable)
	addr = 0x1020f217
	spmRange = AddrRange(addr, addr + 0x1770)
	clstr.linearqparams = ScratchpadMemory(range = spmRange)
	clstr.linearqparams.conf_table_reported = False
	clstr.linearqparams.ready_mode = False
	clstr.linearqparams.port = clstr.local_bus.master
	
	# Connecting LinearQParams to Linear
	for i in range(1):
		clstr.linear.spm = clstr.linearqparams.spm_ports
	
	# LinearSum (Variable)
	addr = 0x10210987
	spmRange = AddrRange(addr, addr + 0x3e8)
	clstr.linearsum = ScratchpadMemory(range = spmRange)
	clstr.linearsum.conf_table_reported = False
	clstr.linearsum.ready_mode = False
	clstr.linearsum.port = clstr.local_bus.master
	
	# Connecting LinearSum to Linear
	for i in range(1):
		clstr.linear.spm = clstr.linearsum.spm_ports
	
def makeHWAcc(options, system):

	system.head = AccCluster()
	buildhead(options, system, system.head)

	system.body = AccCluster()
	buildbody(options, system, system.body)

	system.tail = AccCluster()
	buildtail(options, system, system.tail)

	system.classifier = AccCluster()
	buildclassifier(options, system, system.classifier)

