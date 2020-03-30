import m5
from m5.objects import *
from m5.util import *
import ConfigParser
from HWAccConfig import *

def buildHead(options, system, clstr):
    # Specify the path to the mobilenet accelerator descriptions
    hw_path = options.accpath + "/mobilenetv2/hw"
    hw_config_path = hw_path + "/configs/head/"
    hw_ir_path = hw_path + "/ir/head/"
    local_low = 0x2F000000
    local_high = 0x2F001ADE
    local_range = AddrRange(local_low, local_high)
    external_range = [AddrRange(0x00000000, local_low-1),
                      AddrRange(local_high+1, 0xFFFFFFFF)]
    clstr._attach_bridges(system, local_range, external_range)
    clstr._connect_caches(system, options)
    gic = system.realview.gic

    # Add the top function
    acc = "top"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.top = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.top, config, ir)
    clstr._connect_hwacc(clstr.top)
    clstr.top.local = clstr.local_bus.slave

    # Add the Stream DMAs
    addr = local_low + 0x0041
    clstr.stream_dma0 = StreamDma(pio_addr=addr, pio_size=32, gic=gic, max_pending=32)
    clstr.stream_dma0.stream_addr=addr+32
    clstr.stream_dma0.stream_size=8
    clstr.stream_dma0.pio_delay='1ns'
    clstr.stream_dma0.rd_int = 210
    clstr.stream_dma0.wr_int = 211
    clstr._connect_dma(system, clstr.stream_dma0)

    # Add the cluster DMA
    addr = local_low + 0x00069
    clstr.dma = NoncoherentDma(pio_addr=addr, pio_size=21, gic=gic, int_num=95)
    clstr._connect_cluster_dma(system, clstr.dma)

    # Add the Normal Convolution
    acc = "NormalConv"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.NormalConv = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.NormalConv, config, ir)
    clstr._connect_hwacc(clstr.NormalConv)
    clstr.NormalConv.stream = clstr.stream_dma0.stream_out

    addr = local_low + 0x0081
    spmRange = AddrRange(addr, addr+(160*2*3))
    clstr.NormalConvBuffer = ScratchpadMemory(range=spmRange)
    clstr.NormalConvBuffer.conf_table_reported = False
    clstr.NormalConvBuffer.ready_mode=True
    clstr.NormalConvBuffer.port = clstr.local_bus.master
    for i in range(1):
        clstr.NormalConv.spm = clstr.NormalConvBuffer.spm_ports

    addr = local_low + 0x0441
    spmRange = AddrRange(addr, addr+27)
    clstr.NormalConvWindow = ScratchpadMemory(range=spmRange)
    clstr.NormalConvWindow.conf_table_reported = False
    clstr.NormalConvWindow.ready_mode=True
    clstr.NormalConvWindow.port = clstr.local_bus.master
    for i in range(1):
        clstr.NormalConv.spm = clstr.NormalConvWindow.spm_ports

    addr = local_low + 0x045C
    spmRange = AddrRange(addr, addr+(24*27))
    clstr.NormalConvWeights = ScratchpadMemory(range=spmRange)
    clstr.NormalConvWeights.conf_table_reported = False
    clstr.NormalConvWeights.ready_mode=True
    clstr.NormalConvWeights.port = clstr.local_bus.master
    for i in range(1):
        clstr.NormalConv.spm = clstr.NormalConvWeights.spm_ports

    addr = local_low + 0x06E4
    spmRange = AddrRange(addr, addr+(24*6))
    clstr.NormalConvQParams = ScratchpadMemory(range=spmRange)
    clstr.NormalConvQParams.conf_table_reported = False
    clstr.NormalConvQParams.ready_mode=True
    clstr.NormalConvQParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.NormalConv.spm = clstr.NormalConvQParams.spm_ports

    addr = local_low + 0x0774
    clstr.NormalConvOut = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.NormalConv.stream = clstr.NormalConvOut.stream_in

    # Add the Depthwise Convolution
    acc = "DWConv"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.DWConv = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.DWConv, config, ir)
    clstr._connect_hwacc(clstr.DWConv)
    clstr.DWConv.stream = clstr.NormalConvOut.stream_out

    addr = local_low + 0x0775
    spmRange = AddrRange(addr, addr+(80*2*24))
    clstr.DWConvBuffer = ScratchpadMemory(range=spmRange)
    clstr.DWConvBuffer.conf_table_reported = False
    clstr.DWConvBuffer.ready_mode=True
    clstr.DWConvBuffer.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvBuffer.spm_ports

    addr = local_low + 0x1675
    spmRange = AddrRange(addr, addr+(9*24))
    clstr.DWConvWindow = ScratchpadMemory(range=spmRange)
    clstr.DWConvWindow.conf_table_reported = False
    clstr.DWConvWindow.ready_mode=True
    clstr.DWConvWindow.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvWindow.spm_ports

    addr = local_low + 0x174D
    spmRange = AddrRange(addr, addr+24)
    clstr.DWConvOutBuffer = ScratchpadMemory(range=spmRange)
    clstr.DWConvOutBuffer.conf_table_reported = False
    clstr.DWConvOutBuffer.ready_mode=True
    clstr.DWConvOutBuffer.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvOutBuffer.spm_ports

    addr = local_low + 0x1765
    spmRange = AddrRange(addr, addr+(24*10))
    clstr.DWConvWeights = ScratchpadMemory(range=spmRange)
    clstr.DWConvWeights.conf_table_reported = False
    clstr.DWConvWeights.ready_mode=True
    clstr.DWConvWeights.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvWeights.spm_ports

    addr = local_low + 0x1855
    spmRange = AddrRange(addr, addr+(24*6))
    clstr.DWConvQParams = ScratchpadMemory(range=spmRange)
    clstr.DWConvQParams.conf_table_reported = False
    clstr.DWConvQParams.ready_mode=True
    clstr.DWConvQParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvQParams.spm_ports

    addr = local_low + 0x18E5
    clstr.DWConvOut = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.DWConv.stream = clstr.DWConvOut.stream_in

    # Add the Pointwise Convolution
    acc = "PWConv"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.PWConv = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.PWConv, config, ir)
    clstr._connect_hwacc(clstr.PWConv)
    clstr.PWConv.stream = clstr.DWConvOut.stream_out

    addr = local_low + 0x18E6
    spmRange = AddrRange(addr, addr+24)
    clstr.PWConvLocalFeat = ScratchpadMemory(range=spmRange)
    clstr.PWConvLocalFeat.conf_table_reported = False
    clstr.PWConvLocalFeat.ready_mode=True
    clstr.PWConvLocalFeat.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv.spm = clstr.PWConvLocalFeat.spm_ports

    addr = local_low + 0x18FE
    spmRange = AddrRange(addr, addr+(24*16))
    clstr.PWConvWeights = ScratchpadMemory(range=spmRange)
    clstr.PWConvWeights.conf_table_reported = False
    clstr.PWConvWeights.ready_mode=True
    clstr.PWConvWeights.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv.spm = clstr.PWConvWeights.spm_ports

    addr = local_low + 0x1A7E
    spmRange = AddrRange(addr, addr+(16*6))
    clstr.PWConvQParams = ScratchpadMemory(range=spmRange)
    clstr.PWConvQParams.conf_table_reported = False
    clstr.PWConvQParams.ready_mode=True
    clstr.PWConvQParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv.spm = clstr.PWConvQParams.spm_ports

    clstr.PWConv.stream = clstr.stream_dma0.stream_in

