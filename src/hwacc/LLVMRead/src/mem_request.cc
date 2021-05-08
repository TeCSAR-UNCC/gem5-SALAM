//------------------------------------------//
#include "mem_request.hh"
//------------------------------------------//

MemoryRequest::MemoryRequest(Addr add, size_t len) {
    address = add;
    length = len;

    needToRead = true;
    needToWrite = false;

    currentReadAddr = address;

    beginAddr = address;
    readLeft = length;
    writeLeft = 0;
    totalLength = length;
    readDone = 0;

    buffer = new uint8_t[length];
    readsDone = new bool[length];
    std::memset(buffer, 0, length);

    for (int i = 0; i < length; i++) {
        readsDone[i] = false;
    }
    pkt = NULL;
}


MemoryRequest::MemoryRequest(Addr add, const void *data, size_t len) {
    address = add;
    length = len;

    needToWrite = true;
    needToRead = false;

    currentWriteAddr = address;

    readLeft = 0;
    writeLeft = length;

    totalLength = length;
    writeLeft = totalLength;

    readDone = length;
    writeDone = 0;

    buffer = new uint8_t[length];
    readsDone = new bool[length];
    std::memcpy(buffer, data, length);
    // for (int i = 0; i < length; i++) {
    //     buffer[i] = *(data + i);
    //     readsDone[i] = true;
    // }
    pkt = NULL;
}
