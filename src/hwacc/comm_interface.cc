#include "hwacc/comm_interface.hh"
#include "base/trace.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"

#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

/***************************************************************************************
 * CommInterface serves as the general system interface for hardware accelerators. It
 * provides a set of memory-mapped registers, as well as master ports for accessing
 * both local busses/SPMs and system memory. 
 **************************************************************************************/
CommInterface::CommInterface(Params *p) :
    BasicPioDevice(p, p->pio_size),
    io_addr(p->pio_addr),
    io_size(p->pio_size),
    flag_size(p->flags_size),
    config_size(p->config_size),
    devname(p->devicename),
    gic(p->gic),
    int_num(p->int_num),
    use_premap_data(p->premap_data),
    dramSide(p->name + ".dram_side", this),
    spmSide(p->name + ".spm_side", this),
    localRange(p->local_range),
    masterId(p->system->getMasterId(this,name())),
    tickEvent(this),
    cacheLineSize(p->cache_line_size),
    cacheSize(p->cache_size),
    readPorts(p->private_read_ports),
    writePorts(p->private_write_ports),
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

    if (use_premap_data) {
        for (auto i = 0; i < p->data_bases.size(); i++) {
            data_base_ptrs.push_back(p->data_bases[i]);
        }
    }
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
            owner->schedule(owner->tickEvent, curTick() + owner->processDelay);
            //owner->schedule(owner->tickEvent, owner->nextCycle());
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
        } else {
            readQueue->push_front(readReq);
            clearMemRequest(readReq, true); // Clear the request from the in-flight queue
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
        } else {
            writeQueue->push_front(writeReq);
            clearMemRequest(writeReq, false); // Clear the request from the in-flight queue
        }
    } else {
        panic("Something went very wrong!");
    }
    if (!tickEvent.scheduled())
    {
        schedule(tickEvent, curTick() + processDelay);
        //schedule(tickEvent, nextCycle());
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
            schedule(tickEvent, curTick() + processDelay);
            //schedule(tickEvent, nextCycle());
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
        schedule(tickEvent, curTick() + processDelay);
        //schedule(tickEvent, nextCycle());
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
            schedule(tickEvent, curTick() + processDelay);
            //schedule(tickEvent, nextCycle());
        }
    } else {
        if (!port->isStalled() && !tickEvent.scheduled())
        {
            schedule(tickEvent, curTick() + processDelay);
            //schedule(tickEvent, nextCycle());
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
            schedule(tickEvent, curTick() + processDelay);
            //schedule(tickEvent, nextCycle());
        }
    } else if (!port->isStalled() && !tickEvent.scheduled()) {
            schedule(tickEvent, curTick() + processDelay);
            //schedule(tickEvent, nextCycle());
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
        schedule(tickEvent, curTick() + processDelay);
        //schedule(tickEvent, nextCycle());
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
        schedule(tickEvent, curTick() + processDelay);
        //schedule(tickEvent, nextCycle());
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
    if (use_premap_data) {
        return data_base_ptrs.at(index);
    } else {
        return *(uint64_t *)(mmreg + VAR_OFFSET + index*8);
    }
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
    SimpleMemory(p), readyMode(p->ready_mode),
    resetOnPrivateRead(p->reset_on_private_read) {
    ready = new bool[range.size()];
    if (readyMode) {
        for (auto i=0;i<range.size();i++) {
            ready[i] = false;
        }
    }
}

bool
PrivateMemory::isReady(Addr ad, Addr size) {
    if (!readyMode) return true;
    Addr start_offset = ad - range.start();
    Addr end_offset = start_offset + size;
    for (auto i=start_offset; i<end_offset; i++) {
        if (ready[i] == false) return false;
    }
    return true;
}

void
PrivateMemory::setAllReady(bool r) {
    for (auto i=0;i<range.size();i++) {
        ready[i] = r;
    }
}

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
        if (resetOnPrivateRead) {
            Addr start_offset = pkt->getAddr() - range.start();
            Addr end_offset = start_offset + pkt->getSize();
            for (auto i=start_offset; i<end_offset; i++) {
                ready[i] = false;
            }
        }
        numReads[pkt->req->masterId()]++;
        bytesRead[pkt->req->masterId()] += pkt->getSize();
    } else if (pkt->isWrite()) {
        if (writeOK(pkt)) {
            if (pmemAddr) {
                memcpy(hostAddr, pkt->getConstPtr<uint8_t>(), pkt->getSize());
                DPRINTF(MemoryAccess, "%s wrote %i bytes to address %x\n",
                        __func__, pkt->getSize(), pkt->getAddr());
            }

            if (readyMode) {
                Addr start_offset = pkt->getAddr() - range.start();
                Addr end_offset = start_offset + pkt->getSize();
                for (auto i=start_offset; i<end_offset; i++) {
                    ready[i] = true;
                }
            }

            assert(!pkt->req->isInstFetch());
            numWrites[pkt->req->masterId()]++;
            bytesWritten[pkt->req->masterId()] += pkt->getSize();
        }
    } else {
        panic("Unexpected packet %s", pkt->print());
    }
}

