#ifndef __HWACC_COMM_INTERFACE_HH__
#define __HWACC_COMM_INTERFACE_HH__

#include "params/CommInterface.hh"
#include "dev/io_device.hh"
#include "dev/arm/base_gic.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/LLVMRead/src/mem_request.hh"
#include "hwacc/stream_port.hh"
#include "hwacc/scratchpad_memory.hh"
#include "hwacc/LLVMRead/src/debug_flags.hh"

#include <list>
#include <queue>
#include <vector>

class CommInterface : public BasicPioDevice
{
  protected:
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
    bool use_premap_data;
    std::vector<Addr> data_base_ptrs;
    ByteOrder endian;

    // const std::string name() const { return devname; }

    class MemSidePort : public StreamMasterPort
    {
      friend class CommInterface;

      private:
        CommInterface *owner;
        std::queue<PacketPtr> outstandingPkts;
        MemoryRequest *readReq;
        MemoryRequest *writeReq;
        bool readActive;
        bool writeActive;

      public:
        MemSidePort(const std::string& name, CommInterface *owner, PortID id=InvalidPortID) :
          StreamMasterPort(name, owner, id), owner(owner) {
          readActive = false;
          writeActive = false;
          readReq = NULL;
          writeReq = NULL;
        }
        MemoryRequest * getReadReq() { return readReq; }
        MemoryRequest * getWriteReq() { return writeReq; }
        void setReadReq(MemoryRequest * req = nullptr) { readReq = req; }
        void setWriteReq(MemoryRequest * req = nullptr) { writeReq = req; }

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
        bool active() { return readActive || writeActive; }
        bool reading() { return readActive; }
        bool writing() { return writeActive; }
    };

    class SPMPort : public ScratchpadMasterPort
    {
      friend class CommInterface;

      private:
        CommInterface *owner;
        std::queue<PacketPtr> outstandingPkts;
        MemoryRequest *readReq;
        MemoryRequest *writeReq;
        bool readActive;
        bool writeActive;

      public:
        SPMPort(const std::string& name, CommInterface *owner, PortID id=InvalidPortID) :
          ScratchpadMasterPort(name, owner, id), owner(owner) {
          readActive = false;
          writeActive = false;
          readReq = NULL;
          writeReq = NULL;
        }
        MemoryRequest * getReadReq() { return readReq; }
        MemoryRequest * getWriteReq() { return writeReq; }
        void setReadReq(MemoryRequest * req = nullptr) { readReq = req; }
        void setWriteReq(MemoryRequest * req = nullptr) { writeReq = req; }

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
        bool active() { return readActive || writeActive; }
        bool reading() { return readActive; }
        bool writing() { return writeActive; }
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

    std::list<MemoryRequest*> readQueue;
    std::list<MemoryRequest*> writeQueue;
    std::list<MemoryRequest*> accRdQ;
    std::list<MemoryRequest*> accWrQ;

    int requestsInQueues;

    std::vector<MemSidePort*> localPorts;
    std::vector<MemSidePort*> globalPorts;
    std::vector<MemSidePort*> streamPorts;
    std::vector<SPMPort*> spmPorts;

    bool localPortsStalled() {
        for (auto it=localPorts.begin(); it!=localPorts.end(); ++it) {
            if (!((*it)->isStalled())) return false;
        }
        return true;
    }
    bool globalPortsStalled() {
        for (auto it=globalPorts.begin(); it!=globalPorts.end(); ++it) {
            if (!((*it)->isStalled())) return false;
        }
        return true;
    }
    bool streamPortsStalled() {
        for (auto it=streamPorts.begin(); it!=streamPorts.end(); ++it) {
            if (!((*it)->isStalled())) return false;
        }
        return true;
    }
    bool spmPortsStalled() {
        for (auto port : spmPorts) {
            if (!(port->isStalled())) return false;
        }
        return true;
    }
    bool allPortsStalled() {
        return localPortsStalled() && globalPortsStalled() && streamPortsStalled() && spmPortsStalled();
    }
    bool inStreamRange(Addr add);
    bool inSPMRange(Addr add);
    bool inLocalRange(Addr add);
    MemSidePort * getValidLocalPort(Addr add, bool read);
    MemSidePort * getValidGlobalPort(Addr add, bool read);
    MemSidePort * getValidStreamPort(Addr add, size_t len, bool read);
    SPMPort * getValidSPMPort(Addr add, size_t len, bool read);

    CommInterface *comm;
    MasterID masterId;
    TickEvent tickEvent;
    unsigned cacheLineSize;

    virtual void checkMMR();
    virtual void processMemoryRequests();
    virtual void tick();

    bool running;
    bool computationNeeded;
    bool int_flag;

    void tryRead(MemSidePort * port);
    void tryWrite(MemSidePort * port);

    void tryRead(SPMPort * port);
    void tryWrite(SPMPort * port);

    Addr dataAddr;

    uint8_t *mmreg;

    bool processingDone;
    int processDelay;
    int clock_period;

    ComputeUnit *cu;
    CycleCounts *cycleCount;

  public:
    typedef CommInterfaceParams Params;
    const Params *
    params() const
    {
      return dynamic_cast<const Params *>(_params);
    }

    CommInterface(Params *p);

    void startup();

    virtual Tick read(PacketPtr pkt);

    virtual Tick write(PacketPtr pkt);

    Port& getPort(const std::string& if_name,
                                  PortID idk = InvalidPortID) override;

    void recvPacket(PacketPtr pkt);

    // Addr value stored in gep register, length based on data type
    void enqueueRead(MemoryRequest * req); 

    void enqueueWrite(MemoryRequest * req);

    //uint8_t* getReadBuffer() { return readBuffer; }

    bool isRunning() { return running; }
    bool isCompNeeded() { return computationNeeded; }

    uint64_t getGlobalVar(unsigned index);
    int getProcessDelay() { return processDelay; }
    virtual int getReadPorts()  { return 0; }
    virtual int getWritePorts()  { return 0; }
    virtual int getReadBusWidth()  { return 0; }
    virtual int getWriteBusWidth()  { return 0; }  
    virtual int getPmemRange() { return 0; }
    void registerCompUnit(ComputeUnit *compunit) { cu = compunit; }
    void registerCycleCounts(CycleCounts *cylcount) { cycleCount = cylcount; }
    virtual void finish();

    MemoryRequest * findMemRequest(PacketPtr pkt, bool isRead);
    void clearMemRequest(MemoryRequest * req, bool isRead);
    virtual void refreshMemPorts() {}
    std::string getName() const { return name(); }

    virtual bool isBaseCommInterface() { return true; }
  protected:
};

#endif //__HWACC_COMM_INTERFACE_HH__

/*
* MM Register Layout
* | Location of Data 32bits | Compute Finished 1bit | Unused 30bits | Start Operation 1bit |
*/
