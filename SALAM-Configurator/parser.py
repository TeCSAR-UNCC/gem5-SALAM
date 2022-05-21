class AccCluster:
    def __init__(self, name, dmas, accs, baseAddress, M5_Path):
        self.name = name
        self.dmas = dmas
        self.accs = accs
        self.clusterBaseAddress = baseAddress
        self.clusterTopAddress = baseAddress
        self.processConfig(M5_Path)

    def processConfig(self, M5_Path):
        dmaClass = []
        accClass = []
        topAddress = self.clusterBaseAddress

        # Parse DMAs
        for dma in self.dmas:
            for deviceDict in dma['DMA']:
                # Decide whether the DMA is NonCoherent or Stream
                if 'NonCoherent' in deviceDict['Type'] :
                    pioSize = 21
                    pioMasters = []
                    if 'PIOMaster' in deviceDict:
                        pioMasters.extend((deviceDict['PIOMaster'].split(',')))
                    if 'InterruptNum' in deviceDict:
                        dmaClass.append(Dma(deviceDict['Name'], pioSize, pioMasters, topAddress, deviceDict['Type'],
                        deviceDict['InterruptNum'], deviceDict['BufferSize'], deviceDict['MaxReqSize']))
                    else:
                        dmaClass.append(Dma(deviceDict['Name'], pioSize, pioMasters, topAddress, deviceDict['Type'],
                        deviceDict['BufferSize'], deviceDict['MaxReqSize']))
                    aligned_inc = int(pioSize) + (64 - (int(pioSize) % 64))
                    topAddress = topAddress + aligned_inc
                elif 'Stream' in deviceDict['Type']:
                    pioSize = 32
                    statusSize = 4
                    pioMasters = []

                    alignedStatusInc = int(statusSize) + (64 - (int(statusSize) % 64))
                    aligned_inc = int(pioSize) + (64 - (int(pioSize) % 64))

                    statusAddress = topAddress + aligned_inc
                    
                    if 'PIOMaster' in deviceDict:
                        pioMasters.extend((deviceDict['PIOMaster'].split(',')))
                    # Can come back and get rid of this if/else tree
                    if 'ReadInt' in deviceDict:
                        if 'WriteInt' in deviceDict:
                            dmaClass.append(StreamDma(deviceDict['Name'], pioSize, pioMasters, topAddress, statusAddress, deviceDict['Type'],
                            deviceDict['ReadInt'], deviceDict['WriteInt'], deviceDict['BufferSize']))
                        else:
                            dmaClass.append(StreamDma(deviceDict['Name'], pioSize, pioMasters, topAddress, statusAddress, deviceDict['Type'],
                            deviceDict['ReadInt'], None, deviceDict['BufferSize']))
                    elif 'WriteInt' in deviceDict:
                            dmaClass.append(StreamDma(deviceDict['Name'], pioSize, pioMasters, topAddress, statusAddress, deviceDict['Type'],
                            None, deviceDict['WriteInt'], deviceDict['BufferSize']))
                    else:
                            dmaClass.append(StreamDma(deviceDict['Name'], pioSize, pioMasters, topAddress, statusAddress, deviceDict['Type'],
                            None, None, deviceDict['BufferSize']))

                    # Increment Top Address
                    topAddress = topAddress + aligned_inc + alignedStatusInc
        # Parse Accelerators
        for acc in self.accs:
            # To handle Accs out of order, create a flag that Accs referenced already exist.
            # Search accs for it.
            name = None
            pioMasters = []
            streamIn = []
            streamOut = []
            localConnections = []
            variables = []
            pioAddress = None
            pioSize = None
            intNum = None
            irPath = None
            configPath = None
            debug = False

            # Find the name first...
            # Also, find a non-stupid way to find the name first
            for deviceDict in acc['Accelerator']:
                if 'Name' in deviceDict:
                    name = deviceDict['Name']
            # Parse the rest of the parameters
            for deviceDict in acc['Accelerator']:
                if 'PIOSize' in deviceDict:
                    pioAddress = topAddress
                    pioSize = deviceDict['PIOSize'] + (64 - (deviceDict['PIOSize'] % 64))
                    topAddress = topAddress + pioSize
                    if ((topAddress + pioSize) % 64) != 0:
                        print("Acc Error: " + hex(pioAddress))
                if 'IrPath' in deviceDict:
                    irPath = deviceDict['IrPath']
                if 'ConfigPath' in deviceDict:
                    configPath = deviceDict['ConfigPath']
                if 'PIOMaster' in deviceDict:
                    pioMasters.extend((deviceDict['PIOMaster'].split(',')))
                if 'StreamIn' in deviceDict:
                    streamIn.extend((deviceDict['StreamIn'].split(',')))
                if 'StreamOut' in deviceDict:
                    streamOut.extend((deviceDict['StreamOut'].split(',')))
                if 'LocalSlaves' in deviceDict:
                    localConnections.extend((deviceDict['LocalSlaves'].split(',')))
                if 'InterruptNum' in deviceDict:
                    intNum = deviceDict['InterruptNum']
                if 'Debug' in deviceDict:
                    debug = deviceDict['Debug']
                if 'Var' in deviceDict:
                    for var in deviceDict['Var']:
                        # Setup the variable's parameters to pass
                        varParams = dict(var)
                        varParams['Address'] = topAddress
                        varParams['AccName'] = name
                        
                        if varParams['Type'] == "Stream":
                            aligned_inc = int(var['StreamSize']+4) + (64 - (int(var['StreamSize']+4) % 64))
                            statusAddress = topAddress + aligned_inc
                            varParams['StatusAddress'] = statusAddress

                        # Create and append a new variable
                        variables.append(Variable(**varParams))
                        # Increment the current address based on size
                        if "SPM" in var['Type']:
                            aligned_inc = int(var['Size']) + (64 - (int(var['Size']) % 64))
                            topAddress = topAddress + aligned_inc
                        elif "Stream" in var['Type']:
                            statusSize = 4
                            aligned_inc = int(var['StreamSize']+4) + (64 - (int(var['StreamSize']+4) % 64))
                            status_inc = int(statusSize) + (64 - (int(statusSize) % 64))
                            topAddress = topAddress + aligned_inc + status_inc
                        elif "RegisterBank" in var['Type']:
                            aligned_inc = int(var['Size']) + (64 - (int(var['Size']) % 64))
                            topAddress = topAddress + aligned_inc
                        elif "Cache" in var['Type']:
                            # Don't need to change anything for cache
                            topAddress = topAddress
                        else:
                            # Should never get here... but just in case throw an exception
                            exceptionString = ("The Variable: " + name
                            + " has an invalid type named: " + self.type)
                            raise Exception(exceptionString)
            # Append accelerator to the cluster
            accClass.append(Accelerator(name, pioMasters, localConnections,
                pioAddress, pioSize, irPath, configPath, streamIn, streamOut, intNum, M5_Path, variables, debug))

        self.accs = accClass
        self.dmas = dmaClass
        self.clusterTopAddress = topAddress

    def genConfig(self):
        lines = []
        # Need to add some customization here. Consider this a placeholder
        # Also need to edit AccCluster.py's addresses to match the gem5 supported ones
        lines.append("def build" + self.name + "(options, system, clstr):" + "\n")
        lines.append("	local_low = " + hex(self.clusterBaseAddress))
        lines.append("	local_high = " + hex(self.clusterTopAddress))
        lines.append("	local_range = AddrRange(local_low, local_high)")
        lines.append("	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]")
        lines.append("	system.iobus.mem_side_ports = clstr.local_bus.cpu_side_ports")
        # Need to define l2coherency in the YAML file?
        lines.append("	clstr._connect_caches(system, options, l2coherent=False)")
        lines.append("	gic = system.realview.gic")
        lines.append("")

        return lines

