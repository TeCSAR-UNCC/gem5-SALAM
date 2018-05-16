#include "hwacc/comm_interface.hh"

#include "base/trace.hh"
#include "debug/CommInterface.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"

#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

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
    dramSide(p->name + ".dram_side", this),
    spmSide(p->name + ".spm_side", this),
    dramRange(p->dram_ranges),
    masterId(p->system->getMasterId(name())),
    tickEvent(this),
    cacheLineSize(p->cache_line_size),
    clock_period(p->clock_period) {
    processDelay = 1000 * clock_period;
    FLAG_OFFSET = 0;
    CONFIG_OFFSET = flag_size;
    VAR_OFFSET = CONFIG_OFFSET + config_size;
    processingDone = false;
    computationNeeded = false;
    int_flag = false;
    dramPort = &dramSide;
    spmPort = &spmSide;
    mmreg = new uint8_t[io_size];
    for(int i = 0; i < io_size; i++) {
        mmreg[i] = 0;
    }
    cu = NULL;
    readQueue = new std::list<MemoryRequest*>();
    writeQueue = new std::list<MemoryRequest*>();
    dramRdQ = new std::list<MemoryRequest*>();
    dramWrQ = new std::list<MemoryRequest*>();
    spmRdQ = new std::list<MemoryRequest*>();
    spmWrQ = new std::list<MemoryRequest*>();
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
        MemoryRequest * readReq = findMemRequest(pkt, true);
        DPRINTF(CommInterface, "Done with a read. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
        pkt->writeData(readReq->buffer + (pkt->req->getPaddr() - readReq->beginAddr));
        DPRINTF(CommInterface, "Read:0x%016lx\n", *(uint64_t *)readReq->buffer);
        for (int i = pkt->req->getPaddr() - readReq->beginAddr;
             i < pkt->req->getPaddr() - readReq->beginAddr + pkt->getSize(); i++)
        {
            readReq->readsDone[i] = true;
        }

        // mark readDone as only the contiguous region
        while (readReq->readDone < readReq->totalLength && readReq->readsDone[readReq->readDone])
        {
            readReq->readDone++;
        }

        if (!readReq->needToRead)
        {
            DPRINTF(CommInterface, "Done reading\n");
            cu->readCommit(readReq);
            clearMemRequest(readReq, true);
            delete readReq;
        }
    } else if (pkt->isWrite()) {
        MemoryRequest * writeReq = findMemRequest(pkt, false);
        DPRINTF(CommInterface, "Done with a write. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
        writeReq->writeDone += pkt->getSize();
        if (!(writeReq->needToWrite)) {
            DPRINTF(CommInterface, "Done writing\n");
            cu->writeCommit(writeReq);
            delete[] writeReq->buffer;
            delete[] writeReq->readsDone;
            clearMemRequest(writeReq, false);
            delete writeReq;
        }
    } else {
        panic("Something went very wrong!");
    }
    if (!tickEvent.scheduled())
    {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
    if (pkt->req) delete pkt->req;
    delete pkt;
}

void
CommInterface::tick() {
    DPRINTF(CommInterface, "Tick!\n");
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
    if (!dramPort->isStalled() || !spmPort->isStalled()) {
        DPRINTF(CommInterface, "Checking read requests\n");
        for (auto it=readQueue->begin(); it!=readQueue->end(); ) {
            DPRINTF(CommInterface, "Request Address: %lx ", (*it)->address);
            if (dramRange.contains((*it)->address)) {
                DPRINTF(CommInterface, "In DRAM\n");
                if (dramPort->isStalled()) {
                    DPRINTF(CommInterface, "System Memory Port Stalled\n");
                    ++it;
                } else {
                    if (!dramPort->readReq) {
                        dramPort->readReq = (*it);
                        it = readQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (dramPort->readReq && dramPort->readReq->needToRead) {
                        DPRINTF(CommInterface, "Trying read on System Memory Port\n");
                        tryRead(dramPort);
                        dramRdQ->push_back(dramPort->readReq);
                        if (!dramPort->readReq->needToRead)
                            dramPort->readReq = NULL;
                    }
                }
            } else {
                DPRINTF(CommInterface, "In SPM\n");
                if (spmPort->isStalled()) {
                    DPRINTF(CommInterface, "SPM Port Stalled\n");
                    ++it;
                } else {
                    if (!spmPort->readReq) {
                        spmPort->readReq = (*it);
                        it = readQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (spmPort->readReq && spmPort->readReq->needToRead) {
                        DPRINTF(CommInterface, "Trying read on SPM Port\n");
                        tryRead(spmPort);
                        spmRdQ->push_back(spmPort->readReq);
                        if (!spmPort->readReq->needToRead)
                            spmPort->readReq = NULL;
                    }
                }
            }
        }
        DPRINTF(CommInterface, "Checking write requests\n");
        for (auto it=writeQueue->begin(); it!=writeQueue->end(); ) {
            DPRINTF(CommInterface, "Request Address: %lx ", (*it)->address);
            if (dramRange.contains((*it)->address)) {
                DPRINTF(CommInterface, "In DRAM\n");
                if (dramPort->isStalled()) {
                    DPRINTF(CommInterface, "System Memory Port Stalled\n");
                    ++it;
                } else {
                    if (!dramPort->writeReq) {
                        dramPort->writeReq = (*it);
                        it = writeQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (dramPort->writeReq && dramPort->writeReq->needToWrite) {
                        DPRINTF(CommInterface, "Trying write on System Memory Port\n");
                        tryWrite(dramPort);
                        dramWrQ->push_back(dramPort->writeReq);
                        if (!dramPort->writeReq->needToWrite)
                            dramPort->writeReq = NULL;
                    }
                }
            } else {
                DPRINTF(CommInterface, "In SPM\n");
                if (spmPort->isStalled()) {
                    DPRINTF(CommInterface, "SPM Port Stalled\n");
                    ++it;
                } else {
                    if (!spmPort->writeReq) {
                        spmPort->writeReq = (*it);
                        it = writeQueue->erase(it);;
                    } else {
                        ++it;
                    }
                    if (spmPort->writeReq && spmPort->writeReq->needToWrite) {
                        DPRINTF(CommInterface, "Trying write on SPM Port\n");
                        tryWrite(spmPort);
                        spmWrQ->push_back(spmPort->writeReq);
                        if (!spmPort->writeReq->needToWrite)
                            spmPort->writeReq = NULL;
                    }
                }
            }
        }
    }
}

void
CommInterface::tryRead(MemSidePort * port) {
    //RequestPtr req = new Request();
    MemoryRequest * readReq = port->readReq;
    Request::Flags flags;
    if (readReq->readLeft <= 0) {
        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to read if there aren't reads left\n");
        return;
    }
    int size;
    if (readReq->currentReadAddr % cacheLineSize) {
        size = cacheLineSize - (readReq->currentReadAddr % cacheLineSize);
        DPRINTF(CommInterface, "Aligning\n");
    } else {
        size = cacheLineSize;
    }
    size = readReq->readLeft > (size - 1) ? size : readReq->readLeft;
    RequestPtr req = new Request(readReq->currentReadAddr, size, flags, masterId);
    DPRINTF(CommInterface, "Trying to read addr: 0x%x, %d bytes\n",
        req->getPaddr(), size);

    PacketPtr pkt = new Packet(req, MemCmd::ReadReq);
    pkt->allocate();
    readReq->pkt = pkt;
    port->sendPacket(pkt);

    readReq->currentReadAddr += size;

    readReq->readLeft -= size;

    if (!(readReq->readLeft > 0)) {
        readReq->needToRead = false;
        if (!tickEvent.scheduled()) {
            //schedule(tickEvent, curTick() + processDelay);
            schedule(tickEvent, nextCycle());
        }
    } else {
        if (!port->isStalled() && !tickEvent.scheduled())
        {
            //schedule(tickEvent, curTick() + processDelay);
            schedule(tickEvent, nextCycle());
        }
    }
}

void
CommInterface::tryWrite(MemSidePort * port) {
    MemoryRequest * writeReq = port->writeReq;
    if (writeReq->writeLeft <= 0) {
        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to write if there aren't writes left\n");
        return;
    }

    int size;
    if (writeReq->currentWriteAddr % cacheLineSize) {
        size = cacheLineSize - (writeReq->currentWriteAddr % cacheLineSize);
        DPRINTF(CommInterface, "Aligning\n");
    } else {
        size = cacheLineSize;
    }
    size = writeReq->writeLeft > size - 1 ? size : writeReq->writeLeft;

    Request::Flags flags;
    uint8_t *data = new uint8_t[size];
    std::memcpy(data, &(writeReq->buffer[writeReq->totalLength-writeReq->writeLeft]), size);
    RequestPtr req = new Request(writeReq->currentWriteAddr, size, flags, masterId);
    req->setExtraData((uint64_t)data);


    DPRINTF(CommInterface, "totalLength: %d, writeLeft: %d\n", writeReq->totalLength, writeReq->writeLeft);
    DPRINTF(CommInterface, "Trying to write to addr: 0x%x, %d bytes, data 0x%x\n",
        writeReq->currentWriteAddr, size, *((uint64_t*)(&(writeReq->buffer[writeReq->totalLength-writeReq->writeLeft]))));

    PacketPtr pkt = new Packet(req, MemCmd::WriteReq);
    uint8_t *pkt_data = (uint8_t *)req->getExtraData();
    pkt->dataDynamic(pkt_data);
    writeReq->pkt = pkt;
    port->sendPacket(pkt);

    writeReq->currentWriteAddr += size;
    writeReq->writeLeft -= size;

    if (!(writeReq->writeLeft > 0)) {
        writeReq->needToWrite = false;
        if (!tickEvent.scheduled()) {
            //schedule(tickEvent, curTick() + processDelay);
            schedule(tickEvent, nextCycle());
        }
    } else if (!port->isStalled() && !tickEvent.scheduled()) {
            //schedule(tickEvent, curTick() + processDelay);
            schedule(tickEvent, nextCycle());
    }
}

void
CommInterface::enqueueRead(MemoryRequest * req) {
    DPRINTF(CommInterface, "Read from 0x%lx of size:%d bytes enqueued\n", req->address, req->length);
    readQueue->push_back(req);
    DPRINTF(CommInterface, "Current Queue:\n");
    for (auto it=readQueue->begin(); it!=readQueue->end(); ++it) {
        DPRINTF(CommInterface, "Read Request: %lx\n", (*it)->address);
    }
    if (!tickEvent.scheduled()) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
}

void
CommInterface::enqueueWrite(MemoryRequest * req) {
    DPRINTF(CommInterface, "Write to 0x%lx of size:%d bytes enqueued\n", req->address, req->length);
    writeQueue->push_back(req);
    DPRINTF(CommInterface, "Current Queue:\n");
    for (auto it=writeQueue->begin(); it!=writeQueue->end(); ++it) {
        DPRINTF(CommInterface, "Write Request: %lx\n", (*it)->address);
    }
    if (!tickEvent.scheduled()) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
}

void
CommInterface::finish() {
    *mmreg &= 0xfc;
    *mmreg |= 0x04;
    int_flag = true;
    computationNeeded = false;
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

    //DPRINTF(CommInterface, "MMReg value: 0x%016x\n", *(uint64_t *)mmreg);
    std::stringstream mm;
    for (int i = io_size-1; i >= 0; i--) {
        if ((i >= flag_size+config_size) && ((i-flag_size-config_size)%8 == 0))
            mm << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)mmreg[i] << "|";
        else if (i == flag_size+config_size)
            mm << "|" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)mmreg[i];
        else if (i == flag_size)
            mm << "|" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)mmreg[i];
        else
            mm << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)mmreg[i];
    }
    std::string mmr = mm.str();
    DPRINTF(CommInterface, "MMReg value: %s\n", mmr);

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
    if (if_name == "dram_side") {
        return dramSide;
    } else if (if_name == "spm_side") {
        return spmSide;
    } else {
        return MemObject::getMasterPort(if_name, idx);
    }
}

//CommInterface::MemoryRequest::MemoryRequest(Addr add, size_t len) {
//    address = add;
//    length = len;
//    currentReadAddr = address;
//    needToRead = true;
//    beginAddr = address;
//    readLeft = length;
//    writeLeft = 0;
//    totalLength = length;
//    readDone = 0;
//    buffer = new uint8_t[length];
//    std::memset(buffer, 0, sizeof(length));
//    readsDone = new bool[length];
//    for (int i = 0; i < length; i++) {
//        buffer[i] = 0;
//        readsDone[i] = false;
//    }

//    pkt = NULL;
//}
//CommInterface::MemoryRequest::MemoryRequest(Addr add, uint8_t *data, size_t len) {
//    address = add;
//    length = len;
//    needToWrite = true;

//    currentWriteAddr = address;

//    readLeft = 0;
//    writeLeft = length;

//    totalLength = length;
//    writeLeft = totalLength;

//    readDone = length;
//    writeDone = 0;

//    buffer = new uint8_t[length];
//    readsDone = new bool[length];
//    for (int i = 0; i < length; i++) {
//        buffer[i] = *(data + i);
//        readsDone[i] = true;
//    }

//    pkt = NULL;
//}

void
CommInterface::clearMemRequest(MemoryRequest * req, bool isRead) {
    if (isRead) {
        if (dramRange.contains(req->address)) {
            for (auto it=dramRdQ->begin(); it!=dramRdQ->end(); ++it) {
                if ((*it)==req) {
                    it=dramRdQ->erase(it);
                    break;
                }
            }
        } else {
            for (auto it=spmRdQ->begin(); it!=dramRdQ->end(); ++it) {
                if ((*it) == req) {
                    it=spmRdQ->erase(it);
                    break;
                }
            }
        }
    } else {
        if (dramRange.contains(req->address)) {
            for (auto it=dramWrQ->begin(); it!=dramWrQ->end(); ++it) {
                if ((*it) == req) {
                    it=dramWrQ->erase(it);
                    break;
                }
            }
        } else {
            for (auto it=spmWrQ->begin(); it!=dramWrQ->end(); ++it) {
                if ((*it) == req) {
                    it=spmWrQ->erase(it);
                    break;
                }
            }
        }
    }
}
