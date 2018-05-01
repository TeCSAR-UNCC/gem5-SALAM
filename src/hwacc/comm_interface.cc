#include "hwacc/comm_interface.hh"

#include "base/trace.hh"
#include "debug/CommInterface.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"

#include <stdio.h>
#include <stdlib.h>


using namespace std;


CommInterface::CommInterface(Params *p) :
    BasicPioDevice(p, p->pio_size),
    io_addr(p->pio_addr),
    io_size(p->pio_size),
    flag_size(p->flags_size),
    config_size(p->config_size),
    devname(p->devicename),
    gic(p->gic),
    int_num(p->int_num),
    memPort(p->name + ".mem_side", this),
    masterId(p->system->getMasterId(name())),
    tickEvent(this),
    cacheLineSize(p->cache_line_size),
    clock_period(p->clock_period) {

    processDelay = 1000 * clock_period;
    FLAG_OFFSET = 0;
    CONFIG_OFFSET = flag_size;
    VAR_OFFSET = CONFIG_OFFSET + config_size;
    needToRead = false;
    needToWrite = false;
    running = false;
    reading = false;
    writing = false;
    processingDone = false;
    computationNeeded = false;
    int_flag = false;
    dataPort = &memPort;
    mmreg = new uint8_t[io_size];
    for(int i = 0; i < io_size; i++) {
        mmreg[i] = 0;
    }
    cu = NULL;
    //readQueue = new requestQueue();
    //writeQueue = new requestQueue();
    readQueue = new std::queue<memRequest*>();
    writeQueue = new std::queue<memRequest*>();
}

bool
CommInterface::MemSidePort::recvTimingResp(PacketPtr pkt) {
    owner->recvPacket(pkt);
    return true;
}

void
CommInterface::MemSidePort::recvReqRetry() {
    assert(outstandingPkts.size());

    DPRINTF(CommInterface, "Got a retry...\n");
    while (outstandingPkts.size() && sendTimingReq(outstandingPkts.front())) {
        DPRINTF(CommInterface, "Unblocked, sent blocked packet.\n");
        outstandingPkts.pop();
        // TODO: This should just signal the engine that the packet completed
        // engine should schedule tick as necessary. Need a test case
        if (!owner->tickEvent.scheduled()) {
            //owner->schedule(owner->tickEvent, curTick() + owner->processDelay);
            owner->schedule(owner->tickEvent, owner->nextCycle());
        }
    }
}

void
CommInterface::MemSidePort::sendPacket(PacketPtr pkt) {
    if (isStalled() || !sendTimingReq(pkt)) {
        DPRINTF(CommInterface, "sendTiming failed in sendPacket(pkt->req->getPaddr()=0x%x)\n", (unsigned int)pkt->req->getPaddr());
        setStalled(pkt);
    }
}