static inline void
tracePacket(System *sys, const char *label, PacketPtr pkt)
{
    int size = pkt->getSize();
#if THE_ISA != NULL_ISA
    if (size == 1 || size == 2 || size == 4 || size == 8) {
        DPRINTF(MemoryAccess,"%s from %s of size %i on address %#x data "
                "%#x %c\n", label, sys->getMasterName(pkt->req->masterId()),
                size, pkt->getAddr(), pkt->getUintX(TheISA::GuestByteOrder),
                pkt->req->isUncacheable() ? 'U' : 'C');
        return;
    }
#endif
    DPRINTF(MemoryAccess, "%s from %s of size %i on address %#x %c\n",
            label, sys->getMasterName(pkt->req->masterId()),
            size, pkt->getAddr(), pkt->req->isUncacheable() ? 'U' : 'C');
    DDUMP(MemoryAccess, pkt->getConstPtr<uint8_t>(), pkt->getSize());
}

#if TRACING_ON
#   define TRACE_PACKET(A) tracePacket(system(), A, pkt)
#else
#   define TRACE_PACKET(A)
#endif

void
PrivateMemory::access(PacketPtr pkt)
{
    if (pkt->cacheResponding()) {
        DPRINTF(MemoryAccess, "Cache responding to %#llx: not responding\n",
                pkt->getAddr());
        return;
    }

    if (pkt->cmd == MemCmd::CleanEvict || pkt->cmd == MemCmd::WritebackClean) {
        DPRINTF(MemoryAccess, "CleanEvict  on 0x%x: not responding\n",
                pkt->getAddr());
      return;
    }

    assert(AddrRange(pkt->getAddr(),
                     pkt->getAddr() + (pkt->getSize() - 1)).isSubset(range));

    uint8_t *hostAddr = pmemAddr + pkt->getAddr() - range.start();

    if (pkt->cmd == MemCmd::SwapReq) {
        if (pkt->isAtomicOp()) {
            if (pmemAddr) {
                pkt->setData(hostAddr);
                (*(pkt->getAtomicOp()))(hostAddr);
            }
        } else {
            std::vector<uint8_t> overwrite_val(pkt->getSize());
            uint64_t condition_val64;
            uint32_t condition_val32;

            panic_if(!pmemAddr, "Swap only works if there is real memory " \
                     "(i.e. null=False)");

            bool overwrite_mem = true;
            // keep a copy of our possible write value, and copy what is at the
            // memory address into the packet
            pkt->writeData(&overwrite_val[0]);
            pkt->setData(hostAddr);

            if (pkt->req->isCondSwap()) {
                if (pkt->getSize() == sizeof(uint64_t)) {
                    condition_val64 = pkt->req->getExtraData();
                    overwrite_mem = !std::memcmp(&condition_val64, hostAddr,
                                                 sizeof(uint64_t));
                } else if (pkt->getSize() == sizeof(uint32_t)) {
                    condition_val32 = (uint32_t)pkt->req->getExtraData();
                    overwrite_mem = !std::memcmp(&condition_val32, hostAddr,
                                                 sizeof(uint32_t));
                } else
                    panic("Invalid size for conditional read/write\n");
            }

            if (overwrite_mem)
                std::memcpy(hostAddr, &overwrite_val[0], pkt->getSize());

            assert(!pkt->req->isInstFetch());
            TRACE_PACKET("Read/Write");
            numOther[pkt->req->masterId()]++;
        }
    } else if (pkt->isRead()) {
        assert(!pkt->isWrite());
        if (pkt->isLLSC()) {
            assert(!pkt->fromCache());
            // if the packet is not coming from a cache then we have
            // to do the LL/SC tracking here
            trackLoadLocked(pkt);
        }
        if (pmemAddr) {
            pkt->setData(hostAddr);
        }
        TRACE_PACKET(pkt->req->isInstFetch() ? "IFetch" : "Read");
        numReads[pkt->req->masterId()]++;
        bytesRead[pkt->req->masterId()] += pkt->getSize();
        if (pkt->req->isInstFetch())
            bytesInstRead[pkt->req->masterId()] += pkt->getSize();
    } else if (pkt->isInvalidate() || pkt->isClean()) {
        assert(!pkt->isWrite());
        // in a fastmem system invalidating and/or cleaning packets
        // can be seen due to cache maintenance requests

        // no need to do anything
    } else if (pkt->isWrite()) {
        if (writeOK(pkt)) {
            if (pmemAddr) {
                pkt->writeData(hostAddr);
                DPRINTF(MemoryAccess, "%s wrote %i bytes to address %x\n",
                        __func__, pkt->getSize(), pkt->getAddr());
            }
            assert(!pkt->req->isInstFetch());
            TRACE_PACKET("Write");
            numWrites[pkt->req->masterId()]++;
            bytesWritten[pkt->req->masterId()] += pkt->getSize();
        }
        // Set ready bits on external writes
        if (readyMode) {
            Addr start_offset = pkt->getAddr() - range.start();
            Addr end_offset = start_offset + pkt->getSize();
            for (auto i=start_offset; i<end_offset; i++) {
                ready[i] = true;
            }
        }
    } else {
        panic("Unexpected packet %s", pkt->print());
    }

    if (pkt->needsResponse()) {
        pkt->makeResponse();
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
    resetPmemOnFinish(p->reset_private_on_finish),
    cacheSize(p->cache_size),
    privateSize(p->private_size),
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
			if ((*it)->readDone == (*it)->totalLength)
            {
                DPRINTF(CommInterface, "Done reading\n");
                cu->readCommit((*it));
                it = readQueue->erase(it);
				if (readQueue->empty()) break;
            }
            if (pmemRange.contains((*it)->address)) {
                DPRINTF(CommInterfaceQueues, "In Private Memory\n");
                int size;
                if ((*it)->currentReadAddr % cacheLineSize) {
                    size = cacheLineSize - ((*it)->currentReadAddr % cacheLineSize);
                    DPRINTF(CommInterface, "Aligning\n");
                } else {
                    size = cacheLineSize;
                }
                size = (*it)->readLeft > (size - 1) ? size : (*it)->readLeft;

                if (avReadPorts <= 0) {
                    DPRINTF(CommInterfaceQueues, "No available internal read ports\n");
                    ++it;
                } else if (!pmem->isReady((*it)->address, size)) {
                    DPRINTF(CommInterfaceQueues, "Data at %lx not ready to read\n", (*it)->address);
					//std::cout << "Data at " << (*it)->address << " is not ready\n";
                    ++it;
                } else {
                    avReadPorts--;
                    Request::Flags flags;
                    if ((*it)->readLeft <= 0) {
                        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to read if there aren't reads left\n");
                        return;
                    }
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

//                    if ((*it)->readDone == (*it)->totalLength)
//                    {
//                        DPRINTF(CommInterface, "Done reading\n");
//                        cu->readCommit((*it));
//                        it = readQueue->erase(it);
//                    }
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
                if ((*it)->writeDone == (*it)->totalLength) {
                    DPRINTF(CommInterface, "Done writing\n");
                    cu->writeCommit((*it));
                    delete[] (*it)->buffer;
                    delete[] (*it)->readsDone;
                    it = writeQueue->erase(it);
                    if (writeQueue->empty()) break;
                }
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
//                    if ((*it)->writeDone == (*it)->totalLength) {
//                        DPRINTF(CommInterface, "Done writing\n");
//                        cu->writeCommit((*it));
//                        delete[] (*it)->buffer;
//                        delete[] (*it)->readsDone;
//                        it = writeQueue->erase(it);
//                    }
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
        schedule(tickEvent, curTick() + processDelay);
        //schedule(tickEvent, nextCycle());
    }
}

void
CommMemInterface::refreshMemPorts() {
      avReadPorts = readPorts;
      avWritePorts = writePorts;
}

void
CommMemInterface::finish() {
    *mmreg &= 0xfc;
    *mmreg |= 0x04;
    int_flag = true;
    computationNeeded = false;
    if (resetPmemOnFinish)
        pmem->setAllReady(false);
    gic->sendInt(int_num);
}

void
CommMemInterface::tick() {
    //DPRINTF(CommInterface, "Tick!\n");
    checkMMR();
    //refreshMemPorts(); //Should be called from the ComputeUnit
    requestsInQueues = readQueue->size() + writeQueue->size();
    if (requestsInQueues>0)
        processMemoryRequests();
}


CommMemInterface *
CommMemInterfaceParams::create() {
    return new CommMemInterface(this);
}
