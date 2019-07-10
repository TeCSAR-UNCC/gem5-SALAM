# Script varies parameters to create design domain
import sys
import os
from argparse import ArgumentParser
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser

parser = ArgumentParser()
parser.add_argument("-m", "--memports", dest="memPorts", help="Read/Write ports")
parser.add_argument("-t", "--timing", dest="nsTime", help="Transistor timing in ns")
parser.add_argument("-f", "--config", dest="configFile", help="Config file location")
args = parser.parse_args()
memPorts = args.memPorts
nsTime = args.nsTime
configFile = args.configFile

config = ConfigParser()
config.read(configFile)
config.set('Memory','read_ports',memPorts)
config.set('Memory','write_ports',memPorts)
config.set('Scheduler','fu_clock_period',nsTime)

with open (configFile,"w") as configfile:
	config.write(configfile)