void
CommInterface::recvPacket(PacketPtr pkt) {
    if (pkt->isRead()) {
        DPRINTF(CommInterface, "Done with a read. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
        pkt->writeData(readBuffer + (pkt->req->getPaddr() - beginAddr));

        for (int i = pkt->req->getPaddr() - beginAddr;
             i < pkt->req->getPaddr() - beginAddr + pkt->getSize(); i++)
        {
            readsDone[i] = true;
        }

        // mark readDone as only the contiguous region
        while (readDone < totalLength && readsDone[readDone])
        {
            readDone++;
        }

        if (readDone >= totalLength)
        {
            DPRINTF(CommInterface, "Done reading\n");
            cu->readCommit(readBuffer);
            needToRead = false;
            reading = false;
        }
    } else if (pkt->isWrite()) {
        DPRINTF(CommInterface, "Done with a write. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
        writeDone += pkt->getSize();
        if (!(writeDone < totalLength)) {
            DPRINTF(CommInterface, "Done writing\n");
            //gic->sendInt(int_num);
            //*(uint32_t *)mmreg |= 0x80000000;
            //DPRINTF(CommInterface, "MMReg value: 0x%016x\n", *(uint64_t *)mmreg);
            cu->writeCommit();
            needToWrite = false;
            delete[] writeBuffer;
            delete[] readsDone;
            writing = false;
        } else {
            if (!tickEvent.scheduled())
            {
                //schedule(tickEvent, curTick() + processDelay);
                schedule(tickEvent, nextCycle());
            }
        }
    } else {
        panic("Something went very wrong!");
    }
    if(!reading && !readQueue->empty()) {
        prepRead(readQueue->front());
        readQueue->pop();
    }
    if(!writing && !writeQueue->empty()) {
        prepWrite(writeQueue->front());
        writeQueue->pop();
    }
    if (pkt->req) delete pkt->req;
    delete pkt;
}

void
CommInterface::tick() {
    running = writing || reading;
    if (!computationNeeded) {
        DPRINTF(CommInterface, "Checking MMR to see if Run bit set\n");
        if (*mmreg & 0x01) {
            *mmreg &= 0xfe;
            *mmreg |= 0x02;
            computationNeeded = true;
            cu->initialize();
        }

        if (processingDone && !tickEvent.scheduled()) {
            processingDone = false;
            //schedule(tickEvent, curTick() + processDelay);
            schedule(tickEvent, nextCycle());
        }

        return;
    }
    if (dataPort->isStalled()) {
        DPRINTF(CommInterface, "Stalled\n");
    } else {
        if(!reading && !readQueue->empty()) {
            prepRead(readQueue->front());
            readQueue->pop();
        }
        if(!writing && !writeQueue->empty()) {
            prepWrite(writeQueue->front());
            writeQueue->pop();
        }
        if (needToRead && !dataPort->isStalled()) {
            DPRINTF(CommInterface, "trying read\n");
            tryRead();
        }

        if (needToWrite && !dataPort->isStalled() &&
            ((totalLength - writeLeft) < readDone)) {
            DPRINTF(CommInterface, "trying write\n");
            tryWrite();
        }
    }
}

void
CommInterface::tryRead() {
    //RequestPtr req = new Request();
    Request::Flags flags;

    if (readLeft <= 0) {
        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to read if there aren't reads left\n");
        return;
    }

    int size;
    if (currentReadAddr % cacheLineSize) {
        size = cacheLineSize - (currentReadAddr % cacheLineSize);
        DPRINTF(CommInterface, "Aligning\n");
    } else {
        size = cacheLineSize;
    }
    size = readLeft > (size - 1) ? size : readLeft;
    RequestPtr req = new Request(currentReadAddr, size, flags, masterId);

    DPRINTF(CommInterface, "Trying to read addr: 0x%x, %d bytes\n",
        req->getPaddr(), size);

    PacketPtr pkt = new Packet(req, MemCmd::ReadReq);
    pkt->allocate();
    dataPort->sendPacket(pkt);

    currentReadAddr += size;

    readLeft -= size;

    if (!(readLeft > 0)) {
        needToRead = false;
        if (!tickEvent.scheduled()) {
            //schedule(tickEvent, curTick() + processDelay);
            schedule(tickEvent, nextCycle());
        }
    } else {
        if (!dataPort->isStalled() && !tickEvent.scheduled())
        {
            //schedule(tickEvent, curTick() + processDelay);
            schedule(tickEvent, nextCycle());
        }
    }
}

void
CommInterface::tryWrite() {
    if (writeLeft <= 0) {
        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to write if there aren't writes left\n");
        return;
    }

    int size;
    if (currentWriteAddr % cacheLineSize) {
        size = cacheLineSize - (currentWriteAddr % cacheLineSize);
        DPRINTF(CommInterface, "Aligning\n");
    } else {
        size = cacheLineSize;
    }
    size = writeLeft > size - 1 ? size : writeLeft;

    Request::Flags flags;
    uint8_t *data = new uint8_t[size];
    std::memcpy(data, &writeBuffer[totalLength-writeLeft], size);
    RequestPtr req = new Request(currentWriteAddr, size, flags, masterId);
    req->setExtraData((uint64_t)data);


    DPRINTF(CommInterface, "totalLength: %d, writeLeft: %d\n", totalLength, writeLeft);
    DPRINTF(CommInterface, "Trying to write to addr: 0x%x, %d bytes, data 0x%x\n",
        currentWriteAddr, size, *((uint64_t*)(&writeBuffer[totalLength-writeLeft])));

    PacketPtr pkt = new Packet(req, MemCmd::WriteReq);
    uint8_t *pkt_data = (uint8_t *)req->getExtraData();
    pkt->dataDynamic(pkt_data);
    dataPort->sendPacket(pkt);

    currentWriteAddr += size;
    writeLeft -= size;

    if (!(writeLeft > 0) && !tickEvent.scheduled()) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
}

int
CommInterface::prepRead(memRequest *readReq) {
    Addr src = readReq->address;
    size_t length = readReq->length;
    assert(length > 0);
    assert(!reading);
    reading = true;
    //gic->clearInt(int_num);

    DPRINTF(CommInterface, "Initiating read of %d bytes from 0x%x\n", length, src);

    needToRead = true;
    needToWrite = false;

    currentReadAddr = src;

    beginAddr = src;

    readLeft = length;
    writeLeft = 0;

    totalLength = length;

    readDone = 0;

    readBuffer = new uint8_t[length];
    std::memset(readBuffer, 0, sizeof(length));
    readsDone = new bool[length];
    for (int i = 0; i < length; i++) {
        readBuffer[i] = 0;
        readsDone[i] = false;
    }

    if (!tickEvent.scheduled()) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }

    delete readReq;
    return 0;
}

int
CommInterface::prepWrite(memRequest *writeReq) {
    assert(!writing);
    Addr dst = writeReq->address;
    uint8_t *value = writeReq->data;
    size_t length = writeReq->length;
    assert(length > 0);
    writing = true;
    //gic->clearInt(int_num);
    //*(uint32_t *)mmreg &= 0xefffffff;

    DPRINTF(CommInterface, "Initiating write of %d bytes at 0x%x to 0x%x\n",
        length, dst, *(uint64_t *)value);

    needToRead = false;
    needToWrite = true;

    currentWriteAddr = dst;

    readLeft = 0;
    writeLeft = length;

    totalLength = length;
    writeLeft = totalLength;

    readDone = length;
    writeDone = 0;

    writeBuffer = new uint8_t[length];
    readsDone = new bool[length];
    for (int i = 0; i < length; i++) {
        writeBuffer[i] = *(value + i);
        readsDone[i] = true;
    }

    if (!tickEvent.scheduled()) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }

    delete writeReq;
    return 0;
}

