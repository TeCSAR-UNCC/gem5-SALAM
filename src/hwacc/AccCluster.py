from m5.params import *
from m5.proxy import *
from Device import BasicPioDevice, PioDevice, IsaFake, BadAddr, DmaDevice
from Platform import Platform
from SimpleMemory import SimpleMemory
from SubSystem import SubSystem
from XBar import *
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
    cache_size = Param.String('32kB', "cache size in bytes")
    local_range_min = Param.Unsigned(0x2f000000, "minimal address of local range")
    local_range_max = Param.Unsigned(0x7fffffff, "maximum address of local range")
    external_range_low_min = Param.Unsigned(0x00000000, "minimal address of external range low")
    external_range_low_max = Param.Unsigned(0x2effffff, "maximum address of external range low")
    external_range_hi_min = Param.Unsigned(0x80000000, "minimal address of external range high")
    external_range_hi_max = Param.Unsigned(0xffffffff, "maximum address of external range high")

    local_bus = NoncoherentXBar(width=2, frontend_latency=1, forward_latency=0, response_latency=1)
    coherency_bus = CoherentXBar(width=2, frontend_latency=1, forward_latency=0, response_latency=1)
    coherency_bus.snoop_filter = SnoopFilter()
    coherency_bus.snoop_response_latency = 4
    coherency_bus.point_of_coherency = True
    coherency_bus.point_of_unification = True

    def _add_spm(self, spm_range, spm_latency):
        self.spm = SimpleMemory(range=spm_range, conf_table_reported=False, latency=spm_latency)
        self.spm.port = self.local_bus.master

    def _connect_spm(self, spm):
        spm.port = self.local_bus.master

    def _attach_bridges(self, system, mem_range, ext_ranges):
        system.mem_to_hwacc = Bridge(delay='1ns', ranges = mem_range)
        system.mem_to_hwacc.master = self.local_bus.slave
        system.mem_to_hwacc.slave = system.membus.master

        # system.hwacc_to_mem = Bridge(delay='1ns', ranges = ext_ranges)
        # system.hwacc_to_mem.master = system.membus.slave
        # system.hwacc_to_mem.slave = self.local_bus.master

    def _connect_hwacc(self, hwacc):
        hwacc.pio = self.local_bus.master

    def _connect_caches(self, system, options, cache_size):
        if options.acc_cache:
            self.cluster_cache = Cache(assoc=4, tag_latency=1, data_latency=1, response_latency=1, mshrs=16, size=cache_size, tgts_per_mshr=20)

            if options.l2cache:
                self.cluster_cache.mem_side = system.tol2bus.slave
            else:
                self.cluster_cache.mem_side = system.membus.slave
            self.coherency_bus.master = self.cluster_cache.cpu_side
        else:
            if options.l2cache:
                self.coherency_bus.master = system.tol2bus.slave
            else:
                self.coherency_bus.master = system.membus.slave

    def _connect_dma(self, system, dma):
        dma.pio = self.local_bus.master
        dma.dma = self.coherency_bus.slave

    def _connect_cluster_dma(self, system, dma):
        self._connect_dma(system, dma)
        dma.cluster_dma = self.local_bus.slave
