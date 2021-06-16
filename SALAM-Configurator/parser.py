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
			for i in dma['DMA']:
				# Decide whether the DMA is NonCoherent or Stream
				if 'NonCoherent' in i['Type'] :
					pioSize = 21
					pioMasters = []
					if 'PIOMaster' in i:
						pioMasters.extend((i['PIOMaster'].split(',')))
					dmaClass.append(Dma(i['Name'], pioSize, pioMasters, topAddress, i['Type'],
						i['InterruptNum'], i['BufferSize'], i['MaxReq']))
					topAddress = topAddress + int(pioSize)
				elif 'Stream' in i['Type']:
					pioSize = 32
					pioMasters = []
					if 'PIOMaster' in i:
						pioMasters.extend((i['PIOMaster'].split(',')))
					dmaClass.append(StreamDma(i['Name'], pioSize, pioMasters, topAddress, i['Type'],
						i['ReadInt'], i['WriteInt'], i['Size']))
					topAddress = topAddress + int(i['Size'] + pioSize)
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
			IrPath = None
			configPath = None
			debug = False

			# Find the name first...
			# Also, find a non-stupid way to find the name first
			for i in acc['Accelerator']:
				if 'Name' in i:
					name = i['Name']
			# Parse the rest of the parameters
			for i in acc['Accelerator']:
				if 'PIOSize' in i:
					pioAddress = topAddress
					pioSize = i['PIOSize']
					topAddress = topAddress + i['PIOSize']
				if 'IrPath' in i:
					IrPath = i['IrPath']
				if 'ConfigPath' in i:
					configPath = i['ConfigPath']
				if 'PIOMaster' in i:
					pioMasters.extend((i['PIOMaster'].split(',')))
				if 'StreamIn' in i:
					streamIn.extend((i['StreamIn'].split(',')))
				if 'StreamOut' in i:
					streamOut.extend((i['StreamOut'].split(',')))
				if 'LocalSlaves' in i:
					localConnections.extend((i['LocalSlaves'].split(',')))
				if 'InterruptNum' in i:
					intNum = i['InterruptNum']
				if 'Debug' in i:
					debug = i['Debug']
				if 'Var' in i:
					for j in i['Var']:
						dictTest = dict(j)
						dictTest['Address'] = topAddress
						dictTest['AccName'] = name
						variables.append(Variable(**dictTest))
						if "SPM" in j['Type']:
							topAddress = topAddress + int(j['Size'])
						elif "Stream" in j['Type']:
							topAddress = topAddress + int(j['StreamSize'])
						elif "Cache" in j['Type']:
							# Don't need to change anything for cache
							topAddress = topAddress
						else:
							# Should never get here... but just in case throw an exception
							exceptionString = ("The Variable: " + kwargs.get('Name')
							+ " has an invalid type named: " + self.type)
							raise Exception(exceptionString)
			accClass.append(Accelerator(name, pioMasters, localConnections,
				pioAddress, pioSize, configPath , IrPath, streamIn, streamOut, intNum, M5_Path, variables, debug))

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
		lines.append("	system.iobus.master = clstr.local_bus.slave")
		# Need to define l2coherency in the YAML file?
		lines.append("	clstr._connect_caches(system, options, l2coherent=False)")
		lines.append("	gic = system.realview.gic")
		lines.append("")

		return lines

class Accelerator:

	def __init__(self, name, pioMasters, localConnections, address,
		size, configPath, irPath, streamIn, streamOut, intNum, M5_Path, variables = None, debug = False):

		self.name = name.lower()
		self.pioMasters = pioMasters
		self.localConnections = localConnections
		self.address = address
		self.size = size
		self.variables = variables
		self.configPath = configPath
		self.irPath = irPath
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
		lines.append("config = " + "\"" + self.M5_Path + "/" + self.configPath + "\"")
		lines.append("ir = "  + "\"" + self.M5_Path + "/" + self.irPath + "\"")

		# Add interrupt number if it exists
		if self.intNum is not None:
			lines.append("clstr." + self.name +" = CommInterface(devicename=acc, gic=gic, pio_addr="
			+ str(hex(self.address)) + ", pio_size=" + str(self.size) + ", int_num=" + str(self.intNum) + ")")
		else:
			lines.append("clstr." + self.name +" = CommInterface(devicename=acc, gic=gic, pio_addr="
			+ str(hex(self.address)) + ", pio_size=" + str(self.size) + ")")

		lines.append("AccConfig(clstr." + self.name + ", config, ir)")
		lines.append("")

		return lines

	def genConfig(self):
		lines = []

		lines.append("# " + self.name + " Config")

		for i in self.localConnections:
				if "LocalBus" in i:
					lines.append("clstr." + self.name + ".local = clstr.local_bus.slave")
				else:
					lines.append("clstr." + self.name + ".local = clstr." + i.lower() + ".pio")

		# Assign PIO Masters
		for i in self.pioMasters:
			if "LocalBus" in i:
				lines.append("clstr." + self.name + ".pio = clstr.local_bus.master")
			else:
				lines.append("clstr." + self.name + ".pio " +
					"=" " clstr." + i + ".local")
		# Add StreamIn
		for i in self.streamIn:
			lines.append("clstr." + self.name + ".stream = clstr." + i.lower() + ".stream_in")
		# Add StreamOut
		for i in self.streamOut:
			lines.append("clstr." + self.name + ".stream = clstr." + i.lower() + ".stream_out")

		lines.append("clstr." + self.name + ".enable_debug_msgs = " + str(self.debug))
		lines.append("")

		# Add scratchpad variables
		for i in self.variables:
			# Have the variable create its config
			lines = i.genConfig(lines)
			# Add a connection to the acc's spm ports
			if i.type == 'SPM':
				lines.append("	clstr." + self.name.lower() + ".spm = " + "clstr." + i.name.lower() + ".spm_ports")
			lines.append("")
		# Return finished config portion
		return lines