class Accelerator:

    def __init__(self, name, pioMasters, localConnections, address,
        size, irPath, configPath, streamIn, streamOut, intNum, M5_Path, variables = None, debug = False):

        self.name = name.lower()
        self.pioMasters = pioMasters
        self.localConnections = localConnections
        self.address = address
        self.size = size
        self.variables = variables
        self.irPath = irPath
        self.configPath = configPath
        self.streamIn = streamIn
        self.streamOut = streamOut
        self.M5_Path = M5_Path
        self.intNum = intNum
        self.debug = debug

    def genDefinition(self):
        lines = []
        lines.append("# " + self.name + " Definition")
        lines.append("acc = " + "\"" + self.name + "\"")
        # Need to add a user defined path & user defined interrupts here
        lines.append("ir = "  + "\"" + self.M5_Path + "/" + self.irPath + "\"")
        lines.append("config = "  + "\"" + self.M5_Path + "/" + self.configPath + "\"")

        # Add interrupt number if it exists
        if self.intNum is not None:
            lines.append("clstr." + self.name +" = CommInterface(devicename=acc, gic=gic, pio_addr="
            + str(hex(self.address)) + ", pio_size=" + str(self.size) + ", int_num=" + str(self.intNum) + ")")
        else:
            lines.append("clstr." + self.name +" = CommInterface(devicename=acc, gic=gic, pio_addr="
            + str(hex(self.address)) + ", pio_size=" + str(self.size) + ")")

        lines.append("AccConfig(clstr." + self.name + ", ir, config)")
        lines.append("")

        return lines

    def genConfig(self):
        lines = []

        lines.append("# " + self.name + " Config")

        for connection in self.localConnections:
                if "LocalBus" in connection:
                    lines.append("clstr." + self.name + ".local = clstr.local_bus.cpu_side_ports")
                else:
                    lines.append("clstr." + self.name + ".local = clstr." + connection.lower() + ".pio")

        # Assign PIO Masters
        for master in self.pioMasters:
            if "LocalBus" in master:
                lines.append("clstr." + self.name + ".pio = clstr.local_bus.mem_side_ports")
            else:
                lines.append("clstr." + self.name + ".pio " +
                    "=" " clstr." + i + ".local")
        # Add StreamIn
        for inCon in self.streamIn:
            lines.append("clstr." + self.name + ".stream = clstr." + inCon.lower() + ".stream_in")
        # Add StreamOut
        for outCon in self.streamOut:
            lines.append("clstr." + self.name + ".stream = clstr." + outCon.lower() + ".stream_out")

        lines.append("clstr." + self.name + ".enable_debug_msgs = " + str(self.debug))
        lines.append("")

        # Add scratchpad variables
        for var in self.variables:
            # Have the variable create its config
            lines = var.genConfig(lines)
            lines.append("")
        # Return finished config portion
        return lines

