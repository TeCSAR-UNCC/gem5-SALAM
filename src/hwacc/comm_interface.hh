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

#define NUM_PORTS 4

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

//    class MemoryRequest {
//      friend class CommInterface;
//      private:
//        Addr address;
//        size_t length;
//        bool needToRead;
//        bool needToWrite;
//        Addr currentReadAddr;
//        Addr currentWriteAddr;
//        Addr beginAddr;
//        Tick writeLeft;
//        Tick writeDone;
//        Tick readLeft;
//        Tick readDone;
//        Tick totalLength;

//        uint8_t *buffer;
//        bool *readsDone;

//        PacketPtr pkt;
//      public:
//        MemoryRequest(Addr add, size_t len);
//        MemoryRequest(Addr add, uint8_t *data, size_t len);
//    };

    std::list<MemoryRequest*> *readQueue;
    std::list<MemoryRequest*> *writeQueue;
    std::list<MemoryRequest*> *dramRdQ;
    std::list<MemoryRequest*> *dramWrQ;
    std::list<MemoryRequest*> *spmRdQ;
    std::list<MemoryRequest*> *spmWrQ;

    MemSidePort dramSide;
    MemSidePort spmSide;
    MemSidePort *dramPort;
    MemSidePort *spmPort;

    AddrRange dramRange;

    CommInterface *comm;
    MasterID masterId;
    TickEvent tickEvent;
    unsigned cacheLineSize;

    void tick();

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

    void registerCompUnit(ComputeUnit *compunit) { cu = compunit; }

    void finish();

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
        return NULL;
    }

    void clearMemRequest(MemoryRequest * req, bool isRead);
  protected:
};

#endif //__HWACC_COMM_INTERFACE_HH__

/*
* MM Register Layout
* | Location of Data 32bits | Compute Finished 1bit | Unused 30bits | Start Operation 1bit |
*/
