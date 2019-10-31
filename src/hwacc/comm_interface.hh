#ifndef __HWACC_COMM_INTERFACE_HH__

#define __HWACC_COMM_INTERFACE_HH__

#include "params/CommInterface.hh"
#include "dev/io_device.hh"
#include "dev/arm/base_gic.hh"
#include "hwacc/compute_unit.hh"
#include "hwacc/LLVMRead/src/mem_request.hh"
#include "hwacc/LLVMRead/src/debugFlags.hh"

#include <list>
#include <queue>

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
    int read_bus_width;
    int write_bus_width;

    //class MemoryRequest;

    class MemSidePort : public MasterPort
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
        MemSidePort(const std::string& name, CommInterface *owner) :
          MasterPort(name, owner), owner(owner) {
          readActive = false;
          writeActive = false;
          readReq = NULL;
          writeReq = NULL;
        }

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
        bool active() { return readActive && writeActive; }
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

    std::list<MemoryRequest*> *readQueue;
    std::list<MemoryRequest*> *writeQueue;
    std::list<MemoryRequest*> *dramRdQ;
    std::list<MemoryRequest*> *dramWrQ;
    std::list<MemoryRequest*> *spmRdQ;
    std::list<MemoryRequest*> *spmWrQ;

    int requestsInQueues;

    MemSidePort dramSide;
    MemSidePort spmSide;
    MemSidePort *dramPort;
    MemSidePort *spmPort;

    bool dramPortStalled() { return dramPort->isStalled(); }
    bool spmPortStalled() { return spmPort->isStalled(); }
    void setSpmReadReq(MemoryRequest *req) { spmPort->readReq = req; }
    void setSpmWriteReq(MemoryRequest *req) { spmPort->writeReq = req; }
    void setDramReadReq(MemoryRequest *req) { dramPort->readReq = req; }
    void setDramWriteReq(MemoryRequest *req) { dramPort->writeReq = req; }
    MemoryRequest * getSpmReadReq() { return spmPort->readReq; }
    MemoryRequest * getSpmWriteReq() { return spmPort->writeReq; }
    MemoryRequest * getDramReadReq() { return dramPort->readReq; }
    MemoryRequest * getDramWriteReq() { return dramPort->writeReq; }

    AddrRange localRange;

    CommInterface *comm;
    MasterID masterId;
    TickEvent tickEvent;
    unsigned cacheLineSize;
    int cacheSize;
    int readPorts;
    int writePorts;

    virtual void checkMMR();
    virtual void processMemoryRequests();
    virtual void tick();

    bool running;
    bool computationNeeded;
    bool int_flag;

    void tryRead(MemSidePort * port);
    void tryWrite(MemSidePort * port);

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

    virtual Tick read(PacketPtr pkt);

    virtual Tick write(PacketPtr pkt);

    BaseMasterPort& getMasterPort(const std::string& if_name,
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
    virtual int getCacheSize() { return cacheSize; }
    virtual int getReadPorts() { return readPorts; }
    virtual int getWritePorts() { return writePorts; }
    virtual int getReadBusWidth() { return write_bus_width; }
    virtual int getWriteBusWidth() { return read_bus_width; }

    virtual int getPmemRange() { return 0; }
    void registerCompUnit(ComputeUnit *compunit) { cu = compunit; }
    void registerCycleCounts(CycleCounts *cylcount) { cycleCount = cylcount; }
    virtual void finish();

    MemoryRequest * findMemRequest(PacketPtr pkt, bool isRead) {
        if (isRead) {
            for (auto it=dramRdQ->begin(); it!=dramRdQ->end(); ++it) {
                if ((*it)->pkt == pkt) {
                    return (*it);
                }
            }
            for (auto it=spmRdQ->begin(); it!=dramRdQ->end(); ++it) {
                if ((*it)->pkt == pkt) {
                    return (*it);
                }
            }
        } else {
            for (auto it=dramWrQ->begin(); it!=dramWrQ->end(); ++it) {
                if ((*it)->pkt == pkt) {
                    return (*it);
                }
            }
            for (auto it=spmWrQ->begin(); it!=dramWrQ->end(); ++it) {
                if ((*it)->pkt == pkt) {
                    return (*it);
                }
            }
        }
		panic("Could not find memory request in request queues");
        return NULL;
    }

    void clearMemRequest(MemoryRequest * req, bool isRead);
    virtual void refreshMemPorts() {}
  protected:
};

#include "mem/simple_mem.hh"
#include "params/PrivateMemory.hh"

class PrivateMemory : public SimpleMemory
{
  protected:
    bool readyMode;
    bool resetOnPrivateRead;
    bool *ready;
  public:
    PrivateMemory(const PrivateMemoryParams *p);
    bool isReady(Addr ad, Addr size);
    void privateAccess(PacketPtr pkt);
    void access(PacketPtr pkt) override;
    void setAllReady(bool r);
};

#include "params/CommMemInterface.hh"

class CommMemInterface : public CommInterface
{
  protected:
    PrivateMemory * pmem;
    AddrRange pmemRange;
    bool resetPmemOnFinish;
    int readPorts;
    int writePorts;
    int read_bus_width;
    int write_bus_width;
    int availablePorts;
    int avReadPorts;
    int avWritePorts;
    int cacheSize;
    int privateSize;

  public:
    typedef CommMemInterfaceParams Params;
    const Params *
    params() const
    {
      return dynamic_cast<const Params *>(_params);
    }

    CommMemInterface(Params *p);
    virtual void processMemoryRequests() override;
    virtual void refreshMemPorts() override;
    virtual void finish() override;
    int getCacheSize() override { return cacheSize; }
    int getReadPorts() override { return readPorts; }
    int getWritePorts() override { return writePorts; }
    int getReadBusWidth() override { return write_bus_width; }
    int getWriteBusWidth() override { return read_bus_width; }
    int getPmemRange() override { return privateSize; }

  protected:
    virtual void tick() override;
};

#endif //__HWACC_COMM_INTERFACE_HH__

/*
* MM Register Layout
* | Location of Data 32bits | Compute Finished 1bit | Unused 30bits | Start Operation 1bit |
*/