class StreamDma:
	def __init__(self, name, pio, pioMasters, address, dmaType, rd_int = None, wr_int = None, size = 64):
		self.name = name.lower()
		self.pio = pio
		self.pioMasters = pioMasters
		self.size = size
		self.address = address
		self.dmaType = dmaType
		self.rd_int = rd_int
		self.wr_int = wr_int

		for i in self.pioMasters:
			count = 0
			if "localbus" in i.lower():
				pioMasters[count] = "local_bus"
				count += 1
	# Probably could apply the style used here in other genConfigs
	def genConfig(self):
		lines = []
		dmaPath = "clstr." + self.name + "."
		systemPath = "clstr."
		# Need to fix max_pending?
		lines.append("# Stream DMA")
		lines.append("clstr." + self.name + " = StreamDma(pio_addr=" + hex(self.address) +
			", pio_size = " + str(self.pio) + ", gic=gic, max_pending = " + str(self.pio) + ")")
		lines.append(dmaPath + "stream_addr = " + hex(self.address) + " + " + str(self.pio))
		lines.append(dmaPath + "stream_size = " + str(self.size))
		lines.append(dmaPath + "pio_delay = '1ns'")
		lines.append(dmaPath + "rd_int = " + str(self.rd_int))
		lines.append(dmaPath + "wr_int = " + str(self.wr_int))
		lines.append("clstr." + self.name + ".dma = clstr.coherency_bus.slave")
		if self.pioMasters is not None:
			for i in self.pioMasters:
				lines.append("clstr." + i.lower() + ".master = clstr." + self.name + ".pio" )
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

		for i in self.pioMasters:
			count = 0
			if "localbus" in i.lower():
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
		lines.append(dmaPath + "cluster_dma = " + systemPath + "local_bus.slave")
		lines.append(dmaPath + "max_req_size = " + str(self.maxReq))
		lines.append(dmaPath + "buffer_size = " + str(self.size))
		lines.append("clstr." + self.name + ".dma = clstr.coherency_bus.slave")
		if self.pioMasters is not None:
			for i in self.pioMasters:
				lines.append("clstr." + i.lower() + ".master = clstr." + self.name + ".pio" )
		lines.append("")

		return lines

class Variable:
	def __init__ (self, **kwargs):
		# Read the type first
		self.type = kwargs.get('Type')
		if self.type == 'SPM':
			# Read in SPM args
			self.name = kwargs.get('Name')
			self.size = kwargs.get('Size')
			self.ports = kwargs.get('Ports')
			self.address = kwargs.get('Address')
			self.readyMode = kwargs.get('ReadyMode', False)
		elif self.type == 'Stream':
			# Read in Stream args
			self.name = kwargs.get('Name')
			self.inCon = kwargs.get('InCon')
			self.outCon = kwargs.get('OutCon')
			self.streamSize = kwargs.get('StreamSize')
			self.bufferSize = kwargs.get('BufferSize')
			self.address = kwargs.get('Address')
			# Convert connection definitions to lowercase
			self.inCon = self.inCon.lower()
			self.outCon = self.outCon.lower()
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
			lines.append("clstr." + self.name.lower() + " = StreamBuffer(stream_address = addr, stream_size = "
			+ str(self.streamSize) + ", buffer_size = " + str(self.bufferSize) + ")")
			lines.append("clstr." + self.inCon + ".stream = " + "clstr." + self.name.lower() + ".stream_in")
			lines.append("clstr." + self.outCon + ".stream = " + "clstr." + self.name.lower() + ".stream_out")
			lines.append("")
		# Scratchpad Memory
		elif self.type == 'SPM':
			lines.append("# " + self.name + " (Variable)")
			lines.append("addr = " + hex(self.address))
			lines.append("spmRange = AddrRange(addr, addr + " + hex(self.size) + ")")
			# Choose a style with the "."s and pick it
			lines.append("clstr." + self.name.lower() + " = ScratchpadMemory(range = spmRange)")
			# Probably need to add table and read mode to the YAML File
			lines.append("clstr." + self.name.lower() + "." + "conf_table_reported = False")
			lines.append("clstr." + self.name.lower() + "." + "ready_mode = " + str(self.readyMode))
			lines.append("clstr." + self.name.lower() + "." + "port" + " = " + "clstr.local_bus.master")
			lines.append("for i in range(" + str(self.ports) + "):")
		# L1 Cache, need to add L2 still...
		elif self.type == 'Cache':
			lines.append("# " + self.name + " (Cache)")
			lines.append("clstr." + self.name + " = L1Cache(size = '" + str(self.size) + "B')")
			lines.append("clstr." + self.name + ".mem_side = clstr.coherency_bus.slave")
			lines.append("clstr." + self.name + ".cpu_side = clstr." + self.accName + ".local")
		else:
			# Should never get here... but just in case throw an exception
			exceptionString = ("The variable: " + kwargs.get('Name')
			+ " has an invalid type named: " + self.type)
			raise Exception(exceptionString)
		return lines