def buildBody(options, system, clstr):
    # Specify the path to the mobilenet accelerator descriptions
    hw_path = options.accpath + "/mobilenetv2/hw"
    hw_config_path = hw_path + "/configs/body/"
    hw_ir_path = hw_path + "/ir/body/"
    local_low = 0x2F002000
    local_high = 0x2F0623E6
    local_range = AddrRange(local_low, local_high)
    external_range = [AddrRange(0x00000000, local_low-1),
                      AddrRange(local_high+1, 0xFFFFFFFF)]
    clstr._attach_bridges(system, local_range, external_range)
    clstr._connect_caches(system, options)
    gic = system.realview.gic

    # Add the top function
    acc = "top"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.top = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.top, config, ir)
    clstr._connect_hwacc(clstr.top)
    clstr.top.local = clstr.local_bus.slave

    # Add the Stream DMAs
    addr = local_low + 0x004A
    clstr.stream_dma0 = StreamDma(pio_addr=addr, pio_size=32, gic=gic, max_pending=32)
    clstr.stream_dma0.stream_addr=addr+32
    clstr.stream_dma0.stream_size=8
    clstr.stream_dma0.pio_delay='1ns'
    clstr.stream_dma0.rd_int = 212
    clstr.stream_dma0.wr_int = 213
    clstr._connect_dma(system, clstr.stream_dma0)

    addr = local_low + 0x0072
    clstr.stream_dma1 = StreamDma(pio_addr=addr, pio_size=32, gic=gic, max_pending=32)
    clstr.stream_dma1.stream_addr=addr+32
    clstr.stream_dma1.stream_size=8
    clstr.stream_dma1.pio_delay='1ns'
    clstr.stream_dma1.rd_int = 214
    clstr.stream_dma1.wr_int = 215
    clstr._connect_dma(system, clstr.stream_dma1)

    # Add the cluster DMA
    addr = local_low + 0x0009A
    clstr.dma = NoncoherentDma(pio_addr=addr, pio_size=21, gic=gic, int_num=96)
    clstr._connect_cluster_dma(system, clstr.dma)

    # Add the Residual Vector Add
    acc = "Residual"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.Residual = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.Residual, config, ir)
    clstr._connect_hwacc(clstr.Residual)
    clstr.Residual.stream = clstr.stream_dma0.stream_out
    clstr.Residual.stream = clstr.stream_dma1.stream_out

    addr = local_low + 0x0000F5
    clstr.ResidualOut = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.Residual.stream = clstr.ResidualOut.stream_in

    # Add the First Pointwise Convolution
    acc = "PWConv0"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.PWConv0 = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.PWConv0, config, ir)
    clstr._connect_hwacc(clstr.PWConv0)
    clstr.PWConv0.stream = clstr.ResidualOut.stream_out

    addr = local_low + 0x0000F6
    spmRange = AddrRange(addr, addr+120)
    clstr.PWConv0LocalFeat = ScratchpadMemory(range=spmRange)
    clstr.PWConv0LocalFeat.conf_table_reported = False
    clstr.PWConv0LocalFeat.ready_mode=True
    clstr.PWConv0LocalFeat.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv0.spm = clstr.PWConv0LocalFeat.spm_ports

    addr = local_low + 0x00016E
    spmRange = AddrRange(addr, addr+(120*720))
    clstr.PWConv0Weights = ScratchpadMemory(range=spmRange)
    clstr.PWConv0Weights.conf_table_reported = False
    clstr.PWConv0Weights.ready_mode=True
    clstr.PWConv0Weights.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv0.spm = clstr.PWConv0Weights.spm_ports

    addr = local_low + 0x0152EE
    spmRange = AddrRange(addr, addr+(720*6))
    clstr.PWConv0QParams = ScratchpadMemory(range=spmRange)
    clstr.PWConv0QParams.conf_table_reported = False
    clstr.PWConv0QParams.ready_mode=True
    clstr.PWConv0QParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv0.spm = clstr.PWConv0QParams.spm_ports

    addr = local_low + 0x0163CE
    clstr.PWConv0Out = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.PWConv0.stream = clstr.PWConv0Out.stream_in

    # Add the Depthwise Convolution
    acc = "DWConv"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.DWConv = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.DWConv, config, ir)
    clstr._connect_hwacc(clstr.DWConv)
    clstr.DWConv.stream = clstr.PWConv0Out.stream_out

    addr = local_low + 0x0163CF
    spmRange = AddrRange(addr, addr+(80*2*720))
    clstr.DWConvBuffer = ScratchpadMemory(range=spmRange)
    clstr.DWConvBuffer.conf_table_reported = False
    clstr.DWConvBuffer.ready_mode=True
    clstr.DWConvBuffer.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvBuffer.spm_ports

    addr = local_low + 0x0325CF
    spmRange = AddrRange(addr, addr+(9*720))
    clstr.DWConvWindow = ScratchpadMemory(range=spmRange)
    clstr.DWConvWindow.conf_table_reported = False
    clstr.DWConvWindow.ready_mode=True
    clstr.DWConvWindow.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvWindow.spm_ports

    addr = local_low + 0x033F1F
    spmRange = AddrRange(addr, addr+720)
    clstr.DWConvOutBuffer = ScratchpadMemory(range=spmRange)
    clstr.DWConvOutBuffer.conf_table_reported = False
    clstr.DWConvOutBuffer.ready_mode=True
    clstr.DWConvOutBuffer.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvOutBuffer.spm_ports

    addr = local_low + 0x0341EF
    spmRange = AddrRange(addr, addr+(10*720))
    clstr.DWConvWeights = ScratchpadMemory(range=spmRange)
    clstr.DWConvWeights.conf_table_reported = False
    clstr.DWConvWeights.ready_mode=True
    clstr.DWConvWeights.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvWeights.spm_ports

    addr = local_low + 0x035E0F
    spmRange = AddrRange(addr, addr+(6*720))
    clstr.DWConvQParams = ScratchpadMemory(range=spmRange)
    clstr.DWConvQParams.conf_table_reported = False
    clstr.DWConvQParams.ready_mode=True
    clstr.DWConvQParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.DWConv.spm = clstr.DWConvQParams.spm_ports

    addr = local_low + 0x036EEF
    clstr.DWConvOut = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.DWConv.stream = clstr.DWConvOut.stream_in

    # Add the Second Pointwise Convolution
    acc = "PWConv1"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.PWConv1 = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.PWConv1, config, ir)
    clstr._connect_hwacc(clstr.PWConv1)
    clstr.PWConv1.stream = clstr.DWConvOut.stream_out

    addr = local_low + 0x036EF0
    spmRange = AddrRange(addr, addr+720)
    clstr.PWConv1LocalFeat = ScratchpadMemory(range=spmRange)
    clstr.PWConv1LocalFeat.conf_table_reported = False
    clstr.PWConv1LocalFeat.ready_mode=True
    clstr.PWConv1LocalFeat.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv1.spm = clstr.PWConv1LocalFeat.spm_ports

    addr = local_low + 0x0371C0
    spmRange = AddrRange(addr, addr+(240*720))
    clstr.PWConv1Weights = ScratchpadMemory(range=spmRange)
    clstr.PWConv1Weights.conf_table_reported = False
    clstr.PWConv1Weights.ready_mode=True
    clstr.PWConv1Weights.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv1.spm = clstr.PWConv1Weights.spm_ports

    addr = local_low + 0x0614C0
    spmRange = AddrRange(addr, addr+(6*240))
    clstr.PWConv1QParams = ScratchpadMemory(range=spmRange)
    clstr.PWConv1QParams.conf_table_reported = False
    clstr.PWConv1QParams.ready_mode=True
    clstr.PWConv1QParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv1.spm = clstr.PWConv1QParams.spm_ports

    clstr.PWConv1.stream = clstr.stream_dma0.stream_in

