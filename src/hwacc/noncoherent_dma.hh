#ifndef __HWACC_NONCOHERENT_DMA_HH__
#define __HWACC_NONCOHERENT_DMA_HH__
//------------------------------------------//
#include "dev/arm/base_gic.hh"
#include "dev/dma_device.hh"
#include "hwacc/LLVMRead/src/debug_flags.hh"
#include "hwacc/dma_write_fifo.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "params/NoncoherentDma.hh"

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

    EventFunctionWrapper tickEvent;

  protected:
    DmaPort accPort;
    DmaReadFifo * getActiveReadFifo();
    DmaWriteFifo * getActiveWriteFifo();
  public:
    PARAMS(NoncoherentDma);
    NoncoherentDma(const NoncoherentDmaParams &p);
    ~NoncoherentDma() {}

    AddrRangeList getAddrRanges() const;

    void tick();

    Tick read(PacketPtr pkt);
    Tick write(PacketPtr pkt);

    Port &getPort(const std::string &if_name,
                  PortID idx=InvalidPortID) override;
};

#endif //_HWACC_NONCOHERENT_DMA_HH__
