#include "hwacc/comm_interface.hh"
#include "base/trace.hh"
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
    localRange(p->local_range),
    masterId(p->system->getMasterId(this,name())),
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
    //if (pkt->req) delete pkt->req;
    delete pkt;
}

void
CommInterface::checkMMR() {
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
    }
}

void
CommInterface::processMemoryRequests() {
    if (!dramPort->isStalled() || !spmPort->isStalled()) {
        DPRINTF(CommInterface, "Checking read requests. %d requests in queue.\n", readQueue->size());
        for (auto it=readQueue->begin(); it!=readQueue->end(); ) {
            DPRINTF(CommInterfaceQueues, "Request Address: %lx\n", (*it)->address);
            if (!localRange.contains((*it)->address)) {
                DPRINTF(CommInterfaceQueues, "In DRAM\n");
                if (dramPort->isStalled()) {
                    DPRINTF(CommInterfaceQueues, "System Memory Port Stalled\n");
                    ++it;
                } else {
                    if (!dramPort->readReq) {
                        dramPort->readReq = (*it);
                        it = readQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (dramPort->readReq && dramPort->readReq->needToRead) {
                        DPRINTF(CommInterfaceQueues, "Trying read on System Memory Port\n");
                        tryRead(dramPort);
                        dramRdQ->push_back(dramPort->readReq);
                        if (!dramPort->readReq->needToRead)
                            dramPort->readReq = NULL;
                    }
                }
            } else {
                DPRINTF(CommInterfaceQueues, "In SPM\n");
                if (spmPort->isStalled()) {
                    DPRINTF(CommInterfaceQueues, "SPM Port Stalled\n");
                    ++it;
                } else {
                    if (!spmPort->readReq) {
                        spmPort->readReq = (*it);
                        it = readQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (spmPort->readReq && spmPort->readReq->needToRead) {
                        DPRINTF(CommInterfaceQueues, "Trying read on SPM Port\n");
                        tryRead(spmPort);
                        spmRdQ->push_back(spmPort->readReq);
                        if (!spmPort->readReq->needToRead)
                            spmPort->readReq = NULL;
                    }
                }
            }
        }
        DPRINTF(CommInterface, "Checking write requests. %d requests in queue.\n", writeQueue->size());
        for (auto it=writeQueue->begin(); it!=writeQueue->end(); ) {
            DPRINTF(CommInterfaceQueues, "Request Address: %lx ", (*it)->address);
            if (!localRange.contains((*it)->address)) {
                DPRINTF(CommInterfaceQueues, "In DRAM\n");
                if (dramPort->isStalled()) {
                    DPRINTF(CommInterfaceQueues, "System Memory Port Stalled\n");
                    ++it;
                } else {
                    if (!dramPort->writeReq) {
                        dramPort->writeReq = (*it);
                        it = writeQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (dramPort->writeReq && dramPort->writeReq->needToWrite) {
                        DPRINTF(CommInterfaceQueues, "Trying write on System Memory Port\n");
                        tryWrite(dramPort);
                        dramWrQ->push_back(dramPort->writeReq);
                        if (!dramPort->writeReq->needToWrite)
                            dramPort->writeReq = NULL;
                    }
                }
            } else {
                DPRINTF(CommInterfaceQueues, "In SPM\n");
                if (spmPort->isStalled()) {
                    DPRINTF(CommInterfaceQueues, "SPM Port Stalled\n");
                    ++it;
                } else {
                    if (!spmPort->writeReq) {
                        spmPort->writeReq = (*it);
                        it = writeQueue->erase(it);;
                    } else {
                        ++it;
                    }
                    if (spmPort->writeReq && spmPort->writeReq->needToWrite) {
                        DPRINTF(CommInterfaceQueues, "Trying write on SPM Port\n");
                        tryWrite(spmPort);
                        spmWrQ->push_back(spmPort->writeReq);
                        if (!spmPort->writeReq->needToWrite)
                            spmPort->writeReq = NULL;
                    }
                }
            }
        }
    }
    requestsInQueues = readQueue->size() + writeQueue->size();
    if (!tickEvent.scheduled() && requestsInQueues>0) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
}

void
CommInterface::tick() {
    DPRINTF(CommInterface, "Tick!\n");
    checkMMR();
    requestsInQueues = readQueue->size() + writeQueue->size();
    if (requestsInQueues > 0)
        processMemoryRequests();
}

void
CommInterface::tryRead(MemSidePort * port) {
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
    RequestPtr req = make_shared<Request>(readReq->currentReadAddr, size, flags, masterId);
    DPRINTF(CommInterface, "Trying to read addr: 0x%016x, %d bytes\n",
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
    RequestPtr req = make_shared<Request>(writeReq->currentWriteAddr, size, flags, masterId);
    req->setExtraData((uint64_t)data);


    DPRINTF(CommInterface, "totalLength: %d, writeLeft: %d\n", writeReq->totalLength, writeReq->writeLeft);
    DPRINTF(CommInterface, "Trying to write to addr: 0x%016x, %d bytes, data 0x%08x\n",
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
    DPRINTF(CommInterface, "Read from 0x%lx of Size:%d Bytes Enqueued:\n", req->address, req->length);
    readQueue->push_back(req);
    DPRINTF(CommInterfaceQueues, "Current Queue:\n");
    for (auto it=readQueue->begin(); it!=readQueue->end(); ++it) {
        DPRINTF(CommInterfaceQueues, "Read Request: %lx\n", (*it)->address);
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
    DPRINTF(CommInterfaceQueues, "Current Queue:\n");
    for (auto it=writeQueue->begin(); it!=writeQueue->end(); ++it) {
        DPRINTF(CommInterfaceQueues, "Write Request: %lx\n", (*it)->address);
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
        pkt->setLE<uint8_t>(data);
        break;
      case 2:
        pkt->setLE<uint16_t>(data);
        break;
      case 4:
        pkt->setLE<uint32_t>(data);
        break;
      case 8:
        pkt->setLE<uint64_t>(data);
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

void
CommInterface::clearMemRequest(MemoryRequest * req, bool isRead) {
    if (isRead) {
        if (!localRange.contains(req->address)) {
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
        if (!localRange.contains(req->address)) {
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

/***************************************************************************************
 * Private Scratchpad Device for Accelerators using CommMemInterface
 * Acts as a simple memory for external devices
 * Enables specialization of access for parent device
 **************************************************************************************/
#include "debug/MemoryAccess.hh"

PrivateMemory::PrivateMemory(const PrivateMemoryParams *p) :
    SimpleMemory(p) {}

void
PrivateMemory::privateAccess(PacketPtr pkt) {
    assert(AddrRange(pkt->getAddr(),
                     pkt->getAddr() + (pkt->getSize() - 1)).isSubset(range));

    uint8_t *hostAddr = pmemAddr + pkt->getAddr() - range.start();

    if (pkt->isRead()) {
        assert(!pkt->isWrite());
        if (pkt->isLLSC()) {
            assert(!pkt->fromCache());
            // if the packet is not coming from a cache then we have
            // to do the LL/SC tracking here
            trackLoadLocked(pkt);
        }
        if (pmemAddr)
            memcpy(pkt->getPtr<uint8_t>(), hostAddr, pkt->getSize());
        numReads[pkt->req->masterId()]++;
        bytesRead[pkt->req->masterId()] += pkt->getSize();
    } else if (pkt->isWrite()) {
        if (writeOK(pkt)) {
            if (pmemAddr) {
                memcpy(hostAddr, pkt->getConstPtr<uint8_t>(), pkt->getSize());
                DPRINTF(MemoryAccess, "%s wrote %i bytes to address %x\n",
                        __func__, pkt->getSize(), pkt->getAddr());
            }
            assert(!pkt->req->isInstFetch());
            numWrites[pkt->req->masterId()]++;
            bytesWritten[pkt->req->masterId()] += pkt->getSize();
        }
    } else {
        panic("Unexpected packet %s", pkt->print());
    }
}

PrivateMemory*
PrivateMemoryParams::create()
{
    return new PrivateMemory(this);
}

/***************************************************************************************
 * CommInterface device with a private scratchpad
 **************************************************************************************/
CommMemInterface::CommMemInterface(Params *p) :
    CommInterface(p),
    pmem(p->private_memory),
    pmemRange(p->private_range),
    readPorts(p->private_read_ports),
    writePorts(p->private_write_ports) {
        avReadPorts = readPorts;
        avWritePorts = writePorts;
    }

void
CommMemInterface::processMemoryRequests() {
    availablePorts = avReadPorts + avWritePorts;

    if (!dramPortStalled() || !spmPortStalled() || availablePorts>0) {
        DPRINTF(CommInterface, "Checking read requests. %d requests in queue.\n", readQueue->size());
        for (auto it=readQueue->begin(); it!=readQueue->end(); ) {
            DPRINTF(CommInterfaceQueues, "Request Address: %lx\n", (*it)->address);
            if (pmemRange.contains((*it)->address)) {
                DPRINTF(CommInterfaceQueues, "In Private Memory\n");
                if (avReadPorts <= 0) {
                    DPRINTF(CommInterfaceQueues, "No available internal read ports\n");
                    ++it;
                } else {
                    avReadPorts--;
                    Request::Flags flags;
                    if ((*it)->readLeft <= 0) {
                        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to read if there aren't reads left\n");
                        return;
                    }
                    int size;
                    if ((*it)->currentReadAddr % cacheLineSize) {
                        size = cacheLineSize - ((*it)->currentReadAddr % cacheLineSize);
                        DPRINTF(CommInterface, "Aligning\n");
                    } else {
                        size = cacheLineSize;
                    }
                    size = (*it)->readLeft > (size - 1) ? size : (*it)->readLeft;
                    RequestPtr req = make_shared<Request>((*it)->currentReadAddr, size, flags, masterId);
                    DPRINTF(CommInterface, "Trying to read addr: 0x%016x, %d bytes\n",
                        req->getPaddr(), size);

                    PacketPtr pkt = new Packet(req, MemCmd::ReadReq);
                    pkt->allocate();
                    (*it)->pkt = pkt;

                    pmem->privateAccess(pkt);

                    (*it)->currentReadAddr += size;

                    (*it)->readLeft -= size;

                    DPRINTF(CommInterface, "Done with a read. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
                    pkt->writeData((*it)->buffer + (pkt->req->getPaddr() - (*it)->beginAddr));
                    DPRINTF(CommInterface, "Read:0x%016lx\n", *(uint64_t *)(*it)->buffer);
                    for (int i = pkt->req->getPaddr() - (*it)->beginAddr;
                         i < pkt->req->getPaddr() - (*it)->beginAddr + pkt->getSize(); i++)
                    {
                        (*it)->readsDone[i] = true;
                    }

                    // mark readDone as only the contiguous region
                    while ((*it)->readDone < (*it)->totalLength && (*it)->readsDone[(*it)->readDone])
                    {
                        (*it)->readDone++;
                    }

                    if ((*it)->readDone == (*it)->totalLength)
                    {
                        DPRINTF(CommInterface, "Done reading\n");
                        cu->readCommit((*it));
                        it = readQueue->erase(it);
                    }
                }
            } else if (localRange.contains((*it)->address)) {
                DPRINTF(CommInterfaceQueues, "In Local Memory\n");
                if (spmPortStalled()) {
                    DPRINTF(CommInterfaceQueues, "SPM Port Stalled\n");
                    ++it;
                } else {
                    if (!getSpmReadReq()) {
                        setSpmReadReq(*it);
                        it = readQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (getSpmReadReq() && getSpmReadReq()->needToRead) {
                        DPRINTF(CommInterfaceQueues, "Trying read on SPM Port\n");
                        tryRead(spmPort);
                        spmRdQ->push_back(getSpmReadReq());
                        if (!getSpmReadReq()->needToRead)
                            setSpmReadReq(NULL);
                    }
                }
            } else {
                DPRINTF(CommInterfaceQueues, "In System Memory\n");
                if (dramPortStalled()) {
                    DPRINTF(CommInterfaceQueues, "System Memory Port Stalled\n");
                    ++it;
                } else {
                    if (!getDramReadReq()) {
                        setDramReadReq(*it);
                        it = readQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (getDramReadReq() && getDramReadReq()->needToRead) {
                        DPRINTF(CommInterfaceQueues, "Trying read on System Memory Port\n");
                        tryRead(dramPort);
                        dramRdQ->push_back(getDramReadReq());
                        if (!getDramReadReq()->needToRead)
                            setDramReadReq(NULL);
                    }
                }
            }
        }
        DPRINTF(CommInterface, "Checking write requests. %d requests in queue.\n", writeQueue->size());
        for (auto it=writeQueue->begin(); it!=writeQueue->end(); ) {
            DPRINTF(CommInterfaceQueues, "Request Address: %lx ", (*it)->address);
            if (pmemRange.contains((*it)->address)) {
                DPRINTF(CommInterfaceQueues, "In Private Memory\n");
                if (avWritePorts <= 0) {
                    DPRINTF(CommInterfaceQueues, "No available internal read ports\n");
                    ++it;
                } else {
                    avWritePorts--;

                    if ((*it)->writeLeft <= 0) {
                        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to write if there aren't writes left\n");
                        return;
                    }

                    int size;
                    if ((*it)->currentWriteAddr % cacheLineSize) {
                        size = cacheLineSize - ((*it)->currentWriteAddr % cacheLineSize);
                        DPRINTF(CommInterface, "Aligning\n");
                    } else {
                        size = cacheLineSize;
                    }
                    size = (*it)->writeLeft > size - 1 ? size : (*it)->writeLeft;

                    Request::Flags flags;
                    uint8_t *data = new uint8_t[size];
                    std::memcpy(data, &((*it)->buffer[(*it)->totalLength-(*it)->writeLeft]), size);
                    RequestPtr req = make_shared<Request>((*it)->currentWriteAddr, size, flags, masterId);
                    req->setExtraData((uint64_t)data);


                    DPRINTF(CommInterface, "totalLength: %d, writeLeft: %d\n", (*it)->totalLength, (*it)->writeLeft);
                    DPRINTF(CommInterface, "Trying to write to addr: 0x%016x, %d bytes, data 0x%08x\n",
                        (*it)->currentWriteAddr, size, *((uint64_t*)(&((*it)->buffer[(*it)->totalLength-(*it)->writeLeft]))));

                    PacketPtr pkt = new Packet(req, MemCmd::WriteReq);
                    uint8_t *pkt_data = (uint8_t *)req->getExtraData();
                    pkt->dataDynamic(pkt_data);
                    (*it)->pkt = pkt;
                    pmem->privateAccess(pkt);

                    (*it)->currentWriteAddr += size;
                    (*it)->writeLeft -= size;

                    DPRINTF(CommInterface, "Done with a write. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
                    (*it)->writeDone += pkt->getSize();
                    if ((*it)->writeDone == (*it)->totalLength) {
                        DPRINTF(CommInterface, "Done writing\n");
                        cu->writeCommit((*it));
                        delete[] (*it)->buffer;
                        delete[] (*it)->readsDone;
                        it = writeQueue->erase(it);
                    }
                }
            } else if (localRange.contains((*it)->address)) {
                DPRINTF(CommInterfaceQueues, "In Local Memory\n");
                if (spmPortStalled()) {
                    DPRINTF(CommInterfaceQueues, "SPM Port Stalled\n");
                    ++it;
                } else {
                    if (!getSpmWriteReq()) {
                        setSpmWriteReq(*it);
                        it = writeQueue->erase(it);;
                    } else {
                        ++it;
                    }
                    if (getSpmWriteReq() && getSpmWriteReq()->needToWrite) {
                        DPRINTF(CommInterfaceQueues, "Trying write on SPM Port\n");
                        tryWrite(spmPort);
                        spmWrQ->push_back(getSpmWriteReq());
                        if (!getSpmWriteReq()->needToWrite)
                            setSpmWriteReq(NULL);
                    }
                }
            } else {
                DPRINTF(CommInterfaceQueues, "In System Memory\n");
                if (dramPortStalled()) {
                    DPRINTF(CommInterfaceQueues, "System Memory Port Stalled\n");
                    ++it;
                } else {
                    if (!getDramWriteReq()) {
                        setDramWriteReq(*it);
                        it = writeQueue->erase(it);
                    } else {
                        ++it;
                    }
                    if (getDramWriteReq() && getDramWriteReq()->needToWrite) {
                        DPRINTF(CommInterfaceQueues, "Trying write on System Memory Port\n");
                        tryWrite(dramPort);
                        dramWrQ->push_back(getDramWriteReq());
                        if (!getDramWriteReq()->needToWrite)
                            setDramWriteReq(NULL);
                    }
                }
            }
        }
    }
    requestsInQueues = readQueue->size() + writeQueue->size();
    if (!tickEvent.scheduled() && requestsInQueues>0) {
        //schedule(tickEvent, curTick() + processDelay);
        schedule(tickEvent, nextCycle());
    }
}

void
CommMemInterface::refreshMemPorts() {
      avReadPorts = readPorts;
      avWritePorts = writePorts;
}

void
CommMemInterface::tick() {
    DPRINTF(CommInterface, "Tick!\n");
    checkMMR();
    refreshMemPorts();
    requestsInQueues = readQueue->size() + writeQueue->size();
    if (requestsInQueues>0)
        processMemoryRequests();
}


CommMemInterface *
CommMemInterfaceParams::create() {
    return new CommMemInterface(this);
}
