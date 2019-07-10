# Script stores simulation results 
import sys
import os
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("-f", "--file", dest="myFile", help="Open specified file")
parser.add_argument("-d", "--design", dest="myDesign", help="Saves to specified file")
parser.add_argument("-m", "--memports", dest="memPorts", help="Read/Write ports")
parser.add_argument("-t", "--timing", dest="nsTime", help="Transistor timing in ns")
args = parser.parse_args()
myFile = args.myFile
myDesign = args.myDesign
memPorts = args.memPorts
nsTime = args.nsTime

flag = False
param = 0
designFile = open(myDesign, "a")
with open (myFile,"rt") as parameters:
	for line in parameters:
		line = line.rstrip()
		if not flag:
			if line == r"StatsStart:":
				print ('Adjusting Parameters')	
				flag = True
		else:
		
			designFile.write(line)
designFile.write(","+memPorts+","+nsTime)
designFile.write("\n")
designFile.close()


