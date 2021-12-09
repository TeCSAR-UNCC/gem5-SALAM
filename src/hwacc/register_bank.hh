#ifndef __HWACC_REGISTER_BANK_HH__
#define __HWACC_REGISTER_BANK_HH__

#include "mem/abstract_mem.hh"
#include "mem/port.hh"
#include "hwacc/scratchpad_memory.hh"

#include <vector>

using namespace gem5;
using namespace memory;

#include "params/RegisterBank.hh"

class RegisterBank : public AbstractMemory
{
  public:
    PARAMS(RegisterBank);
    RegisterBank(const RegisterBankParams &p);
    void registerAccess(PacketPtr pkt);

  private:
    class DeferredPacket
    {
      public:
        const Tick tick;
        const PacketPtr pkt;
        const PortID origin;

        DeferredPacket(PacketPtr _pkt, Tick _tick, PortID _origin) : tick(_tick), pkt(_pkt), origin(_origin)
        { }
    }

    class RegPort : public ScratchpadResponsePort
    {
      private:
        RegisterBank * memory;
      public:
        RegPort(const std::string& _name, RegisterBank * _memory, PortID id=InvalidPortID) :
            ScratchpadResponsePort(_name, _memory, id), memory(_memory) {}
      protected:
        Tick recvAtomic(PacketPtr pkt) override { return memory->recvAtomic(pkt, true); };
        Tick recvAtomicBackdoor(
                PacketPtr pkt, MemBackdoorPtr &_backdoor) override { return memory->recvAtomicBackdoor(pkt,_backdoor); };
        void recvFunctional(PacketPtr pkt) override { memory->recvFunctional(pkt); };
        bool recvTimingReq(PacketPtr pkt) override { return memory->recvTimingReq(pkt, id, true); };
        void recvRespRetry() override { memory->recvRespRetry(id); };
        AddrRangeList getAddrRanges() const override {
            AddrRangeList ranges;
            ranges.push_back(memory->getAddrRange());
            return ranges;
        }
    };

    std::vector<RegPort *> reg_ports;

    /**
     * Container for register deltas. Copied to pmem on delta events.
     */
    uint8_t* deltaAddr;
    /**
     * Latency of a delta cycle in the register bank
     */
    const Tick deltaTime;
    /**
     * Internal (unbounded) storage to mimic the delay caused by the
     * delta timing of writes. Note that this is where the packet spends
     * the memory latency.
     */
    std::list<DeferredPacket> packetQueue;

    /**
     * Track the state of the memory as either idle or busy, no need
     * for an enum with only two states.
     */
    std::vector<bool> isBusy;

    /**
     * Remember if we have to retry an outstanding request that
     * arrived while we were busy.
     */
    std::vector<bool> retryReq;

    /**
     * Remember if we failed to send a response and are awaiting a
     * retry. This is only used as a check.
     */
    std::vector<bool> retryResp;

    /**
     * Release the memory after being busy and send a retry if a
     * request was rejected in the meanwhile.
     */
    void release();

    std::vector<EventFunctionWrapper> releaseEvent;
    std::vector<Tick> releaseTick;

    /**
     * Dequeue a packet from our internal packet queue and move it to
     * the port where it will be sent as soon as possible.
     */
    void dequeue();

    /**
     * Handle the delta cycle of the registers in the bank.
     * Copies updated data at deltaAddr to the storage buffer in pmemAddr.
     * Calls dequeue() to send packet responses.
     */
    void delta();

    EventFunctionWrapper deltaEvent;

  public:
    DrainState drain() override;

    Port &getPort(const std::string &if_name,
                  PortID idx=InvalidPortID) override;
    void init() override;

  protected:
    Tick recvAtomic(PacketPtr pkt);
    Tick recvAtomicBackdoor(PacketPtr pkt, MemBackdoorPtr &_backdoor);
    void recvFunctional(PacketPtr pkt);
    bool recvTimingReq(PacketPtr pkt, PortID recvPort);
    void recvRespRetry(PortID id);
}
#endif //__HWACC_REGISTER_BANK_HH__