class StreamDma:  
    def __init__(self, name, pio, pioMasters, address, statusAddress, dmaType, rd_int = None, wr_int = None, size = 64):
        self.name = name.lower()
        self.pio = pio
        self.pioMasters = pioMasters
        self.size = size
        self.address = address
        self.statusAddress = statusAddress
        self.dmaType = dmaType
        self.rd_int = rd_int
        self.wr_int = wr_int

        for master in self.pioMasters:
            count = 0
            if "localbus" in master.lower():
                pioMasters[count] = "local_bus"
                count += 1
    # Probably could apply the style used here in other genConfigs
    def genConfig(self):
        lines = []
        dmaPath = "clstr." + self.name + "."
        # Need to fix max_pending?
        lines.append("# Stream DMA")
        lines.append("clstr." + self.name + " = StreamDma(pio_addr=" + hex(self.address) +
        ", status_addr=" + hex(self.statusAddress) + ", pio_size = " + str(self.pio) + ", gic=gic, max_pending = " + str(self.pio) + ")")
        lines.append(dmaPath + "stream_addr = " + hex(self.address) + " + " + str(self.pio))
        lines.append(dmaPath + "stream_size = " + str(self.size))
        lines.append(dmaPath + "pio_delay = '1ns'")
        if self.rd_int != None:
            lines.append(dmaPath + "rd_int = " + str(self.rd_int))
        if self.wr_int != None:
            lines.append(dmaPath + "wr_int = " + str(self.wr_int))
        lines.append("clstr." + self.name + ".dma = clstr.coherency_bus.cpu_side_ports")
        if self.pioMasters is not None:
            for master in self.pioMasters:
                lines.append("clstr." + master.lower() + ".mem_side_ports = clstr." + self.name + ".pio" )
        lines.append("")

        return lines

