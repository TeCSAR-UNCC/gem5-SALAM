//------------------------------------------//
#include "hwacc/noncoherent_dma.hh"
//------------------------------------------//

NoncoherentDma::NoncoherentDma(const NoncoherentDmaParams &p)
    : DmaDevice(p),
    devname(p.devicename),
    pioAddr(p.pio_addr),
    pioDelay(p.pio_delay),
    pioSize(p.pio_size),
    bufferSize(p.buffer_size),
    maxPending(p.max_pending),
    maxReqSize(p.max_req_size),
    gic(p.gic),
    intNum(p.int_num),
    clock_period(p.clock_period),
    tickEvent([this]{tick();}, name()),
    accPort(this, sys, p.sid, p.ssid) {
    memSideReadFifo = new DmaReadFifo(dmaPort, size_t(bufferSize/2), maxReqSize, maxPending);
    memSideWriteFifo = new DmaWriteFifo(dmaPort, size_t(bufferSize/2), maxReqSize, maxPending);
    accSideReadFifo = new DmaReadFifo(accPort, size_t(bufferSize/2), maxReqSize, maxPending);
    accSideWriteFifo = new DmaWriteFifo(accPort, size_t(bufferSize/2), maxReqSize, maxPending);
    readFifo = nullptr;
    writeFifo = nullptr;
    mmreg = new uint8_t[pioSize];
    for (int i=0; i<pioSize; i++)
        mmreg[i]=0;
    FLAGS = mmreg;
    last_flag = 0;
    SRC = (uint64_t *)(mmreg+1);
    DST = (uint64_t *)(mmreg+9);
    LEN = (int *)(mmreg+17);
    running = false;
}

AddrRangeList
NoncoherentDma::getAddrRanges() const
{
    assert(pioSize != 0);
    AddrRangeList ranges;
    DPRINTF(AddrRanges, "registering range: %#x-%#x\n", pioAddr, pioSize);
    ranges.push_back(RangeSize(pioAddr, pioSize));
    return ranges;
}

// Select the appropriate DmaReadFifo based on which port holds
// the active read address
DmaReadFifo *
NoncoherentDma::getActiveReadFifo() {
    AddrRangeList accPortRanges = accPort.getAddrRanges();
    for (auto range : accPortRanges) {
        if (range.contains(activeSrc)) return accSideReadFifo;
    }
    return memSideReadFifo;
}

// Select the appropriate DmaWriteFifo based on which port holds
// the active write address
DmaWriteFifo *
NoncoherentDma::getActiveWriteFifo() {
    AddrRangeList accPortRanges = accPort.getAddrRanges();
    for (auto range : accPortRanges) {
        if (range.contains(activeDst)) return accSideWriteFifo;
    }
    return memSideWriteFifo;
}

void
NoncoherentDma::tick() {
    if (!running && ((*FLAGS&0x01)==0x01)) {
        running = true;
        *FLAGS &= 0xFE;
        *FLAGS |= 0x02;
        activeSrc = *SRC;
        activeDst = *DST;
        writesLeft = *LEN;
        DPRINTF(NoncoherentDma, "SRC:0x%016x, DST:0x%016x, LEN:%d\n", activeSrc, activeDst, writesLeft);
        start_time = curTick();
        readFifo = getActiveReadFifo();
        writeFifo = getActiveWriteFifo();
        readFifo->startFill(activeSrc, writesLeft);
        writeFifo->startEmpty(activeDst, writesLeft);
    }
    if (((last_flag&0x04)==0x04) && ((*FLAGS&0x04) != 0x04)) {
        //clear interrupts
        gic->clearInt(intNum);
    }
    if (running) {
        if (writesLeft > 0) {
            int toWrite = MIN(maxReqSize, writesLeft);
            if (writeFifo->canFill(toWrite)) {
                uint8_t * data = new uint8_t[toWrite];
                if (readFifo->tryGet(data, toWrite)) {
                    writeFifo->fill(data, toWrite);
                    writesLeft -= toWrite;
                }
                delete[] data;
            }
        } else {
            if (!writeFifo->isActive()) {
                running = false;
                *FLAGS &= 0xFD;
                *FLAGS |= 0x04;
                //raise interrupts
                gic->sendInt(intNum);
                double xfer_time = (double)(curTick() - start_time) * (1e-6);
                DPRINTF(NoncoherentDma, "Transfer completed in %f us\n", xfer_time);
            }
        }
    }
	last_flag = *FLAGS;
    if (!tickEvent.scheduled() && running) {
        schedule(tickEvent, curTick() + clock_period*1000);
    }
}

Tick
NoncoherentDma::read(PacketPtr pkt) {
    DPRINTF(DeviceMMR, "The address range associated with this DMA was read!\n");

    Addr offset = pkt->req->getPaddr() - pioAddr;

    uint64_t data;

    data = *(uint64_t *)(mmreg+offset);

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
NoncoherentDma::write(PacketPtr pkt) {
    DPRINTF(DeviceMMR,
        "The address range associated with this DMA was written to!\n");
    DPRINTF(DeviceMMR, "LEN Reg:0x%08x\n", *LEN);
    DPRINTF(DeviceMMR, "SRC Reg:0x%016x\n", *SRC);
    DPRINTF(DeviceMMR, "DST Reg:0x%016x\n", *DST);
    DPRINTF(DeviceMMR, "FLAGS Reg:0x%02x\n", *FLAGS);

    pkt->writeData(mmreg + (pkt->req->getPaddr() - pioAddr));

    if (!tickEvent.scheduled()) {
        schedule(tickEvent, curTick() + clock_period*1000);
    }
    pkt->makeAtomicResponse();
    return pioDelay;
}

Port &
NoncoherentDma::getPort(const std::string &if_name, PortID idx)
{
    if (if_name == "cluster_dma") {
        return accPort;
    }
    return DmaDevice::getPort(if_name, idx);
}

// NoncoherentDma *
// NoncoherentDmaParams::create() {
//     return new NoncoherentDma(this);
// }
