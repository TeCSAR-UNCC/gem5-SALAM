import yaml
import os

from SALAMArgs import HWArgs
from SALAMClassGenerator import FunctionalUnitGenerator


class HWModel():
    def __init__(self, model='40nm_model', latency='10ns', profile='default_profile', benchname=None, benchfolder = 'benchmarks/sys_validation'):
        self.benchname = benchname
        self.benchfolder = benchfolder
        self.model = model
        self.latency = latency
        self.profile = profile
        self.yaml_dir = benchfolder + '/' + benchname + '/configs/hw_interface/functional_units/' + model + '/' + latency + '/' + profile 
        self.inst_list_yaml = benchfolder + '/' + benchname + '/configs/hw_interface/instructions/inst_list.yml'
        self.fu_list = os.listdir(self.yaml_dir)

    def get_fu_list(self):
        return self.fu_list

    def generate_hw(self, fu):
        self.fu_yaml_path = self.yaml_dir + '/' + fu + '/' + fu + '.yml'
        self.fu_yaml = file(self.fu_yaml_path, 'r')
        self.data = yaml.load(self.fu_yaml, Loader=yaml.FullLoader)
        self.alias = self.data['functional_unit']['parameters']['alias']
        self.stages = self.data['functional_unit']['parameters']['stages']
        self.cycles = self.data['functional_unit']['parameters']['cycles']
        self.enum_value = self.data['functional_unit']['parameters']['enum_value']
        self.int_size = self.data['functional_unit']['parameters']['datatypes']['integer']['size']
        self.int_sign = self.data['functional_unit']['parameters']['datatypes']['integer']['sign']
        self.int_apmode = self.data['functional_unit']['parameters']['datatypes']['integer']['APMode']
        self.fp_size = self.data['functional_unit']['parameters']['datatypes']['floating_point']['size']
        self.fp_sign = self.data['functional_unit']['parameters']['datatypes']['floating_point']['sign']
        self.fp_apmode = self.data['functional_unit']['parameters']['datatypes']['floating_point']['APMode']
        self.ptr_size = self.data['functional_unit']['parameters']['datatypes']['pointer']['size'] 
        self.ptr_sign = self.data['functional_unit']['parameters']['datatypes']['pointer']['sign']
        self.ptr_apmode= self.data['functional_unit']['parameters']['datatypes']['pointer']['APMode']
        self.instructions_list = self.data['functional_unit']['parameters']['instructions']
        with open(self.inst_list_yaml) as inst_list:
            buffer = yaml.safe_load(inst_list)
            for instruction in self.instructions_list:
                if (instruction != 'any') and (instruction != 'none'):
                    buffer['instructions'][instruction]['functional_unit'] = self.enum_value
        with open(self.inst_list_yaml, 'w') as inst_list:
            yaml.safe_dump(buffer, inst_list, default_flow_style=False)
        self.limit = self.data['functional_unit']['parameters']['limit']
        
    def generate_power_model(self, fu):
        self.fu_yaml_path = self.yaml_dir + '/' + fu + '/' + fu + '.yml'
        self.fu_yaml = file(self.fu_yaml_path, 'r')
        self.data = yaml.load(self.fu_yaml, Loader=yaml.FullLoader)
        self.units_dict = self.data['functional_unit']['power_model']['units']
        self.power_units = self.units_dict['power']
        self.energy_units = self.units_dict['energy']
        self.time_units = self.units_dict['time']
        self.area_units = self.units_dict['area']
        self.fu_latency = self.data['functional_unit']['power_model']['latency']
        self.internal_power = self.data['functional_unit']['power_model']['internal_power']
        self.switch_power = self.data['functional_unit']['power_model']['switch_power']
        self.dynamic_power = self.data['functional_unit']['power_model']['dynamic_power']
        self.dynamic_energy = self.data['functional_unit']['power_model']['dynamic_energy']
        self.leakage_power = self.data['functional_unit']['power_model']['leakage_power']
        self.area = self.data['functional_unit']['power_model']['area']
        self.path_delay = self.data['functional_unit']['power_model']['path_delay']

benchmark_args = HWArgs()
generate_hw_models = HWModel(benchname=benchmark_args.bench, latency='5ns')
fu_file_generator = FunctionalUnitGenerator(fu_directory="src/hwacc/FunctionalUnits.py")
fu_file_generator.initialize_base_header_file()
fu_file_generator.initalize_fu_list_header(generate_hw_models.get_fu_list())
fu_file_generator.initialize_simobject_file(generate_hw_models.get_fu_list())
fu_file_generator.generate_sconscript(generate_hw_models.get_fu_list())

for functional_unit in generate_hw_models.get_fu_list():
    generate_hw_models.generate_hw(functional_unit)
    generate_hw_models.generate_power_model(functional_unit)
    fu_file_generator.set_fu(functional_unit)
    fu_file_generator.header_generator(generate_hw_models)
    fu_file_generator.simobject_generator(generate_hw_models)