void
CommInterface::enqueueRead(Addr src, size_t length) {
    DPRINTF(CommInterface, "Read from 0x%lx of size:%d bytes enqueued\n", src, length);
    readQueue->push(new memRequest(src, length));
    if (!tickEvent.scheduled()) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
}

void
CommInterface::enqueueWrite(Addr dst, uint8_t* value, size_t length) {
    DPRINTF(CommInterface, "Write to 0x%lx of size:%d bytes enqueued\n", dst, length);
    writeQueue->push(new memRequest(dst, value, length));
    if (!tickEvent.scheduled()) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
}

void
CommInterface::finish() {
    *mmreg &= 0xfd;
    *mmreg |= 0x04;
    int_flag = true;
    computationNeeded = false;
    DPRINTF(CommInterface, "Computation Finished! Raising Interrupt!\n");
    gic->sendInt(int_num);
}

Tick
CommInterface::read(PacketPtr pkt) {
    DPRINTF(CommInterface, "The address range associated with this ACC was read!\n");

    Addr offset = pkt->req->getPaddr() - io_addr;

    uint32_t data;

    data = *(uint32_t *)(mmreg+offset);

    switch(pkt->getSize()) {
      case 1:
        pkt->set<uint8_t>(data);
        break;
      case 2:
        pkt->set<uint16_t>(data);
        break;
      case 4:
        pkt->set<uint32_t>(data);
        break;
      default:
        panic("Read size too big?\n");
        break;
    }

    pkt->makeAtomicResponse();
    return pioDelay;
}

Tick
CommInterface::write(PacketPtr pkt) {
    DPRINTF(CommInterface,
        "The address range associated with this ACC was written to!\n");

    pkt->writeData(mmreg + (pkt->req->getPaddr() - io_addr));

    DPRINTF(CommInterface, "MMReg value: 0x%016x\n", *(uint64_t *)mmreg);

    pkt->makeAtomicResponse();

    if (((*mmreg & 0x04) == 0x00) && int_flag) {
        gic->clearInt(int_num);
        int_flag = false;
    }
    if (!tickEvent.scheduled()) {
        schedule(tickEvent, nextCycle());
    }

    return pioDelay;
}

uint64_t
CommInterface::getGlobalVar(unsigned index) {
    return *(uint64_t *)(mmreg + VAR_OFFSET + index*8);
}

CommInterface *
CommInterfaceParams::create() {
    return new CommInterface(this);
}

BaseMasterPort&
CommInterface::getMasterPort(const std::string& if_name, PortID idx) {
    if (if_name == "mem_side") {
        return memPort;
    } else {
        return MemObject::getMasterPort(if_name, idx);
    }
}