class Dma:
    def __init__(self, name, pio, pioMasters, address, dmaType, int_num = None, size = 64, maxReq = 4):
        self.name = name.lower()
        self.pio = pio
        self.pioMasters = pioMasters
        self.size = size
        self.address = address
        self.dmaType = dmaType
        self.int_num = int_num
        self.maxReq = maxReq

        for master in self.pioMasters:
            count = 0
            if "localbus" in master.lower():
                pioMasters[count] = "local_bus"
                count += 1
    # Probably could apply the style used here in other genConfigs
    def genConfig(self):
        lines = []
        dmaPath = "clstr." + self.name + "."
        systemPath = "clstr."
        lines.append("# Noncoherent DMA")
        lines.append("clstr." + self.name + " = NoncoherentDma(pio_addr="
            + hex(self.address) + ", pio_size = " + str(self.pio)
            + ", gic=gic, int_num=" + str(self.int_num) +")")
        lines.append(dmaPath + "cluster_dma = " + systemPath + "local_bus.cpu_side_ports")
        lines.append(dmaPath + "max_req_size = " + str(self.maxReq))
        lines.append(dmaPath + "buffer_size = " + str(self.size))
        lines.append("clstr." + self.name + ".dma = clstr.coherency_bus.cpu_side_ports")
        if self.pioMasters is not None:
            for master in self.pioMasters:
                lines.append("clstr." + master.lower() + ".mem_side_ports = clstr." + self.name + ".pio" )
        lines.append("")

        return lines

class PortedConnection:
    def __init__ (self, conName, numPorts):
        self.conName = conName
        self.numPorts = numPorts

