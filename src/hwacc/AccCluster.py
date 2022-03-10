from m5.params import *
from m5.proxy import *
from m5.objects.Device import BasicPioDevice, PioDevice, IsaFake, BadAddr, DmaDevice
from m5.objects.Platform import Platform
from m5.objects.SimpleMemory import SimpleMemory
from m5.objects.SubSystem import SubSystem
from m5.objects.XBar import *
from m5.objects.Bridge import Bridge
from m5.objects.Cache import Cache
from m5.objects.CommInterface import CommInterface
from m5.objects.NoncoherentDma import NoncoherentDma
from m5.objects.StreamDma import StreamDma

class ClusterCache(Cache):
    assoc = 8
    tag_latency = 20
    data_latency = 20
    response_latency = 20
    mshrs = 20
    tgts_per_mshr = 12
    write_buffers = 8

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
        self.mem2cls = Bridge(delay='1ns', ranges = mem_range)
        self.mem2cls.master = self.local_bus.slave
        self.mem2cls.slave = system.membus.master

        # self.cls2mem = Bridge(delay='1ns', ranges = ext_ranges)
        # self.cls2mem.master = system.membus.slave
        # self.cls2mem.slave = self.local_bus.master

    def _connect_hwacc(self, hwacc):
        hwacc.pio = self.local_bus.master

    def _connect_caches(self, system, options, l2coherent, cache_size=0):
        if (cache_size!=0):
            self.cluster_cache = ClusterCache()
            self.cluster_cache.size = cache_size

            if options.l2cache and l2coherent:
                self.cluster_cache.mem_side = system.tol2bus.slave
            else:
                self.cluster_cache.mem_side = system.membus.slave
            self.coherency_bus.master = self.cluster_cache.cpu_side
        else:
            if options.l2cache and l2coherent:
                self.coherency_bus.master = system.tol2bus.slave
            else:
                self.coherency_bus.master = system.membus.slave

    def _connect_dma(self, system, dma):
        dma.pio = self.local_bus.master
        dma.dma = self.coherency_bus.slave

    def _connect_cluster_dma(self, system, dma):
        self._connect_dma(system, dma)
        dma.cluster_dma = self.local_bus.slave
