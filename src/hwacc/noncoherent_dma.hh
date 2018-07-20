#ifndef __HWACC_NONCOHERENT_DMA_HH__
#define __HWACC_NONCOHERENT_DMA_HH__

#include "params/NoncoherentDma.hh"
#include "dev/dma_device.hh"
#include "hwacc/dma_write_fifo.hh"
#include "dev/arm/base_gic.hh"

/*
    Memory Map
    |  Length  | Dst Addr | Src Addr | Flags  |
    |----------|----------|----------|--------|
    |  4 Bytes | 8 Bytes  | 8 Bytes  | 1 Byte |
*/

#define MIN(a,b) ((a) < (b) ? (a) : (b))

class NoncoherentDma : public DmaDevice
{
  private:
    std::string devname;
    DmaReadFifo *readFifo;
    DmaWriteFifo *writeFifo;
    Addr pioAddr;
    Addr pioDelay;
    Addr pioSize;
    size_t bufferSize;
    unsigned maxPending;
    unsigned maxReqSize;
    BaseGic * gic;
    uint32_t intNum;

    uint8_t * mmreg;
    uint8_t * FLAGS;
    uint64_t * SRC;
    uint64_t * DST;
    int * LEN;

    Addr activeSrc;
    Addr activeDst;
    int writesLeft;
    bool running;

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
};

#endif //_HWACC_NONCOHERENT_DMA_HH__
