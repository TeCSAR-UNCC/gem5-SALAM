# Script varies parameters to create design domain
import sys
import os
from argparse import ArgumentParser
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser

parser = ArgumentParser()
parser.add_argument("-f", "--config", dest="configFile", help="Config file location")
parser.add_argument("-m", "--memports", dest="memPorts", help="Read/Write ports")
parser.add_argument("-t", "--timing", dest="nsTime", help="Transistor timing in ns")
parser.add_argument("-c", "--counter", dest="counters", help="Set number of counter units")
parser.add_argument("-ia", "--intadd", dest="intAdders", help="Set number of int adder units")
parser.add_argument("-im", "--intmul", dest="intMuls", help="Set number of int mul units")
parser.add_argument("-s", "--shifter", dest="shifters", help="Set number of shifter units")
parser.add_argument("-b", "--bitwise", dest="bitWise", help="Set number of bitwise units")
parser.add_argument("-fa", "--floatadd", dest="floatAdd", help="Set number of float adder units")
parser.add_argument("-da", "--doubadd", dest="doubAdd", help="Set number of double adder units")
parser.add_argument("-fm", "--floatmul", dest="floatMul", help="Set number of float mul units")
parser.add_argument("-dm", "--doubmul", dest="doubMul", help="Set number of double mul units")
parser.add_argument("-z", "--zero", dest="zeroCyl", help="Set number of zero cycle units")
parser.add_argument("-g", "--gep", dest="gepSet", help="Set number of GEPs per cycle")
parser.add_argument("-cv", "--conv", dest="conversion", help="Set number of conversion units")
args = parser.parse_args()



config = ConfigParser()
config.read(args.configFile)
config.set('Memory','read_ports',args.memPorts)
config.set('Memory','write_ports',args.memPorts)
config.set('Scheduler','fu_clock_period',args.nsTime)
config.set('FunctionalUnits','fp_sp_add',args.floatAdd)
config.set('FunctionalUnits','fp_dp_add',args.doubAdd)
config.set('FunctionalUnits','fp_sp_mul',args.floatMul)
config.set('FunctionalUnits','fp_dp_mul',args.doubMul)
config.set('FunctionalUnits','fu_int_add',args.intAdders)
config.set('FunctionalUnits','fu_int_mul',args.intMuls)
config.set('FunctionalUnits','fu_int_bit',args.bitWise)
config.set('FunctionalUnits','fu_int_shift',args.shifters)
config.set('FunctionalUnits','fu_counter',args.counters)
config.set('FunctionalUnits','fu_gep',args.gepSet)
config.set('FunctionalUnits','fu_compare',args.zeroCyl)
config.set('FunctionalUnits','fu_conversion',args.conversion)


with open (args.configFile,"w") as configfile:
	config.write(configfile)
