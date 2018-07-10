#ifndef __MEMORY_REQUEST_HH__

#define __MEMORY_REQUEST_HH__

#include "mem/packet.hh"
#include "debugFlags.hh"

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
  public:
    MemoryRequest(Addr add, size_t len);
    MemoryRequest(Addr add, uint8_t *data, size_t len);
};

#endif //__MEM_REQUEST_HH__
