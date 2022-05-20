#!/usr/bin/env python3

# Import needed packages
import yaml
import os
import textwrap
import shutil
import argparse
from parser import *

# This requires M5_PATH to point to your gem5-SALAM directory
M5_Path = os.getenv('M5_PATH')

# Define the imports of the gem5 script
imports = """import m5\nfrom m5.objects import *\nfrom m5.util import *\nfrom configparser import ConfigParser\nfrom HWAccConfig import *\n\n"""
# L1 Cache defined here for now, need to add some more configurability to this
l1Cache = """class L1Cache(Cache):
\tassoc = 2
\ttag_latency = 2
\tdata_latency = 2
\tresponse_latency = 2
\tmshrs = 4
\ttgts_per_mshr = 20\n\t
def __init__(self, size, options=None):
\t\tself.size = size
\t\tsuper(L1Cache, self).__init__()
\t\tpass\n\n"""

# Parse Arguements
parser = argparse.ArgumentParser(description="SALAM System Builder")
parser.add_argument('--sysName', help="Desired gem5 script name", required=True)
# parser.add_argument('--headerName', help="Header Name", required=True)
parser.add_argument('--benchDir', help="Path to Benchmark Directory" +
"Relative to M5_PATH", required=True)
parser.add_argument('--path', help="Path to M5 Directory", required=False)
args=parser.parse_args()

if M5_Path is None:
    print("Looking for Path Argument from Command Line")
    M5_Path = args.path
    if M5_Path is None:
        print("M5_PATH Not Found")
        exit(1)

args=parser.parse_args()

# Set file information
fileName = args.sysName
CONFIG_Path = M5_Path + "/configs/SALAM/generated/"
workingDirectory = M5_Path + "/"  + args.benchDir + "/"

def main():
    # Set base addresses
    baseAddress = 0x10020000
    maxAddress = 0x13ffffff
    # Load in the YAML file
    stream = open(workingDirectory + 'config.yml', "r")
    config = yaml.load_all(stream, Loader=yaml.FullLoader)
    # Parse YAML File
    baseAddress, clusters = parseYAMLFile(config, baseAddress)
    # Generate SALAM Config
    genConfigFile(clusters)
    # Parse original header for custom code
    headerList = parseOriginalHeader(clusters)
    # Make the header files with custom code
    genHeaderFiles(headerList, clusters)
    # Generate full system file
    shutil.copyfile(M5_Path + "/SALAM-Configurator/fs_template.py", CONFIG_Path + "fs_" + fileName + ".py")
    f = open(CONFIG_Path + "fs_" + fileName + ".py", "r")
    fullSystem = f.readlines()
    fullSystem[65] = "import " + fileName
    fullSystem[229] = "        " + fileName + ".makeHWAcc(args, test_sys)\n"
    f = open(CONFIG_Path + "fs_" + fileName + ".py", "w")
    f.writelines(fullSystem)
    # Warn if the size is greater than allowed
    if(clusters[-1].clusterTopAddress>maxAddress):
        print("WARNING: Address range is greater than defined for gem5")

def parseYAMLFile(config, baseAddress):
    clusters = []
    # Load in each acc cluster and add it to the list
    for clusterList in config:
        clusterName = None
        dmas = []
        accs = []
        for listType,devices in clusterList.items():
            if listType == "acc_cluster":
                for device in devices:
                    if "Name" in device:
                        clusterName = device['Name']
                    if "DMA" in device:
                        dmas.append(device)
                    if "Accelerator" in device:
                        accs.append(device)
        clusters.append(AccCluster(clusterName, dmas, accs, baseAddress, M5_Path))
        baseAddress = clusters[-1].clusterTopAddress + (64 - (int(clusters[-1].clusterTopAddress) % 64))
        if (int(baseAddress) % 64) != 0:
            print("Address Alignment Error: " + hex(baseAddress))
    return baseAddress, clusters

