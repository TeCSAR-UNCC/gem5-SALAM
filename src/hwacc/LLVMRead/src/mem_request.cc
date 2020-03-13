//------------------------------------------//
#include "mem_request.hh"
//------------------------------------------//

MemoryRequest::MemoryRequest(Addr add, size_t len) {
    address = add;
    length = len;
    currentReadAddr = address;
    needToRead = true;
    beginAddr = address;
    readLeft = length;
    writeLeft = 0;
    totalLength = length;
    readDone = 0;
    buffer = new uint8_t[length];
    std::memset(buffer, 0, sizeof(length));
    readsDone = new bool[length];
    for (int i = 0; i < length; i++) {
        buffer[i] = 0;
        readsDone[i] = false;
    }

    pkt = NULL;
}
MemoryRequest::MemoryRequest(Addr add, uint8_t *data, size_t len) {
    address = add;
    length = len;
    needToWrite = true;

    currentWriteAddr = address;

    readLeft = 0;
    writeLeft = length;

    totalLength = length;
    writeLeft = totalLength;

    readDone = length;
    writeDone = 0;

    buffer = new uint8_t[length];
    readsDone = new bool[length];
    for (int i = 0; i < length; i++) {
        buffer[i] = *(data + i);
        readsDone[i] = true;
    }

    pkt = NULL;
}
