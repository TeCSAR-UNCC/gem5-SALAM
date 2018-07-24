#include "hwacc/stream_dma.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "debug/AddrRanges.hh"
#include "debug/StreamDma.hh"

StreamDma::StreamDma(const Params *p)
    : DmaDevice(p),
    devname(p->devicename),
    pioAddr(p->pio_addr),
    pioDelay(p->pio_delay),
    pioSize(p->pio_size),
    memDelay(p->mem_delay),
    rdBufferSize(p->read_buffer_size),
    wrBufferSize(p->write_buffer_size),
    maxPending(p->max_pending),
    maxReqSize(p->max_req_size),
    gic(p->gic),
    rdInt(p->rd_int),
    wrInt(p->wr_int),
    tickEvent(this) {
    readFifo = new DmaReadFifo(dmaPort, rdBufferSize, maxReqSize, maxPending);
    writeFifo = new DmaWriteFifo(dmaPort, wrBufferSize, maxReqSize, maxPending);
    mmreg = new uint8_t[32];
    for (int i=0; i<pioSize; i++)
        mmreg[i]=0;
    FLAGS = mmreg;
    CONFIG = (uint16_t *)(mmreg+CONFIG_OFF);
    RD_ADDR = (uint64_t *)(mmreg+RD_ADDR_OFF);
    WR_ADDR = (uint64_t *)(mmreg+WR_ADDR_OFF);
    RD_FRAME_SIZE = (uint32_t *)(mmreg+RD_FRAME_SIZE_OFF);
    NUM_RD_FRAMES = (uint8_t *)(mmreg+NUM_RD_FRAMES_OFF);
    RD_FRAME_BUFF_SIZE = (uint8_t *)(mmreg+RD_FRAME_BUFF_SIZE_OFF);
    WR_FRAME_SIZE = (uint32_t *)(mmreg+WR_FRAME_SIZE_OFF);
    NUM_WR_FRAMES = (uint8_t *)(mmreg+NUM_WR_FRAMES_OFF);
    WR_FRAME_BUFF_SIZE = (uint8_t *)(mmreg+WR_FRAME_BUFF_SIZE_OFF);

    rdRunning = false;
    wrRunning = false;
    running = false;
}

AddrRangeList
StreamDma::getAddrRanges() const
{
    assert(pioSize != 0);
    AddrRangeList ranges;
    DPRINTF(AddrRanges, "registering range: %#x-%#x\n", pioAddr, pioSize);
    ranges.push_back(RangeSize(pioAddr, pioSize));
    return ranges;
}