def genConfigFile(clusters):
    # Write out config file
    with open(CONFIG_Path + fileName + ".py", 'w') as writer:
        writer.write(imports)
        writer.write(l1Cache)
        for cluster in clusters:
            for line in cluster.genConfig():
                writer.write(line + "\n")
            # Add cluster definitions here
            for dma in cluster.dmas:
                writeLines(writer, dma.genConfig())
            # Come back here later and see if you can get away with doing these at the same time (probably not)
            for acc in cluster.accs:
                writeLines(writer, acc.genDefinition())
            for acc in cluster.accs:
                writeLines(writer, acc.genConfig())
        # Write cluster creation
        writer.write("def makeHWAcc(args, system):\n\n")
        for i in clusters:
            writer.write("	system." + i.name.lower() + " = AccCluster()" + "\n")
            writer.write("	build" + i.name + "(args, system, system." + i.name.lower() + ")\n\n")

def parseOriginalHeader(clusters):
    begin = None
    end = None
    # Read in existing header
    headerList = []
    for i in clusters:
        try:
            # f = open(workingDirectory + i.name  + "_" + args.headerName + ".h", 'r')
            f = open(workingDirectory + i.name  + "_hw_defines.h", 'r')
            oldHeader = f.readlines()
            for i in range(0,len(oldHeader)):
                if oldHeader[i] == "//BEGIN GENERATED CODE\n":
                    begin = i
                elif oldHeader[i] == "//END GENERATED CODE\n" or oldHeader[i] == "//END GENERATED CODE":
                    end = i
            del oldHeader[begin:end+1]
            headerList.append(oldHeader)
        except:
            print("No Header Found")
            emptyList = []
            headerList.append(emptyList)
    return headerList

def genHeaderFiles(headerList, clusters):
    # Write out headers
    for currentHeader in headerList:
        for cluster in clusters:
            with open(workingDirectory + cluster.name  + "_hw_defines.h", 'w') as f:
                currentHeader.append("//BEGIN GENERATED CODE\n")
                currentHeader.append("//Cluster: " + cluster.name.upper() + "\n")
                for dma in cluster.dmas:
                    if dma.dmaType == "NonCoherent":
                        currentHeader.append("//" + dma.dmaType + "DMA" + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_Flags " + hex(dma.address) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_RdAddr " + hex(dma.address + 1) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_WrAddr " + hex(dma.address + 9) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_CopyLen " + hex(dma.address + 17) + "\n")
                    elif dma.dmaType == "Stream":
                        currentHeader.append("//" + dma.dmaType + "DMA" + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_Flags " + hex(dma.address) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_RdAddr " + hex(dma.address + 4) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_WrAddr " + hex(dma.address + 12) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_RdFrameSize " + hex(dma.address + 20) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_NumRdFrames " + hex(dma.address + 24) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_RdFrameBufSize " + hex(dma.address + 25) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_WrFrameSize " + hex(dma.address + 26) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_NumWrFrames " + hex(dma.address + 30) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_WrFrameBufSize " + hex(dma.address + 31) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_Stream " + hex(dma.address + 32) + "\n")
                        currentHeader.append("#define " + dma.name.upper() + "_Status " + hex(dma.statusAddress) + "\n")
                for acc in cluster.accs:
                    currentHeader.append("//Accelerator: " + acc.name.upper() + "\n")
                    currentHeader.append("#define " + acc.name.upper() + " " + hex(acc.address) + "\n")
                    for var in acc.variables:
                        if "Cache" in var.type:
                            continue
                        elif "Stream" in var.type:
                            currentHeader.append("#define " + var.name + " " + hex(var.address) + "\n")
                            currentHeader.append("#define " + var.name + "_Status " + hex(var.statusAddress) + "\n")
                        else:
                            currentHeader.append("#define " + var.name + " " + hex(var.address) + "\n")
                currentHeader.append("//END GENERATED CODE")
                f.writelines(currentHeader)
                currentHeader = []

def writeLines(writer, lines):
    for line in lines:
        writer.write("	" + line + "\n")

if __name__ == "__main__":
    main()