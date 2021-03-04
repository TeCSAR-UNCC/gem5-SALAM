#ifndef MEMORY_REQUEST_HH
#define MEMORY_REQUEST_HH
//------------------------------------------//
#include "mem/packet.hh"
#include "mem/port.hh"
#include "debug_flags.hh"
//------------------------------------------//

class MemoryRequest {
  friend class CommInterface;
  friend class LLVMInterface;
  private:
    Addr address;
    size_t length;
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

    uint8_t *buffer;
    bool *readsDone;

    PacketPtr pkt;
    MasterPort * port;
  public:
    MemoryRequest(Addr add, size_t len);
    MemoryRequest(Addr add, uint8_t *data, size_t len);
    ~MemoryRequest() {
        delete readsDone;
        delete buffer;
        if (pkt) delete pkt;
    }
    void setCarrierPort(MasterPort * _port) { port = _port; }
    MasterPort * getCarrierPort() { return port; }
    uint8_t * getBuffer() { return buffer; }
};

#endif //__MEM_REQUEST_HH__