def buildTail(options, system, clstr):
    # Specify the path to the mobilenet accelerator descriptions
    hw_path = options.accpath + "/mobilenetv2/hw"
    hw_config_path = hw_path + "/configs/tail/"
    hw_ir_path = hw_path + "/ir/tail/"
    local_low = 0x2F065000
    local_high = 0x2F0B7C52
    local_range = AddrRange(local_low, local_high)
    external_range = [AddrRange(0x00000000, local_low-1),
                      AddrRange(local_high+1, 0xFFFFFFFF)]
    clstr._attach_bridges(system, local_range, external_range)
    clstr._connect_caches(system, options)
    gic = system.realview.gic

    # Add the top function
    acc = "top"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.top = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.top, config, ir)
    clstr._connect_hwacc(clstr.top)
    clstr.top.local = clstr.local_bus.slave

    # Add the Stream DMAs
    addr = local_low + 0x000021
    clstr.stream_dma0 = StreamDma(pio_addr=addr, pio_size=32, gic=gic, max_pending=32)
    clstr.stream_dma0.stream_addr=addr+32
    clstr.stream_dma0.stream_size=8
    clstr.stream_dma0.pio_delay='1ns'
    clstr.stream_dma0.rd_int = 216
    clstr.stream_dma0.wr_int = 217
    clstr._connect_dma(system, clstr.stream_dma0)

    # Add the cluster DMA
    addr = local_low + 0x000049
    clstr.dma = NoncoherentDma(pio_addr=addr, pio_size=21, gic=gic, int_num=97)
    clstr._connect_cluster_dma(system, clstr.dma)

    # Add the Pointwise Convolution
    acc = "PWConv"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.PWConv = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.PWConv, config, ir)
    clstr._connect_hwacc(clstr.PWConv)
    clstr.PWConv.stream = clstr.stream_dma0.stream_out

    addr = local_low + 0x000061
    spmRange = AddrRange(addr, addr+240)
    clstr.PWConvLocalFeat = ScratchpadMemory(range=spmRange)
    clstr.PWConvLocalFeat.conf_table_reported = False
    clstr.PWConvLocalFeat.ready_mode=True
    clstr.PWConvLocalFeat.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv.spm = clstr.PWConvLocalFeat.spm_ports

    addr = local_low + 0x000151
    spmRange = AddrRange(addr, addr+(240*1280))
    clstr.PWConvWeights = ScratchpadMemory(range=spmRange)
    clstr.PWConvWeights.conf_table_reported = False
    clstr.PWConvWeights.ready_mode=True
    clstr.PWConvWeights.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv.spm = clstr.PWConvWeights.spm_ports

    addr = local_low + 0x04B151
    spmRange = AddrRange(addr, addr+(6*1280))
    clstr.PWConvQParams = ScratchpadMemory(range=spmRange)
    clstr.PWConvQParams.conf_table_reported = False
    clstr.PWConvQParams.ready_mode=True
    clstr.PWConvQParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.PWConv.spm = clstr.PWConvQParams.spm_ports

    addr = local_low + 0x04CF51
    clstr.PWConvOut = StreamBuffer(stream_address=addr, stream_size=1, buffer_size=8)
    clstr.PWConv.stream = clstr.PWConvOut.stream_in

    # Add the Reshape
    acc = "Reshape"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.Reshape = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.Reshape, config, ir)
    clstr._connect_hwacc(clstr.Reshape)
    clstr.Reshape.stream = clstr.PWConvOut.stream_out

    addr = local_low + 0x04CF52
    spmRange = AddrRange(addr, addr+(25*1280))
    clstr.ReshapeOut = ScratchpadMemory(range=spmRange)
    clstr.ReshapeOut.conf_table_reported = False
    clstr.ReshapeOut.ready_mode=True
    clstr.ReshapeOut.port = clstr.local_bus.master
    for i in range(1):
        clstr.Reshape.spm = clstr.ReshapeOut.spm_ports

    # Add the Avg Pool
    acc = "AvgPool"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.AvgPool = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.AvgPool, config, ir)
    clstr._connect_hwacc(clstr.AvgPool)
    for i in range(1):
        clstr.AvgPool.spm = clstr.ReshapeOut.spm_ports

    clstr.AvgPool.stream = clstr.stream_dma0.stream_in

