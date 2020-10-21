#ifndef __HWACC_STREAM_PORT_HH__
#define __HWACC_STREAM_PORT_HH__

#include "mem/port.hh"
#include "mem/tport.hh"

/** Forward declaration **/
class StreamMasterPort;

/**
 * A StreamSlavePort is a specialization of a SimpleTimingPort meant to enable
 * functionality similar to the master port in the AXI-Stream specification.
 * This serves only as a base class.
 */
class StreamSlavePort : public SimpleTimingPort {
	friend class StreamMasterPort;
  private:
  protected:
  	virtual bool tvalid(PacketPtr pkt) = 0;
    virtual bool tvalid(size_t len, bool isRead) = 0;
  	Tick recvAtomic(PacketPtr pkt) override {
  		Tick t = 0;
  		return t;
  	}
  	AddrRangeList getAddrRanges() const override {
  		AddrRangeList range;
  		return range;
  	}
  public:
  	StreamSlavePort(const std::string& name, SimObject* owner) :
  		SimpleTimingPort(name, owner) {}
};

/**
 * Templated StreamSlavePort that functions similarly to the pio port on PioDevices.
 */
template <class Device>
class StreamSlavePortT : public StreamSlavePort
{
  friend class StreamMasterPort;

  private:
    bool isBusy;
    bool retryReq;

    EventFunctionWrapper releaseEvent;

    void
    release() {
        assert(isBusy);
        isBusy = false;
        if (retryReq) {
            retryReq = false;
            sendRetryReq();
        }
    }

  protected:
  Device *device;

  	virtual bool tvalid(PacketPtr pkt) { return device->tvalid(pkt); }
    virtual bool tvalid(size_t len, bool isRead) { return device->tvalid(len, isRead); }

    bool
    recvTimingReq(PacketPtr pkt) override {
        // we should not get a new request after committing to retry the
        // current one, but unfortunately the CPU violates this rule, so
        // simply ignore it for now
        if (retryReq)
          return false;
        // if we are busy with a read or write, remember that we have to
        // retry
        if (isBusy) {
          retryReq = true;
          return false;
        }
        // Make sure that the transfer is valid
        if(!tvalid(pkt))
          return false;
        // the SimpleTimingPort should not be used anywhere where there is
        // a need to deal with snoop responses and their flow control
        // requirements
        if (pkt->cacheResponding())
            panic("SimpleTimingPort should never see packets with the "
                  "cacheResponding flag set\n");

        Tick duration = pkt->getSize() * device->getBandwidth();

        if (duration != 0) {
            device->schedule(releaseEvent, curTick() + duration);
            isBusy = true;
        }

        bool needsResponse = pkt->needsResponse();

        Tick latency = recvAtomic(pkt);

        // turn packet around to go back to requester if response expected
        if (needsResponse) {
            // recvAtomic() should already have turned packet into
            // atomic response
            assert(pkt->isResponse());
            schedTimingResp(pkt, curTick() + latency);
        } else {
            // queue the packet for deletion
            pendingDelete.reset(pkt);
        }
        return true;
    }

    Tick recvAtomic(PacketPtr pkt) override {
      Tick receive_delay = pkt->headerDelay + pkt->payloadDelay;
      pkt->headerDelay = pkt->payloadDelay = 0;
      const Tick delay =
        pkt->isRead() ? device->streamRead(pkt) : device->streamWrite(pkt);
      assert(pkt->isResponse() || pkt->isError());
      return delay + receive_delay;
    }

    AddrRangeList getAddrRanges() const override {
      return device->getStreamAddrRanges();
  	}

  public:
  	StreamSlavePortT(Device *dev) :
      StreamSlavePort(dev->name() + ".stream", dev),
      isBusy(false),
      retryReq(false),
      releaseEvent([this]{ release(); }, dev->name()),
      device(dev) {}
      virtual ~StreamSlavePortT() {}
};

/**
 * A StreamMasterPort is a specialization of a MasterPort, meant to enable
 * functionality similar to the master port in the AXI-Stream specification.
 * A StreamMasterPort is able to check the valid signal on a corresponding
 * StreamSlavePort before initiating a transfer. Otherwise it functions like
 * a standard MasterPort.
 */
class StreamMasterPort : public MasterPort {
  private:
  	StreamSlavePort *_stream_slave;

  protected:
  	//
  public:
  	StreamMasterPort(const std::string& name, SimObject* _owner,
            		 PortID id=InvalidPortID);
  	virtual ~StreamMasterPort();

  	/**
     * Bind this master port to a slave port. This also does the
     * mirror action and binds the slave port to the master port.
     * If the slave port is a stream slave, also binds the tvalid
     * signal.
     */
    void bind(Port &peer) override;

    /**
     * Unbind this master port and the associated slave port.
     */
    void unbind() override;

    /**
	 * If the slave port is a stream slave port, then check if it can
	 * service a request of size 'len'
     */
    bool streamValid(PacketPtr pkt) {
    	if (_stream_slave)
    		return _stream_slave->tvalid(pkt);
    	return true;
    }
    bool streamValid(size_t len, bool isRead) {
      if (_stream_slave)
        return _stream_slave->tvalid(len, isRead);
      return true;
    }
};

#endif //__HWACC_STREAM_PORT_HH__