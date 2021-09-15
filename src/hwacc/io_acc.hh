#ifndef __HWACC_IO_ACC_HH__
#define __HWACC_IO_ACC_HH__
//------------------------------------------//
#include "base/trace.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"
#include "params/IOAcc.hh"
#include "dev/io_device.hh"
#include "dev/arm/base_gic.hh"
#include "hwacc/LLVMRead/src/debug_flags.hh"
//------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <queue>
//------------------------------------------//

class IOAcc : public BasicPioDevice
{
  private:
    Addr io_addr;
    Addr io_size;
    std::string devname;
    BaseGic *gic;
    uint32_t int_num;

    class MemSidePort : public RequestPort
    {
      friend class IOAcc;

      private:
        IOAcc *owner;
        std::queue<PacketPtr> outstandingPkts;

      public:
        MemSidePort(const std::string& name, IOAcc *owner) :
          RequestPort(name, owner), owner(owner)
        { }

      protected:
        virtual bool recvTimingResp(PacketPtr pkt);
        virtual void recvReqRetry();
        virtual void recvRangeChange() { };
        virtual Tick recvAtomic(PacketPtr pkt) {return 0;}
        virtual void recvFunctional(PacketPtr pkt) { };
        void setStalled(PacketPtr pkt)
        {
          outstandingPkts.push(pkt);
        }
        bool isStalled() { return !outstandingPkts.empty(); }
        void sendPacket(PacketPtr pkt);
    };

    class TickEvent : public Event
    {
      private:
        IOAcc *acc;

      public:
        TickEvent(IOAcc *_acc) : Event(CPU_Tick_Pri), acc(_acc) {}
        void process() { acc->tick(); }
        virtual const char *description() const { return "IOAcc tick"; }
    };

    MemSidePort memPort;
    MemSidePort* dataPort;
    IOAcc *acc;
    MasterID masterId;
    TickEvent tickEvent;
    unsigned int cacheLineSize;
    unsigned int cacheSize;

    void tick();

    bool needToRead;
    bool needToWrite;
    Addr currentReadAddr;
    Addr currentWriteAddr;
    Addr beginAddr;
    Tick writeLeft;
    Tick writeDone;
    Tick readLeft;
    Tick readDone;
    Tick totalLength;

    uint8_t *curData;
    bool *readsDone;
    bool running;
    bool computationNeeded;

    void tryRead();
    void tryWrite();

    Addr dataAddr;

    uint8_t *mmreg;
    uint32_t mmrval;

    bool processingDone;
    int processDelay;
    int clock_period;

  public:
    typedef IOAccParams Params;
    const Params *
    params() const
    {
      return dynamic_cast<const Params *>(_params);
    }

    IOAcc(Params *p);

    virtual Tick read(PacketPtr pkt);

    virtual Tick write(PacketPtr pkt);

    Port& getPort(const std::string& if_name,
                                  PortID idk = InvalidPortID) override;

    void recvPacket(PacketPtr pkt);

    int prepRead(Addr src, size_t length);
    int prepWrite(Addr dst, uint8_t* value, size_t length);
    int getCacheSize() { return cacheSize; }
    void processData();

    uint8_t* getCurData() { return curData; }

    bool isRunning() { return running; }
    bool isCompNeeded() { return computationNeeded; }

    uint64_t getMMRData(unsigned index) { return *(uint64_t *)(mmreg + DEV_MEM_LOC + index * 8); }
    int getProcessDelay() { return processDelay; }

  protected:
    static const int DEV_CONFIG = 0x00;
    static const int DEV_MEM_LOC = 0x04;
};

#endif //__HWACC_IO_ACC_HH__

/*
* MM Register Layout
* | Location of Data 32bits | Compute Finished 1bit | Unused 30bits | Start Operation 1bit |
*/
