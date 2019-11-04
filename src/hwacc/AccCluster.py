from m5.params import *
from m5.proxy import *
from Device import BasicPioDevice, PioDevice, IsaFake, BadAddr, DmaDevice
from Platform import Platform
from SimpleMemory import SimpleMemory
from SubSystem import SubSystem
from XBar import NoncoherentXBar
from Bridge import Bridge
from Cache import Cache
from CommInterface import CommInterface
from NoncoherentDma import NoncoherentDma
from StreamDma import StreamDma

class AccCluster(Platform):
    type = 'AccCluster'
    cxx_header = "hwacc/acc_cluster.hh"
    system = Param.System(Parent.any, "system")
    
    # System Cache Parameter
    cache_size = Param.String('4kB', "cache size in bytes")
    cache_ports = Param.Int32(4, "read/write ports for cache bus")
    local_ports = Param.Int32(4, "read/write ports for local bus")
    local_range_min = Param.Unsigned(0x2f000000, "minimal address of local range")
    local_range_max = Param.Unsigned(0x7fffffff, "maximum address of local range")
    external_range_low_min = Param.Unsigned(0x00000000, "minimal address of external range low")
    external_range_low_max = Param.Unsigned(0x2effffff, "maximum address of external range low")
    external_range_hi_min = Param.Unsigned(0x80000000, "minimal address of external range high")
    external_range_hi_max = Param.Unsigned(0xffffffff, "maximum address of external range high")
    
    local_bus = NoncoherentXBar(width=2, frontend_latency=1, forward_latency=0, response_latency=1)
    cache_bus = NoncoherentXBar(width=2, frontend_latency=1, forward_latency=0, response_latency=1)

    def _resize_bus(self, local_ports, cache_ports):
        self.local_bus = NoncoherentXBar(width=local_ports, frontend_latency=1, forward_latency=0, response_latency=1)
        self.cache_bus = NoncoherentXBar(width=cache_ports, frontend_latency=1, forward_latency=0, response_latency=1)

    def _add_spm(self, spm_range, spm_latency):
        self.spm = SimpleMemory(range=spm_range, conf_table_reported=False, latency=spm_latency)
        self.spm.port = self.local_bus.master

    def _connect_spm(self, spm):
        spm.port = self.local_bus.master

    def _attach_bridges(self, system, mem_range, ext_ranges):
        system.mem_to_hwacc = Bridge(delay='1ns', ranges = mem_range)
        system.mem_to_hwacc.master = self.local_bus.slave
        system.mem_to_hwacc.slave = system.membus.master

        system.hwacc_to_mem = Bridge(delay='1ns', ranges = ext_ranges)
        system.hwacc_to_mem.master = system.membus.slave
        system.hwacc_to_mem.slave = self.local_bus.master

    def _connect_hwacc(self, hwacc):
        hwacc.pio = self.local_bus.master
        hwacc.dram_side = self.cache_bus.slave
        hwacc.spm_side = self.local_bus.slave

    def _connect_caches(self, system, options, cache_size):
        if options.acc_cache:
            self.cache = Cache(assoc=4, tag_latency=1, data_latency=1, response_latency=1, mshrs=16, size=cache_size, tgts_per_mshr=20)

            if options.l2cache:
                self.cache.mem_side = system.tol2bus.slave
            else:
                self.cache.mem_side = system.membus.slave
            self.cache_bus.master = self.cache.cpu_side
        else:
            if options.l2cache:
                self.cache_bus.master = system.tol2bus.slave
            else:
                self.cache_bus.master = system.membus.slave

    def _connect_dma(self, system, dma):
        dma.pio = self.local_bus.master
        dma.dma = system.membus.slave
