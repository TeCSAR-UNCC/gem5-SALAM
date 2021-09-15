#ifndef MEMORY_REQUEST_HH
#define MEMORY_REQUEST_HH
//------------------------------------------//
#include "mem/packet.hh"
#include "mem/port.hh"
#include "debug_flags.hh"
//------------------------------------------//

using namespace gem5;

class MemoryRequest {
  friend class CommInterface;
  // friend class LLVMInterface;
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
    RequestPort * port;
  public:
    MemoryRequest(Addr add, size_t len);
    MemoryRequest(Addr add, const void *data, size_t len);
    ~MemoryRequest() {
        delete[] readsDone;
        delete[] buffer;
        // if (pkt) delete pkt;
    }
    void setCarrierPort(RequestPort * _port) { port = _port; }
    RequestPort * getCarrierPort() { return port; }
    uint8_t * getBuffer() { return buffer; }
    Addr getAddress() { return address; }
    std::string printBuffer();
};

#endif //__MEM_REQUEST_HH__
