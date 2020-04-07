#ifndef __HWACC_NONCOHERENT_DMA_HH__
#define __HWACC_NONCOHERENT_DMA_HH__
//------------------------------------------//
#include "hwacc/LLVMRead/src/debug_flags.hh"
#include "params/NoncoherentDma.hh"
#include "dev/dma_device.hh"
#include "hwacc/dma_write_fifo.hh"
#include "dev/arm/base_gic.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
//------------------------------------------ 
//    Memory Map
//    |  Length  | Dst Addr | Src Addr | Flags  |
//    |----------|----------|----------|--------|
//    |  4 Bytes | 8 Bytes  | 8 Bytes  | 1 Byte |
//------------------------------------------//

class NoncoherentDma : public DmaDevice
{
  private:
    std::string devname;
    DmaReadFifo *memSideReadFifo;
    DmaReadFifo *accSideReadFifo;
    DmaReadFifo *readFifo;
    DmaWriteFifo *memSideWriteFifo;
    DmaWriteFifo *accSideWriteFifo;
    DmaWriteFifo *writeFifo;
    Addr pioAddr;
    Addr pioDelay;
    Addr pioSize;
    size_t bufferSize;
    unsigned maxPending;
    unsigned maxReqSize;
    BaseGic * gic;
    uint32_t intNum;
    int clock_period;

    uint8_t * mmreg;
    uint8_t * FLAGS;
    uint64_t * SRC;
    uint64_t * DST;
    int * LEN;

	uint8_t last_flag;

    Addr activeSrc;
    Addr activeDst;
    int writesLeft;
    bool running;

    Tick start_time;

    class TickEvent : public Event
    {
      private:
        NoncoherentDma *dma;

      public:
        TickEvent(NoncoherentDma *_dma) : Event(CPU_Tick_Pri), dma(_dma) {}
        void process() { dma->tick(); }
        virtual const char *description() const { return "NoncoherentDma tick"; }
    };

    TickEvent tickEvent;

  protected:
    DmaPort accPort;
    DmaReadFifo * getActiveReadFifo();
    DmaWriteFifo * getActiveWriteFifo();
  public:
    typedef NoncoherentDmaParams Params;

    const Params *
    params() const
    {
      return dynamic_cast<const Params *>(_params);
    }

    NoncoherentDma(const Params *p);
    ~NoncoherentDma() {}

    AddrRangeList getAddrRanges() const;

    void tick();

    Tick read(PacketPtr pkt);
    Tick write(PacketPtr pkt);

    Port &getPort(const std::string &if_name,
                  PortID idx=InvalidPortID) override;
};

#endif //_HWACC_NONCOHERENT_DMA_HH__
