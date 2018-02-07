#ifndef __HWACC_COMM_INTERFACE_HH__

#define __HWACC_COMM_INTERFACE_HH__

#include "params/CommInterface.hh"
#include "dev/io_device.hh"
#include "dev/arm/base_gic.hh"
#include "hwacc/compute_unit.hh"

#include <queue>

class CommInterface : public BasicPioDevice
{
  private:
    Addr io_addr;
    Addr io_size;
    Addr flag_size;
    Addr config_size;
    Addr FLAG_OFFSET;
    Addr CONFIG_OFFSET;
    Addr VAR_OFFSET;
    std::string devname;
    BaseGic *gic;
    uint32_t int_num;

    class MemSidePort : public MasterPort
    {
      friend class CommInterface;

      private:
        CommInterface *owner;
        std::queue<PacketPtr> outstandingPkts;

      public:
        MemSidePort(const std::string& name, CommInterface *owner) :
          MasterPort(name, owner), owner(owner)
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
        CommInterface *comm;

      public:
        TickEvent(CommInterface *_comm) : Event(CPU_Tick_Pri), comm(_comm) {}
        void process() { comm->tick(); }
        virtual const char *description() const { return "CommInterface tick"; }
    };

    MemSidePort memPort;
    MemSidePort* dataPort;
    CommInterface *comm;
    MasterID masterId;
    TickEvent tickEvent;
    unsigned cacheLineSize;

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

    bool processingDone;
    int processDelay;
    int clock_period;

    ComputeUnit *cu;

  public:
    typedef CommInterfaceParams Params;
    const Params *
    params() const
    {
      return dynamic_cast<const Params *>(_params);
    }

    CommInterface(Params *p);

    virtual Tick read(PacketPtr pkt);

    virtual Tick write(PacketPtr pkt);

    BaseMasterPort& getMasterPort(const std::string& if_name,
                                  PortID idk = InvalidPortID) override;

    void recvPacket(PacketPtr pkt);

    int prepRead(Addr src, size_t length);
    int prepWrite(Addr dst, uint8_t* value, size_t length);

    uint8_t* getCurData() { return curData; }

    bool isRunning() { return running; }
    bool isCompNeeded() { return computationNeeded; }

    uint64_t getGlobalVar(unsigned index);
    int getProcessDelay() { return processDelay; }

    void registerCompUnit(ComputeUnit *compunit) { cu = compunit; }

  protected:
};

#endif //__HWACC_COMM_INTERFACE_HH__

/*
* MM Register Layout
* | Location of Data 32bits | Compute Finished 1bit | Unused 30bits | Start Operation 1bit |
*/
