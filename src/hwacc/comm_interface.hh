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

    class memRequest {
      public:
        Addr address;
        size_t length;
        memRequest *next;
        uint8_t *data;

        memRequest(Addr add, size_t len) {
            address = add;
            length = len;
            next = NULL;
            data = NULL;
        }
        memRequest(Addr add, uint8_t *dat, size_t len) {
            address = add;
            length = len;
            next = NULL;
            data = dat;
        }
    };

    class requestQueue {
      private:
        memRequest *head;
        memRequest *tail;
      public:
        requestQueue() {
          head = NULL;
          tail = NULL;
        }
        void enqueue(memRequest *req) {
          if (head) {
            tail->next = req;
          } else {
            head = req;
          }
          tail = req;
        }
        memRequest * dequeue() {
          assert(head);
          memRequest *temp = head;
          if(head != tail) {
            head = head->next;
          } else {
            head = NULL;
            tail = NULL;
          }

          return temp;
        }
    };

    requestQueue *readQueue, *writeQueue;
    int readQueueSize, writeQueueSize;

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

    uint8_t *readBuffer;
    uint8_t *writeBuffer;
    bool *readsDone;
    bool running;
    bool reading;
    bool writing;
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

    int prepRead(memRequest *readReq);
    int prepWrite(memRequest *writeReq);
    //int prepRead(Addr src, size_t length);
    //int prepWrite(Addr dst, uint8_t* value, size_t length);

    void enqueueRead(Addr src, size_t length);
    void enqueueWrite(Addr dst, uint8_t* value, size_t length);

    uint8_t* getReadBuffer() { return readBuffer; }

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