def buildClassifier(options, system, clstr):
    # Specify the path to the mobilenet accelerator descriptions
    hw_path = options.accpath + "/mobilenetv2/hw"
    hw_config_path = hw_path + "/configs/classifier/"
    hw_ir_path = hw_path + "/ir/classifier/"
    local_low = 0x2F0BF000
    local_high = 0x2F1F28B7
    local_range = AddrRange(local_low, local_high)
    external_range = [AddrRange(0x00000000, local_low-1),
                      AddrRange(local_high+1, 0xFFFFFFFF)]
    clstr._attach_bridges(system, local_range, external_range)
    clstr._connect_caches(system, options)
    gic = system.realview.gic

    # Add the top function
    acc = "top"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.top = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.top, config, ir)
    clstr._connect_hwacc(clstr.top)
    clstr.top.local = clstr.local_bus.slave

    # Add the Stream DMAs
    addr = local_low + 0x0021
    clstr.stream_dma0 = StreamDma(pio_addr=addr, pio_size=32, gic=gic, max_pending=32)
    clstr.stream_dma0.stream_addr=addr+32
    clstr.stream_dma0.stream_size=8
    clstr.stream_dma0.pio_delay='1ns'
    clstr.stream_dma0.rd_int = 218
    clstr.stream_dma0.wr_int = 219
    clstr._connect_dma(system, clstr.stream_dma0)

    # Add the cluster DMA
    addr = local_low + 0x00049
    clstr.dma = NoncoherentDma(pio_addr=addr, pio_size=21, gic=gic, int_num=98)
    clstr._connect_cluster_dma(system, clstr.dma)

    # Add the Fully Connected Layer
    acc = "Linear"
    config = hw_config_path + acc + ".ini"
    ir = hw_ir_path + acc + ".ll"
    clstr.Linear = CommInterface(devicename=acc, gic=gic)
    AccConfig(clstr.Linear, config, ir)
    clstr._connect_hwacc(clstr.Linear)

    addr = local_low + 0x00005F
    spmRange = AddrRange(addr, addr+(1280))
    clstr.LinearFeats = ScratchpadMemory(range=spmRange)
    clstr.LinearFeats.conf_table_reported = False
    clstr.LinearFeats.ready_mode=True
    clstr.LinearFeats.port = clstr.local_bus.master
    for i in range(1):
        clstr.Linear.spm = clstr.LinearFeats.spm_ports

    addr = local_low + 0x00055F
    spmRange = AddrRange(addr, addr+(1000*1280))
    clstr.LinearWeights = ScratchpadMemory(range=spmRange)
    clstr.LinearWeights.conf_table_reported = False
    clstr.LinearWeights.ready_mode=True
    clstr.LinearWeights.port = clstr.local_bus.master
    for i in range(1):
        clstr.Linear.spm = clstr.LinearWeights.spm_ports

    addr = local_low + 0x138D5F
    spmRange = AddrRange(addr, addr+(1000*6))
    clstr.LinearQParams = ScratchpadMemory(range=spmRange)
    clstr.LinearQParams.conf_table_reported = False
    clstr.LinearQParams.ready_mode=True
    clstr.LinearQParams.port = clstr.local_bus.master
    for i in range(1):
        clstr.Linear.spm = clstr.LinearQParams.spm_ports

    addr = local_low + 0x13A4CF
    spmRange = AddrRange(addr, addr+(1000))
    clstr.LinearSum = ScratchpadMemory(range=spmRange)
    clstr.LinearSum.conf_table_reported = False
    clstr.LinearSum.ready_mode=True
    clstr.LinearSum.port = clstr.local_bus.master
    for i in range(1):
        clstr.Linear.spm = clstr.LinearSum.spm_ports

    clstr.Linear.stream = clstr.stream_dma0.stream_in

def makeHWAcc(options, system):
    system.head = AccCluster()
    buildHead(options, system, system.head)

    system.body = AccCluster()
    buildBody(options, system, system.body)

    system.tail = AccCluster()
    buildTail(options, system, system.tail)

    system.classifier = AccCluster()
    buildClassifier(options, system, system.classifier)