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

        self.datatype_dict = self.data['functional_unit']['parameters']['datatypes']
        for types in self.datatype_dict.keys():
            print(types)
            self.size = self.datatype_dict[types]['size']
            self.sign = self.datatype_dict[types]['sign']
            self.apmode = self.datatype_dict[types]['APMode']

        self.instructions_list = self.data['functional_unit']['parameters']['instructions']
        print(self.instructions_list)

        self.limit = self.data['functional_unit']['parameters']['limit']
        
    def generate_power_model(self):
        self.units_dict = self.data['functional_unit']['power_model']['units']
        self.power_units = self.units_dict['power']
        self.energy_units = self.units_dict['energy']
        self.time_units = self.units_dict['time']
        self.area_units = self.units_dict['area']
        self.fu_latenct = self.data['functional_unit']['power_model']['latency']
        self.internal_power = self.data['functional_unit']['power_model']['internal_power']
        self.power_power = self.data['functional_unit']['power_model']['switch_power']
        self.internal_power = self.data['functional_unit']['power_model']['internal_power']
        self.switch_power = self.data['functional_unit']['power_model']['switch_power']
        self.dynamic_power = self.data['functional_unit']['power_model']['dynamic_power']
        self.dynamic_energy = self.data['functional_unit']['power_model']['dynamic_energy']
        self.leakage_power = self.data['functional_unit']['power_model']['leakage_power']
        self.area = self.data['functional_unit']['power_model']['area']
        self.path_delay = self.data['functional_unit']['power_model']['path_delay']


benchmark = HWArgs()
fu = HWModel(benchname=benchmark.bench)
for unit in fu.get_fu_list():
    fu.generate_hw(unit)
    fu.generate_power_model()

headerfile = FunctionalUnitGenerator(alias = "integer_adder")
headerfile.header_generator()