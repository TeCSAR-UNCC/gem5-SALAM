#ifndef __HWACC_REGISTER_BANK_HH__
#define __HWACC_REGISTER_BANK_HH__

#include "mem/abstract_mem.hh"
#include "mem/port.hh"

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

        DeferredPacket(PacketPtr _pkt, Tick _tick) : tick(_tick), pkt(_pkt)
        { }
    };

    class RegPort : public ResponsePort
    {
      private:
        RegisterBank * memory;
      public:
        RegPort(const std::string& _name, RegisterBank * _memory, PortID id=InvalidPortID) :
            ResponsePort(_name, _memory, id), memory(_memory) {}
      protected:
        Tick recvAtomic(PacketPtr pkt) override { return memory->recvAtomic(pkt); };
        Tick recvAtomicBackdoor(
                PacketPtr pkt, MemBackdoorPtr &_backdoor) override { return memory->recvAtomicBackdoor(pkt,_backdoor); };
        void recvFunctional(PacketPtr pkt) override { memory->recvFunctional(pkt); };
        bool recvTimingReq(PacketPtr pkt) override { return memory->recvTimingReq(pkt); };
        void recvRespRetry() override { memory->recvRespRetry(); };
        AddrRangeList getAddrRanges() const override {
            AddrRangeList ranges;
            ranges.push_back(memory->getAddrRange());
            return ranges;
        }
    };

    RegPort port;

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
     * Remember if we failed to send a response and are awaiting a
     * retry. This is only used as a check.
     */
    bool retryResp;

    /**
     * Dequeue a packet from our internal packet queue and move it to
     * the port where it will be sent as soon as possible.
     */
    void dequeue();
    EventFunctionWrapper dequeueEvent;

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
    bool recvTimingReq(PacketPtr pkt);
    void recvRespRetry();
};
#endif //__HWACC_REGISTER_BANK_HH__