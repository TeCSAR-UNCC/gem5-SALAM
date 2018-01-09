# -*- coding: utf-8 -*-
# Copyright (c) 2017 Jason Lowe-Power
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: Jason Lowe-Power

""" This file creates a barebones system and executes 'hello', a simple Hello
World application. Adds a simple memobj between the CPU and the membus.

This config file assumes that the x86 ISA was built.
"""

# import the m5 (gem5) library created when gem5 is built
import m5
import optparse
# import all of the SimObjects
from m5.objects import *
from m5.util import *
m5.util.addToPath('../../')
from common import PlatformConfig
from common import CacheConfig
from common import MemConfig
from common import Options
from common.Caches import *
from common.Benchmarks import *

# create the system we are going to simulate
system = System()

parser = optparse.OptionParser()
Options.addCommonOptions(parser)
(options, args) = parser.parse_args()

# Set the clock fequency of the system (and all of its children)
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the system
system.mem_mode = 'timing'               # Use timing accesses
system.mem_ranges = [AddrRange('4GB'), AddrRange(Addr('4GB'), size = '2GB')] # Create an address range

# Create a simple CPU
system.cpu = TimingSimpleCPU()

## Create the simple memory object
#system.memobj = SimpleMemobj()

## Hook the CPU ports up to the cache
#system.cpu.icache_port = system.memobj.inst_port
#system.cpu.dcache_port = system.memobj.data_port

# Create a memory bus, a coherent crossbar, in this case
system.membus = SystemXBar()
system.iobus = IOXBar()
system.bridge = Bridge(delay='50ns', ranges = system.mem_ranges[1])
system.bridge.master = system.iobus.slave
system.bridge.slave = system.membus.master
system.iobridge = Bridge(delay='50ns', ranges = system.mem_ranges[0])
system.iobridge.master = system.membus.slave
system.iobridge.slave = system.iobus.master

#system.gic_cpu_addr = system.realview.gic.cpu_addr
#system.flags_addr = system.realview.realview_io.pio_addr + 0x30

# Connect the system up to the membus
system.system_port = system.membus.slave
#CacheConfig.config_cache(options, system)
system.cpu.dcache_port = system.membus.slave
system.cpu.icache_port = system.membus.slave

# Create a DDR3 memory controller and connect it to the membus
system.mem_ctrl = DDR3_1600_8x8()
system.mem_ctrl.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.master

system.acc = IOAcc(pio_addr=0x100000000, pio_size=8);
system.acc.pio = system.iobus.master;
system.acc.mem_side = system.iobus.slave;
system.acc.comp_interface = CompInterface();

# create the interrupt controller for the CPU and connect to the membus
system.cpu.createInterruptController()

system.intrctrl = IntrControl()
system.terminal = Terminal()
system.vncserver = VncServer()
# Create a process for a simple "Hello World" application
process = Process()
# Set the command
# cmd is a list which begins with the executable (like argv)
process.cmd = ['tests/test-progs/virt']
# Set the cpu to use the process as its workload and create thread contexts
system.cpu.workload = process
system.cpu.createThreads()

# set up the root SimObject and start the simulation
root = Root(full_system = False, system = system)
# instantiate all of the objects we've created above
m5.instantiate()

print "Beginning simulation!"
exit_event = m5.simulate()
print 'Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause())
