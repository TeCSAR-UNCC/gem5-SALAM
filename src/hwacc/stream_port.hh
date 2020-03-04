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
class StreamSlavePortT : public StreamSlavePort {
	friend class StreamMasterPort;

  private:
  protected:
  	Device *device;

  	virtual bool tvalid(PacketPtr pkt) { return device->tvalid(pkt); }
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
  		StreamSlavePort(dev->name() + ".stream", dev), device(dev) {}
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
    inline bool streamValid(PacketPtr pkt) {
    	if (_stream_slave)
    		return _stream_slave->tvalid(pkt);
    	return false;
    }
};

#endif //__HWACC_STREAM_PORT_HH__