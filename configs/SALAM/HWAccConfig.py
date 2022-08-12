import m5
from m5.objects import *
from m5.util import *
from configparser import ConfigParser
from pathlib import Path
import yaml
import os

def AccConfig(acc, bench_file, config_file):
    # Initialize LLVMInterface Objects
    acc.llvm_interface = LLVMInterface()

    # Benchmark path
    acc.llvm_interface.in_file = bench_file
    M5_Path = os.getenv('M5_PATH')
    benchname = os.path.splitext(os.path.basename(bench_file))[0]


    # lenet config launcher custom stuff
    benchPath = Path(bench_file).parts
    m5PathLen = len(Path(M5_Path).parts)

    # Set scheduling constraints
    #acc.llvm_interface.sched_threshold = ConfigSectionMap("Scheduler")['sched_threshold']
    #acc.llvm_interface.clock_period = ConfigSectionMap("AccConfig")['clock_period']
    #acc.llvm_interface.lockstep_mode = Config.getboolean("Scheduler", 'lockstep_mode')

    #TODO: Auto generate the functional unit list

	# Initialize HWInterface Objects
    acc.hw_interface = HWInterface()
    # Define HW Counts
    acc.hw_interface.cycle_counts = CycleCounts()
    #acc.hw_interface.cycle_counts
    
    if benchPath[m5PathLen+1] == 'mobilenetv2':
        fu_yaml = open(config_file, 'r')
        for yaml_inst_list in yaml.safe_load_all(fu_yaml):
            document = yaml_inst_list['acc_cluster']
            current_acc = document[0]['Name'] + '_' + benchname 
            if(benchPath[9] == document[0]['Name']):
                print(current_acc + " Profile Loaded")
                # print(yaml_inst_list['hw_config'][benchname])
                inst_list = yaml_inst_list['hw_config'][current_acc]['instructions'].keys()
                for instruction in inst_list:
                    setattr(acc.hw_interface.cycle_counts, instruction, yaml_inst_list['hw_config'][current_acc]['instructions'][instruction]['runtime_cycles'])
        fu_yaml.close()
    
    else:
        fu_yaml = open(config_file, 'r')
        yaml_inst_list = yaml.safe_load(fu_yaml)
        if yaml_inst_list['hw_config'][benchname] is not None:
            inst_list = yaml_inst_list['hw_config'][benchname]['instructions'].keys()
            for instruction in inst_list:
                setattr(acc.hw_interface.cycle_counts, instruction, yaml_inst_list['hw_config'][benchname]['instructions'][instruction]['runtime_cycles'])
        fu_yaml.close()

    #TODO Automate the generation of the list below
    # Functional Units
    acc.hw_interface.functional_units = FunctionalUnits()
    acc.hw_interface.functional_units.double_multiplier = DoubleMultiplier() 
    acc.hw_interface.functional_units.bit_register = BitRegister()
    acc.hw_interface.functional_units.bitwise_operations = BitwiseOperations()
    acc.hw_interface.functional_units.double_adder = DoubleAdder()
    acc.hw_interface.functional_units.float_divider = FloatDivider()
    acc.hw_interface.functional_units.bit_shifter = BitShifter()
    acc.hw_interface.functional_units.integer_multiplier = IntegerMultiplier()
    acc.hw_interface.functional_units.integer_adder = IntegerAdder()
    acc.hw_interface.functional_units.double_divider = DoubleDivider()
    acc.hw_interface.functional_units.float_adder = FloatAdder()
    acc.hw_interface.functional_units.float_multiplier = FloatMultiplier()

    #TODO Automate the generation of the list below
    # Instructions
    acc.hw_interface.inst_config = InstConfig()
    acc.hw_interface.inst_config.add = Add()
    acc.hw_interface.inst_config.addrspacecast = Addrspacecast()
    acc.hw_interface.inst_config.alloca = Alloca()
    acc.hw_interface.inst_config.and_inst = AndInst()
    acc.hw_interface.inst_config.ashr = Ashr()
    acc.hw_interface.inst_config.bitcast = Bitcast()
    acc.hw_interface.inst_config.br = Br()
    acc.hw_interface.inst_config.call = Call()
    acc.hw_interface.inst_config.fadd = Fadd()
    acc.hw_interface.inst_config.fcmp = Fcmp()
    acc.hw_interface.inst_config.fdiv = Fdiv()
    acc.hw_interface.inst_config.fence = Fence()
    acc.hw_interface.inst_config.fmul = Fmul()
    acc.hw_interface.inst_config.fpext = Fpext()
    acc.hw_interface.inst_config.fptosi = Fptosi()
    acc.hw_interface.inst_config.fptoui = Fptoui()
    acc.hw_interface.inst_config.fptrunc = Fptrunc()
    acc.hw_interface.inst_config.frem = Frem()
    acc.hw_interface.inst_config.fsub = Fsub()
    acc.hw_interface.inst_config.gep = Gep()
    acc.hw_interface.inst_config.icmp = Icmp()
    acc.hw_interface.inst_config.indirectbr = Indirectbr()
    acc.hw_interface.inst_config.inttoptr = Inttoptr()
    acc.hw_interface.inst_config.invoke = Invoke()
    acc.hw_interface.inst_config.landingpad = Landingpad()
    acc.hw_interface.inst_config.load = Load()
    acc.hw_interface.inst_config.lshr = Lshr()
    acc.hw_interface.inst_config.mul = Mul()
    acc.hw_interface.inst_config.or_inst = OrInst()
    acc.hw_interface.inst_config.phi = Phi()
    acc.hw_interface.inst_config.ptrtoint = Ptrtoint()
    acc.hw_interface.inst_config.resume = Resume()
    acc.hw_interface.inst_config.ret = Ret()
    acc.hw_interface.inst_config.sdiv = Sdiv()
    acc.hw_interface.inst_config.select = Select()
    acc.hw_interface.inst_config.sext = Sext()
    acc.hw_interface.inst_config.shl = Shl()
    acc.hw_interface.inst_config.srem = Srem()
    acc.hw_interface.inst_config.store = Store()
    acc.hw_interface.inst_config.sub = Sub()
    acc.hw_interface.inst_config.switch_inst = SwitchInst()
    acc.hw_interface.inst_config.trunc = Trunc()
    acc.hw_interface.inst_config.udiv = Udiv()
    acc.hw_interface.inst_config.uitofp = Uitofp()
    acc.hw_interface.inst_config.unreachable = Unreachable()
    acc.hw_interface.inst_config.urem = Urem()
    acc.hw_interface.inst_config.vaarg = Vaarg()
    acc.hw_interface.inst_config.xor_inst = XorInst()
    acc.hw_interface.inst_config.zext = Zext()


    acc.hw_interface.salam_power_model = SALAMPowerModel()
    acc.hw_interface.hw_statistics = HWStatistics()
    acc.hw_interface.simulator_config = SimulatorConfig()
    acc.hw_interface.opcodes = InstOpCodes()

#def AccSPMConfig(acc, spm, config_file):
    # Setup config file parser
    #Config = ConfigParser.ConfigParser()
    #Config.read((config_file))
    #Config.sections()
    #def ConfigSectionMap(section):
    #    dict1 = {}
    #    options = Config.options(section)
    #    for option in options:
    #        try:
    #            dict1[option] = Config.get(section, option)
    #            if dict1[option] == -1:
    #                DebugPrint("skip: %s" % option)
    #        except:
    #            print("exception on %s!" % option)
    #            dict1[option] = None
    #    return dict1

    #spm.range = AddrRange(ConfigSectionMap("Memory")['addr_range'], \
    #                      size=ConfigSectionMap("Memory")['size'])
    #spm.latency = ConfigSectionMap("Memory")['latency']
    #spm.conf_table_reported = False
    #spm.ready_mode = Config.getboolean("Memory", 'ready_mode')
    #spm.reset_on_scratchpad_read = Config.getboolean("Memory", 'reset_on_private_read')
    #num_ports = ConfigSectionMap("Memory")['ports']
    #for i in range(int(num_ports)):
    #    acc.spm[i] = spm.spm_ports[i]
