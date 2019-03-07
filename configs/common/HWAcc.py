import m5
from m5.objects import *
from m5.util import *
import ConfigParser



def makeHWAcc(options, system):

    # Setup config file parser
    Config = ConfigParser.ConfigParser()
    Config.read((options.accpath + "/" + options.accbench + "/host/" + "config.ini"))
    Config.sections()
    def ConfigSectionMap(section):
        dict1 = {}
        options = Config.options(section)
        for option in options:
            try:
                dict1[option] = Config.get(section, option)
                if dict1[option] == -1:
                    DebugPrint("skip: %s" % option)
            except:
                print("exception on %s!" % option)
                dict1[option] = None
        return dict1
        
    # Load config file data into application 
    # Construct our Accelerator Cluster and add a SPM of size 128kB
    system.acc_cluster = AccCluster()
    local_range = AddrRange(ConfigSectionMap("AccConfig")['local_range_min'], \
                            ConfigSectionMap("AccConfig")['local_range_max'])
    external_range = [AddrRange(ConfigSectionMap("AccConfig")['external_range_low_min'],  \
                                ConfigSectionMap("AccConfig")['external_range_low_max']), \
                      AddrRange(ConfigSectionMap("AccConfig")['external_range_hi_min'],   \
                                ConfigSectionMap("AccConfig")['external_range_hi_max'])]
    system.acc_cluster._attach_bridges(system, local_range, external_range)
    #system.acc_cluster._add_spm(AddrRange(0x2f100000, size='128kB'), '2ns')
    system.acc_cluster._connect_caches(system, options, ConfigSectionMap("AccConfig")['cache_size'])

    # Setup comm interface
    system.acc_cluster.acc = CommMemInterface(pio_addr=ConfigSectionMap("CommInterface")['pio_addr'], pio_size=ConfigSectionMap("CommInterface")['pio_size'], gic=system.realview.gic)
    
    # Accelerator setup
    system.acc_cluster.acc.flags_size = ConfigSectionMap("AccConfig")['flags_size']
    system.acc_cluster.acc.config_size = ConfigSectionMap("AccConfig")['config_size']
    system.acc_cluster.acc.local_range = local_range
    system.acc_cluster.acc.int_num = ConfigSectionMap("AccConfig")['int_num']
    system.acc_cluster.acc.clock_period = ConfigSectionMap("AccConfig")['clock_period']
    
    # Initialize LLVMInterface Objects
    system.acc_cluster.acc.llvm_interface = LLVMInterface()
    system.acc_cluster.acc.llvm_interface.cycles = CycleCounts()
    
    # Load instruction cycle counts
    system.acc_cluster.acc.llvm_interface.cycles.counter = ConfigSectionMap("CycleCounts")['counter']
    system.acc_cluster.acc.llvm_interface.cycles.gep = ConfigSectionMap("CycleCounts")['gep']
    system.acc_cluster.acc.llvm_interface.cycles.phi = ConfigSectionMap("CycleCounts")['phi']
    system.acc_cluster.acc.llvm_interface.cycles.select = ConfigSectionMap("CycleCounts")['select']
    system.acc_cluster.acc.llvm_interface.cycles.ret = ConfigSectionMap("CycleCounts")['ret']
    system.acc_cluster.acc.llvm_interface.cycles.br = ConfigSectionMap("CycleCounts")['br']
    system.acc_cluster.acc.llvm_interface.cycles.switch_inst = ConfigSectionMap("CycleCounts")['switch']
    system.acc_cluster.acc.llvm_interface.cycles.indirectbr = ConfigSectionMap("CycleCounts")['indirectbr']
    system.acc_cluster.acc.llvm_interface.cycles.invoke = ConfigSectionMap("CycleCounts")['invoke']
    system.acc_cluster.acc.llvm_interface.cycles.resume = ConfigSectionMap("CycleCounts")['resume']
    system.acc_cluster.acc.llvm_interface.cycles.unreachable = ConfigSectionMap("CycleCounts")['unreachable']
    system.acc_cluster.acc.llvm_interface.cycles.icmp = ConfigSectionMap("CycleCounts")['icmp']
    system.acc_cluster.acc.llvm_interface.cycles.fcmp = ConfigSectionMap("CycleCounts")['fcmp']
    system.acc_cluster.acc.llvm_interface.cycles.trunc = ConfigSectionMap("CycleCounts")['trunc']
    system.acc_cluster.acc.llvm_interface.cycles.zext = ConfigSectionMap("CycleCounts")['zext']
    system.acc_cluster.acc.llvm_interface.cycles.sext = ConfigSectionMap("CycleCounts")['sext']
    system.acc_cluster.acc.llvm_interface.cycles.fptrunc = ConfigSectionMap("CycleCounts")['fptrunc']
    system.acc_cluster.acc.llvm_interface.cycles.fpext = ConfigSectionMap("CycleCounts")['fpext']
    system.acc_cluster.acc.llvm_interface.cycles.fptoui = ConfigSectionMap("CycleCounts")['fptoui']
    system.acc_cluster.acc.llvm_interface.cycles.fptosi = ConfigSectionMap("CycleCounts")['fptosi']
    system.acc_cluster.acc.llvm_interface.cycles.uitofp = ConfigSectionMap("CycleCounts")['uitofp']
    system.acc_cluster.acc.llvm_interface.cycles.ptrtoint = ConfigSectionMap("CycleCounts")['ptrtoint']
    system.acc_cluster.acc.llvm_interface.cycles.inttoptr = ConfigSectionMap("CycleCounts")['inttoptr']
    system.acc_cluster.acc.llvm_interface.cycles.bitcast = ConfigSectionMap("CycleCounts")['bitcast']
    system.acc_cluster.acc.llvm_interface.cycles.addrspacecast = ConfigSectionMap("CycleCounts")['addrspacecast']
    system.acc_cluster.acc.llvm_interface.cycles.call = ConfigSectionMap("CycleCounts")['call']
    system.acc_cluster.acc.llvm_interface.cycles.vaarg = ConfigSectionMap("CycleCounts")['vaarg']
    system.acc_cluster.acc.llvm_interface.cycles.landingpad = ConfigSectionMap("CycleCounts")['landingpad']
    system.acc_cluster.acc.llvm_interface.cycles.catchpad = ConfigSectionMap("CycleCounts")['catchpad']
    system.acc_cluster.acc.llvm_interface.cycles.alloca = ConfigSectionMap("CycleCounts")['alloca']
    system.acc_cluster.acc.llvm_interface.cycles.load = ConfigSectionMap("CycleCounts")['load']
    system.acc_cluster.acc.llvm_interface.cycles.store = ConfigSectionMap("CycleCounts")['store']
    system.acc_cluster.acc.llvm_interface.cycles.fence = ConfigSectionMap("CycleCounts")['fence']
    system.acc_cluster.acc.llvm_interface.cycles.cmpxchg = ConfigSectionMap("CycleCounts")['cmpxchg']
    system.acc_cluster.acc.llvm_interface.cycles.atomicrmw = ConfigSectionMap("CycleCounts")['atomicrmw']
    system.acc_cluster.acc.llvm_interface.cycles.extractvalue = ConfigSectionMap("CycleCounts")['extractvalue']
    system.acc_cluster.acc.llvm_interface.cycles.insertvalue = ConfigSectionMap("CycleCounts")['insertvalue']
    system.acc_cluster.acc.llvm_interface.cycles.extractelement = ConfigSectionMap("CycleCounts")['extractelement']
    system.acc_cluster.acc.llvm_interface.cycles.insertelement = ConfigSectionMap("CycleCounts")['insertelement']
    system.acc_cluster.acc.llvm_interface.cycles.shufflevector = ConfigSectionMap("CycleCounts")['shufflevector']
    system.acc_cluster.acc.llvm_interface.cycles.shl = ConfigSectionMap("CycleCounts")['shl']
    system.acc_cluster.acc.llvm_interface.cycles.lshr = ConfigSectionMap("CycleCounts")['lshr']
    system.acc_cluster.acc.llvm_interface.cycles.ashr = ConfigSectionMap("CycleCounts")['ashr']
    system.acc_cluster.acc.llvm_interface.cycles.and_inst = ConfigSectionMap("CycleCounts")['andinst']
    system.acc_cluster.acc.llvm_interface.cycles.or_inst = ConfigSectionMap("CycleCounts")['orinst']
    system.acc_cluster.acc.llvm_interface.cycles.xor_inst = ConfigSectionMap("CycleCounts")['xor']
    system.acc_cluster.acc.llvm_interface.cycles.add = ConfigSectionMap("CycleCounts")['add']
    system.acc_cluster.acc.llvm_interface.cycles.sub = ConfigSectionMap("CycleCounts")['sub']
    system.acc_cluster.acc.llvm_interface.cycles.mul = ConfigSectionMap("CycleCounts")['mul']
    system.acc_cluster.acc.llvm_interface.cycles.udiv = ConfigSectionMap("CycleCounts")['udiv']
    system.acc_cluster.acc.llvm_interface.cycles.sdiv = ConfigSectionMap("CycleCounts")['sdiv']
    system.acc_cluster.acc.llvm_interface.cycles.urem = ConfigSectionMap("CycleCounts")['urem']
    system.acc_cluster.acc.llvm_interface.cycles.srem = ConfigSectionMap("CycleCounts")['srem']
    system.acc_cluster.acc.llvm_interface.cycles.fadd = ConfigSectionMap("CycleCounts")['fadd']
    system.acc_cluster.acc.llvm_interface.cycles.fsub = ConfigSectionMap("CycleCounts")['fsub']
    system.acc_cluster.acc.llvm_interface.cycles.fmul = ConfigSectionMap("CycleCounts")['fmul']
    system.acc_cluster.acc.llvm_interface.cycles.fdiv = ConfigSectionMap("CycleCounts")['fdiv']
    system.acc_cluster.acc.llvm_interface.cycles.frem = ConfigSectionMap("CycleCounts")['frem'] 	 
    
    # Set functional units
    system.acc_cluster.acc.llvm_interface.FU_fp_sp_adder = ConfigSectionMap("FunctionalUnits")['fp_sp_add']
    system.acc_cluster.acc.llvm_interface.FU_fp_dp_adder = ConfigSectionMap("FunctionalUnits")['fp_dp_add']
    system.acc_cluster.acc.llvm_interface.FU_fp_sp_multiplier = ConfigSectionMap("FunctionalUnits")['fp_sp_mul']
    system.acc_cluster.acc.llvm_interface.FU_fp_dp_multiplier = ConfigSectionMap("FunctionalUnits")['fp_dp_mul']
    system.acc_cluster.acc.llvm_interface.FU_counter = ConfigSectionMap("FunctionalUnits")['fu_counter']
    system.acc_cluster.acc.llvm_interface.FU_compare = ConfigSectionMap("FunctionalUnits")['fu_compare']
    system.acc_cluster.acc.llvm_interface.FU_GEP = ConfigSectionMap("FunctionalUnits")['fu_gep']
    system.acc_cluster.acc.llvm_interface.FU_int_multiplier = ConfigSectionMap("FunctionalUnits")['fu_int_mul']
    system.acc_cluster.acc.llvm_interface.FU_int_adder = ConfigSectionMap("FunctionalUnits")['fu_int_add']
    system.acc_cluster.acc.llvm_interface.FU_int_shifter = ConfigSectionMap("FunctionalUnits")['fu_int_shift']
    system.acc_cluster.acc.llvm_interface.FU_int_bit = ConfigSectionMap("FunctionalUnits")['fu_int_bit']
    system.acc_cluster.acc.llvm_interface.FU_conversion = ConfigSectionMap("FunctionalUnits")['fu_conversion']
    
    # Set scheduling constraints
    system.acc_cluster.acc.llvm_interface.FU_pipelined = ConfigSectionMap("Scheduler")['fu_pipelined']
    system.acc_cluster.acc.llvm_interface.sched_threshold = ConfigSectionMap("Scheduler")['sched_threshold']
    system.acc_cluster.acc.llvm_interface.FU_clock_period = ConfigSectionMap("Scheduler")['fu_clock_period']
    system.acc_cluster.acc.llvm_interface.lockstep_mode = Config.getboolean("Scheduler", 'lockstep_mode')
    
    # Private memory
    system.acc_cluster.acc.private_range = AddrRange(ConfigSectionMap("PrivateMemory")['addr_range'], \
	                                                  size=ConfigSectionMap("PrivateMemory")['size'])
    system.acc_cluster.acc.private_memory = PrivateMemory(range=system.acc_cluster.acc.private_range, \
	                                                       conf_table_reported=False, \
	                                                       latency=ConfigSectionMap("PrivateMemory")['latency'])
    
    # Memory constraints
    system.acc_cluster.acc.private_read_ports = ConfigSectionMap("Memory")['read_ports']
    system.acc_cluster.acc.private_write_ports = ConfigSectionMap("Memory")['write_ports']
    system.acc_cluster.acc.private_memory.ready_mode = Config.getboolean("Memory", 'ready_mode')
    system.acc_cluster.acc.private_memory.reset_on_private_read = Config.getboolean("Memory", 'reset_on_private_read')
    
    # Connect HWacc and SPM
    system.acc_cluster._connect_hwacc(system.acc_cluster.acc)
    system.acc_cluster._connect_spm(system.acc_cluster.acc.private_memory)
    # Benchmark path
    system.acc_cluster.acc.llvm_interface.in_file = options.accpath + "/" + options.accbench + "/bench/" + options.accbench + ".ll"

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