void
StreamDma::tick() {
    if (!rdRunning && ((*FLAGS&RD_START_MASK)==RD_START_MASK)) {
        rdRunning = true;
        *FLAGS &= !RD_START_MASK;
        *FLAGS |= RD_RUNNING_MASK;
        readAddr = *RD_ADDR;
        readPtr = readAddr;
        readFrameSize = *RD_FRAME_SIZE;
        framesToRead = *NUM_RD_FRAMES;
        readFrameBuffSize = *RD_FRAME_BUFF_SIZE;
        framesRead = 0;
        readIntFrames = *(uint8_t *)CONFIG;

        DPRINTF(StreamDma, "Initializing frame read from 0x%016x with frame size of %d Bytes\n", readPtr, readFrameSize);
        readFifo->startFill(readPtr, readFrameSize);
    }

    if (!wrRunning && ((*FLAGS&WR_START_MASK)==WR_START_MASK)) {
        wrRunning = true;
        *FLAGS &= !WR_START_MASK;
        *FLAGS |= WR_RUNNING_MASK;
        writeAddr = *WR_ADDR;
        writePtr = writeAddr;
        writeFrameSize = *WR_FRAME_SIZE;
        framesToWrite = *NUM_WR_FRAMES;
        writeFrameBuffSize = *WR_FRAME_BUFF_SIZE;
        framesWritten = 0;
        writeIntFrames = *CONFIG>>8;

        DPRINTF(StreamDma, "Initializing frame write to 0x%016x with frame size of %d Bytes\n", writePtr, writeFrameSize);
        writeFifo->startEmpty(writePtr, writeFrameSize);
    }

    if ((*FLAGS&RD_INT_MASK) != RD_INT_MASK) {
        gic->clearInt(rdInt);
    }

    if ((*FLAGS&WR_INT_MASK) != WR_INT_MASK) {
        gic->clearInt(wrInt);
    }

    if (rdRunning && !readFifo->isActive()) {
        framesRead++;
        DPRINTF(StreamDma, "Frame %d of %d read\n", framesRead, framesToRead);
        if (readIntFrames != 0) {
            if (framesRead % readIntFrames == 0) {
                gic->sendInt(rdInt);
                *FLAGS |= RD_INT_MASK;
            }
        }
        if ((framesToRead != 0) && (framesRead >= framesToRead)) {
            rdRunning = false;
            *FLAGS &= !RD_RUNNING_MASK;
        } else {
            assert(readFrameBuffSize != 0);
            readPtr = readAddr + ((framesRead % readFrameBuffSize) * readFrameSize);
            DPRINTF(StreamDma, "Initializing frame read from 0x%016x with frame size of %d Bytes\n", readPtr, readFrameSize);
            readFifo->startFill(readPtr, readFrameSize);
        }
    }

    if (wrRunning && !writeFifo->isActive()) {
        framesWritten++;
        DPRINTF(StreamDma, "Frame %d of %d written\n", framesWritten, framesToWrite);
        if (writeIntFrames != 0) {
            if (framesWritten % writeIntFrames == 0) {
                gic->sendInt(wrInt);
                *FLAGS |= WR_INT_MASK;
            }
        }
        if ((framesToWrite != 0) && (framesWritten >= framesToWrite)) {
            wrRunning = false;
            *FLAGS &= !WR_RUNNING_MASK;
        } else {
            assert(writeFrameBuffSize != 0);
            writePtr = writeAddr + ((framesWritten % writeFrameBuffSize) * writeFrameSize);
            DPRINTF(StreamDma, "Initializing frame write to 0x%016x with frame size of %d Bytes\n", writePtr, writeFrameSize);
            writeFifo->startEmpty(writePtr, writeFrameSize);
        }
    }

    running = rdRunning || wrRunning;

    if (!tickEvent.scheduled() && running) {
        schedule(tickEvent, nextCycle());
    }
}

Tick
StreamDma::read(PacketPtr pkt) {

    Addr offset = pkt->req->getPaddr() - pioAddr;

    if (offset < BUFFER_ACCESS_OFF) {
        DPRINTF(StreamDma, "The MMR associated with this DMA was read from!\n");

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
    } else {
        DPRINTF(StreamDma, "The data buffer associated with this DMA was read from!\n");

        uint8_t *buff = new uint8_t[pkt->getSize()];
        readFifo->get(buff, pkt->getSize());
        uint64_t data = *(uint64_t *)buff;
        delete buff;

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
          case 8:
            pkt->set<uint64_t>(data);
            break;
          default:
            panic("Read size too big?\n");
            break;
        }
    }

    if (!tickEvent.scheduled()) {
        schedule(tickEvent, nextCycle());
    }
    pkt->makeAtomicResponse();
    return pioDelay;
}

Tick
StreamDma::write(PacketPtr pkt) {

    Addr offset = pkt->req->getPaddr() - pioAddr;

    if (offset < BUFFER_ACCESS_OFF) {
        DPRINTF(StreamDma, "The MMR associated with this DMA was written to!\n");

        pkt->writeData(mmreg + offset);
    } else {
        DPRINTF(StreamDma, "The data buffer associated with this DMA was written to!\n");
        uint8_t * data = new uint8_t[pkt->getSize()];
        pkt->writeData(data);
        writeFifo->fill(data, pkt->getSize());
    }

    if (!tickEvent.scheduled()) {
        schedule(tickEvent, nextCycle());
    }
    pkt->makeAtomicResponse();
    return pioDelay;
}

StreamDma *
StreamDmaParams::create() {
    return new StreamDma(this);
}