class Variable:
    def __init__ (self, **kwargs):
        # Read the type first
        self.type = kwargs.get('Type')
        if self.type == 'SPM':
            self.connections = []
            # Read in SPM args
            self.name = kwargs.get('Name')
            self.accName = kwargs.get('AccName')
            self.size = kwargs.get('Size')
            self.ports = kwargs.get('Ports', 1)
            self.address = kwargs.get('Address')
            self.readyMode = kwargs.get('ReadyMode', False)
            self.resetOnRead = kwargs.get('ResetOnRead', True)
            self.readOnInvalid = kwargs.get('ReadOnInvalid', False)
            self.writeOnValid = kwargs.get('WriteOnValid', True)
            # Append the default connection here... probably need to be more elegant
            self.connections.append(PortedConnection(self.accName, self.ports))
            # Append other connections to the connections list
            if 'Connections' in kwargs:
                for conDef in kwargs.get('Connections').split(','):
                    con, numPorts = conDef.split(':')
                    self.connections.append(PortedConnection(con,numPorts))
        elif self.type == 'Stream':
            # Read in Stream args
            self.name = kwargs.get('Name')
            self.accName = kwargs.get('AccName')
            self.inCon = kwargs.get('InCon')
            self.outCon = kwargs.get('OutCon')
            self.streamSize = kwargs.get('StreamSize')
            self.bufferSize = kwargs.get('BufferSize')
            self.address = kwargs.get('Address')
            self.statusAddress = kwargs.get('StatusAddress')
            # Convert connection definitions to lowercase
            self.inCon = self.inCon.lower()
            self.outCon = self.outCon.lower()
        elif self.type == 'RegisterBank':
            self.connections = []
            # Read in SPM args
            self.name = kwargs.get('Name')
            self.accName = kwargs.get('AccName')
            self.size = kwargs.get('Size')
            self.address = kwargs.get('Address')
            # Append the default connection here... probably need to be more elegant
            self.connections.append(PortedConnection(self.accName, 1))
            # Append other connections to the connections list
            if 'Connections' in kwargs:
                for conDef in kwargs.get('Connections').split(','):
                    con, numPorts = conDef.split(':')
                    self.connections.append(PortedConnection(con,numPorts))		
        elif self.type == 'Cache':
            self.name = kwargs.get('Name')
            self.accName = kwargs.get('AccName')
            self.size = kwargs.get('Size')
        else:
            # Throw an exception if we don't know the type
            exceptionString = ("The variable: " + kwargs.get('Name')
            + " has an invalid type named: " + self.type)
            raise Exception(exceptionString)

    def genConfig(self, lines):
        # Add new variable configs here
        # Stream Buffer Variable
        if self.type == 'Stream':
            lines.append("# " + self.name + " (Stream Variable)")
            lines.append("addr = " + hex(self.address))
            lines.append("clstr." + self.name.lower() + " = StreamBuffer(stream_address = addr, status_address= " + hex(self.statusAddress) 
            + ", stream_size = " + str(self.streamSize) + ", buffer_size = " + str(self.bufferSize) + ")")
            lines.append("clstr." + self.inCon + ".stream = " + "clstr." + self.name.lower() + ".stream_in")
            lines.append("clstr." + self.outCon + ".stream = " + "clstr." + self.name.lower() + ".stream_out")
            lines.append("")
        # Scratchpad Memory
        elif self.type == 'SPM':
            lines.append("# " + self.name + " (Variable)")
            lines.append("addr = " + hex(self.address))
            lines.append("spmRange = AddrRange(addr, addr + " + hex(self.size) + ")")
            # When appending convert all connections to lowercase for standardization
            lines.append("clstr." + self.name.lower() + " = ScratchpadMemory(range = spmRange)")
            # Probably need to add table and read mode to the YAML File
            lines.append("clstr." + self.name.lower() + "." + "conf_table_reported = False")
            lines.append("clstr." + self.name.lower() + "." + "ready_mode = " + str(self.readyMode))
            lines.append("clstr." + self.name.lower() + "." + "reset_on_scratchpad_read = " + str(self.resetOnRead))
            lines.append("clstr." + self.name.lower() + "." + "read_on_invalid = " + str(self.readOnInvalid))
            lines.append("clstr." + self.name.lower() + "." + "write_on_valid = " + str(self.writeOnValid))
            lines.append("clstr." + self.name.lower() + "." + "port" + " = " + "clstr.local_bus.mem_side_ports")
            for con in self.connections:
                lines.append("")
                lines.append("# Connecting " + self.name + " to " + con.conName)
                lines.append("for i in range(" + str(con.numPorts) + "):")
                lines.append("	clstr." + con.conName.lower() + ".spm = " + "clstr." + self.name.lower() + ".spm_ports")
        # RegisterBank
        elif self.type == 'RegisterBank':
            lines.append("# " + self.name + " (Variable)")
            lines.append("addr = " + hex(self.address))
            lines.append("regRange = AddrRange(addr, addr + " + hex(self.size) + ")")
            # When appending convert all connections to lowercase for standardization
            lines.append("clstr." + self.name.lower() + " = RegisterBank(range = regRange)")
            lines.append("clstr." + self.name.lower() + "." + "load_port" + " = " + "clstr.local_bus.mem_side_ports")
            for con in self.connections:
                lines.append("")
                lines.append("# Connecting " + self.name + " to " + con.conName)
                lines.append("clstr." + con.conName.lower() + ".reg = " + "clstr." + self.name.lower() + ".reg_port")
        # L1 Cache, need to add L2 still...
        elif self.type == 'Cache':
            lines.append("# " + self.name + " (Cache)")
            lines.append("clstr." + self.name + " = L1Cache(size = '" + str(self.size) + "B')")
            lines.append("clstr." + self.name + ".mem_side = clstr.coherency_bus.cpu_side_ports")
            lines.append("clstr." + self.name + ".cpu_side = clstr." + self.accName + ".local")
        else:
            # Should never get here... but just in case throw an exception
            exceptionString = ("The variable: " + self.name
            + " has an invalid type named: " + self.type)
            raise Exception(exceptionString)
        return lines