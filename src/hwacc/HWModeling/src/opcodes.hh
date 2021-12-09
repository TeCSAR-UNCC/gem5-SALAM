#ifndef __HWMODEL_INSTOPCODES_HH__
#define __HWMODEL_INSTOPCODES_HH__
//------------------------------------------//
//#include "debug_flags.hh"
#include "params/InstOpCodes.hh"
#include "sim/sim_object.hh"
//------------------------------------------//
#include <cstdint>
#include <map>
//------------------------------------------//

using namespace gem5;

class InstOpCodes : public SimObject
{
    public:
    uint32_t counter_inst;
    uint32_t gep_inst;
    uint32_t phi_inst;
    uint32_t select_inst;
    uint32_t ret_inst;
    uint32_t br_inst;
    uint32_t switch_inst;
    uint32_t indirectbr_inst;
    uint32_t invoke_inst;
    uint32_t resume_inst;
    uint32_t unreachable_inst;
    uint32_t icmp_inst;
    uint32_t fcmp_inst;
    uint32_t trunc_inst;
    uint32_t zext_inst;
    uint32_t sext_inst;
    uint32_t fptrunc_inst;
    uint32_t fpext_inst;
    uint32_t fptoui_inst;
    uint32_t fptosi_inst;
    uint32_t uitofp_inst;
    uint32_t sitofp_inst;
    uint32_t ptrtoint_inst;
    uint32_t inttoptr_inst;
    uint32_t bitcast_inst;
    uint32_t addrspacecast_inst;
    uint32_t call_inst;
    uint32_t vaarg_inst;
    uint32_t landingpad_inst;
    uint32_t catchpad_inst;
    uint32_t alloca_inst;
    uint32_t load_inst;
    uint32_t store_inst;
    uint32_t fence_inst;
    uint32_t cmpxchg_inst;
    uint32_t atomicrmw_inst;
    uint32_t extractvalue_inst;
    uint32_t insertvalue_inst;
    uint32_t extractelement_inst;
    uint32_t insertelement_inst;
    uint32_t shufflevector_inst;
    uint32_t shl_inst;
    uint32_t lshr_inst;
    uint32_t ashr_inst;
    uint32_t and_inst;
    uint32_t or_inst;
    uint32_t xor_inst;
    uint32_t add_inst;
    uint32_t sub_inst;
    uint32_t mul_inst;
    uint32_t udiv_inst;
    uint32_t sdiv_inst;
    uint32_t urem_inst;
    uint32_t srem_inst;
    uint32_t fadd_inst;
    uint32_t fsub_inst;
    uint32_t fmul_inst;
    uint32_t fdiv_inst;
    uint32_t frem_inst;

    std::map<int, int> usage;

    InstOpCodes();
    InstOpCodes(const InstOpCodesParams &p);
    void update_usage(uint64_t OpCode) { usage[OpCode]++; };
    uint64_t get_usage(uint64_t OpCode) { return usage[OpCode]; }

};

#endif //__HWMODEL_INSTOPCODES_HH__
