import m5
from m5.objects import *
from m5.util import *
import ConfigParser

def AccConfig(acc, config_file, bench_file):
    # Setup config file parser
    Config = ConfigParser.ConfigParser()
    Config.read((config_file))
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
    # Setup comm interface
    acc.pio_addr=ConfigSectionMap("CommInterface")['pio_addr']
    acc.pio_size=ConfigSectionMap("CommInterface")['pio_size']
    acc.flags_size = ConfigSectionMap("AccConfig")['flags_size']
    acc.config_size = ConfigSectionMap("AccConfig")['config_size']
    acc.int_num = ConfigSectionMap("AccConfig")['int_num']
    acc.clock_period = ConfigSectionMap("AccConfig")['clock_period']
    predef = ConfigSectionMap("AccConfig")['premap_data']

    if (predef == "1" or predef == "True"):
        acc.premap_data = predef
        acc.data_bases = ConfigSectionMap("AccConfig")['data_bases']


    # Initialize LLVMInterface Objects
    acc.llvm_interface = LLVMInterface()
    acc.llvm_interface.cycles = CycleCounts()

    # Benchmark path
    acc.llvm_interface.in_file = bench_file

    # Load instruction cycle counts
    acc.llvm_interface.cycles.counter = ConfigSectionMap("CycleCounts")['counter']
    acc.llvm_interface.cycles.gep = ConfigSectionMap("CycleCounts")['gep']
    acc.llvm_interface.cycles.phi = ConfigSectionMap("CycleCounts")['phi']
    acc.llvm_interface.cycles.select = ConfigSectionMap("CycleCounts")['select']
    acc.llvm_interface.cycles.ret = ConfigSectionMap("CycleCounts")['ret']
    acc.llvm_interface.cycles.br = ConfigSectionMap("CycleCounts")['br']
    acc.llvm_interface.cycles.switch_inst = ConfigSectionMap("CycleCounts")['switch']
    acc.llvm_interface.cycles.indirectbr = ConfigSectionMap("CycleCounts")['indirectbr']
    acc.llvm_interface.cycles.invoke = ConfigSectionMap("CycleCounts")['invoke']
    acc.llvm_interface.cycles.resume = ConfigSectionMap("CycleCounts")['resume']
    acc.llvm_interface.cycles.unreachable = ConfigSectionMap("CycleCounts")['unreachable']
    acc.llvm_interface.cycles.icmp = ConfigSectionMap("CycleCounts")['icmp']
    acc.llvm_interface.cycles.fcmp = ConfigSectionMap("CycleCounts")['fcmp']
    acc.llvm_interface.cycles.trunc = ConfigSectionMap("CycleCounts")['trunc']
    acc.llvm_interface.cycles.zext = ConfigSectionMap("CycleCounts")['zext']
    acc.llvm_interface.cycles.sext = ConfigSectionMap("CycleCounts")['sext']
    acc.llvm_interface.cycles.fptrunc = ConfigSectionMap("CycleCounts")['fptrunc']
    acc.llvm_interface.cycles.fpext = ConfigSectionMap("CycleCounts")['fpext']
    acc.llvm_interface.cycles.fptoui = ConfigSectionMap("CycleCounts")['fptoui']
    acc.llvm_interface.cycles.fptosi = ConfigSectionMap("CycleCounts")['fptosi']
    acc.llvm_interface.cycles.uitofp = ConfigSectionMap("CycleCounts")['uitofp']
    acc.llvm_interface.cycles.ptrtoint = ConfigSectionMap("CycleCounts")['ptrtoint']
    acc.llvm_interface.cycles.inttoptr = ConfigSectionMap("CycleCounts")['inttoptr']
    acc.llvm_interface.cycles.bitcast = ConfigSectionMap("CycleCounts")['bitcast']
    acc.llvm_interface.cycles.addrspacecast = ConfigSectionMap("CycleCounts")['addrspacecast']
    acc.llvm_interface.cycles.call = ConfigSectionMap("CycleCounts")['call']
    acc.llvm_interface.cycles.vaarg = ConfigSectionMap("CycleCounts")['vaarg']
    acc.llvm_interface.cycles.landingpad = ConfigSectionMap("CycleCounts")['landingpad']
    acc.llvm_interface.cycles.catchpad = ConfigSectionMap("CycleCounts")['catchpad']
    acc.llvm_interface.cycles.alloca = ConfigSectionMap("CycleCounts")['alloca']
    acc.llvm_interface.cycles.load = ConfigSectionMap("CycleCounts")['load']
    acc.llvm_interface.cycles.store = ConfigSectionMap("CycleCounts")['store']
    acc.llvm_interface.cycles.fence = ConfigSectionMap("CycleCounts")['fence']
    acc.llvm_interface.cycles.cmpxchg = ConfigSectionMap("CycleCounts")['cmpxchg']
    acc.llvm_interface.cycles.atomicrmw = ConfigSectionMap("CycleCounts")['atomicrmw']
    acc.llvm_interface.cycles.extractvalue = ConfigSectionMap("CycleCounts")['extractvalue']
    acc.llvm_interface.cycles.insertvalue = ConfigSectionMap("CycleCounts")['insertvalue']
    acc.llvm_interface.cycles.extractelement = ConfigSectionMap("CycleCounts")['extractelement']
    acc.llvm_interface.cycles.insertelement = ConfigSectionMap("CycleCounts")['insertelement']
    acc.llvm_interface.cycles.shufflevector = ConfigSectionMap("CycleCounts")['shufflevector']
    acc.llvm_interface.cycles.shl = ConfigSectionMap("CycleCounts")['shl']
    acc.llvm_interface.cycles.lshr = ConfigSectionMap("CycleCounts")['lshr']
    acc.llvm_interface.cycles.ashr = ConfigSectionMap("CycleCounts")['ashr']
    acc.llvm_interface.cycles.and_inst = ConfigSectionMap("CycleCounts")['andinst']
    acc.llvm_interface.cycles.or_inst = ConfigSectionMap("CycleCounts")['orinst']
    acc.llvm_interface.cycles.xor_inst = ConfigSectionMap("CycleCounts")['xor']
    acc.llvm_interface.cycles.add = ConfigSectionMap("CycleCounts")['add']
    acc.llvm_interface.cycles.sub = ConfigSectionMap("CycleCounts")['sub']
    acc.llvm_interface.cycles.mul = ConfigSectionMap("CycleCounts")['mul']
    acc.llvm_interface.cycles.udiv = ConfigSectionMap("CycleCounts")['udiv']
    acc.llvm_interface.cycles.sdiv = ConfigSectionMap("CycleCounts")['sdiv']
    acc.llvm_interface.cycles.urem = ConfigSectionMap("CycleCounts")['urem']
    acc.llvm_interface.cycles.srem = ConfigSectionMap("CycleCounts")['srem']
    acc.llvm_interface.cycles.fadd = ConfigSectionMap("CycleCounts")['fadd']
    acc.llvm_interface.cycles.fsub = ConfigSectionMap("CycleCounts")['fsub']
    acc.llvm_interface.cycles.fmul = ConfigSectionMap("CycleCounts")['fmul']
    acc.llvm_interface.cycles.fdiv = ConfigSectionMap("CycleCounts")['fdiv']
    acc.llvm_interface.cycles.frem = ConfigSectionMap("CycleCounts")['frem']

    # Set functional units
    acc.llvm_interface.FU_fp_sp_adder = ConfigSectionMap("FunctionalUnits")['fp_sp_add']
    acc.llvm_interface.FU_fp_dp_adder = ConfigSectionMap("FunctionalUnits")['fp_dp_add']
    acc.llvm_interface.FU_fp_sp_multiplier = ConfigSectionMap("FunctionalUnits")['fp_sp_mul']
    acc.llvm_interface.FU_fp_sp_divider = ConfigSectionMap("FunctionalUnits")['fp_sp_div']
    acc.llvm_interface.FU_fp_dp_multiplier = ConfigSectionMap("FunctionalUnits")['fp_dp_mul']
    acc.llvm_interface.FU_fp_dp_divider = ConfigSectionMap("FunctionalUnits")['fp_dp_div']
    acc.llvm_interface.FU_counter = ConfigSectionMap("FunctionalUnits")['fu_counter']
    acc.llvm_interface.FU_compare = ConfigSectionMap("FunctionalUnits")['fu_compare']
    acc.llvm_interface.FU_GEP = ConfigSectionMap("FunctionalUnits")['fu_gep']
    acc.llvm_interface.FU_int_multiplier = ConfigSectionMap("FunctionalUnits")['fu_int_mul']
    acc.llvm_interface.FU_int_adder = ConfigSectionMap("FunctionalUnits")['fu_int_add']
    acc.llvm_interface.FU_int_shifter = ConfigSectionMap("FunctionalUnits")['fu_int_shift']
    acc.llvm_interface.FU_int_bit = ConfigSectionMap("FunctionalUnits")['fu_int_bit']
    acc.llvm_interface.FU_conversion = ConfigSectionMap("FunctionalUnits")['fu_conversion']

    # Set scheduling constraints
    acc.llvm_interface.FU_pipelined = ConfigSectionMap("Scheduler")['fu_pipelined']
    acc.llvm_interface.sched_threshold = ConfigSectionMap("Scheduler")['sched_threshold']
    acc.llvm_interface.FU_clock_period = ConfigSectionMap("Scheduler")['fu_clock_period']
    acc.llvm_interface.clock_period = ConfigSectionMap("AccConfig")['clock_period']
    acc.llvm_interface.lockstep_mode = Config.getboolean("Scheduler", 'lockstep_mode')

def AccSPMConfig(acc, spm, config_file):
    # Setup config file parser
    Config = ConfigParser.ConfigParser()
    Config.read((config_file))
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

    spm.range = AddrRange(ConfigSectionMap("Memory")['addr_range'], \
                          size=ConfigSectionMap("Memory")['size'])
    spm.latency = ConfigSectionMap("Memory")['latency']
    spm.conf_table_reported = False
    spm.ready_mode = Config.getboolean("Memory", 'ready_mode')
    spm.reset_on_scratchpad_read = Config.getboolean("Memory", 'reset_on_private_read')
    num_ports = ConfigSectionMap("Memory")['ports']
    for i in range(int(num_ports)):
        acc.spm[i] = spm.spm_ports[i]
