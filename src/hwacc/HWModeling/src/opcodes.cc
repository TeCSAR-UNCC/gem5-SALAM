//------------------------------------------//
#include "opcodes.hh"
//------------------------------------------//

InstOpCodes::InstOpCodes(const InstOpCodesParams &p):
    SimObject(p),
    gep_inst(p.gep),
    phi_inst(p.phi),
    select_inst(p.select),
    ret_inst(p.ret),
    br_inst(p.br),
    switch_inst(p.switch_inst),
    indirectbr_inst(p.indirectbr),
    invoke_inst(p.invoke),
    resume_inst(p.resume),
    unreachable_inst(p.unreachable),
    icmp_inst(p.icmp),
    fcmp_inst(p.fcmp),
    trunc_inst(p.trunc),
    zext_inst(p.zext),
    sext_inst(p.sext),
    fptrunc_inst(p.fptrunc),
    fpext_inst(p.fpext),
    fptoui_inst(p.fptoui),
    fptosi_inst(p.fptosi),
    uitofp_inst(p.uitofp),
    ptrtoint_inst(p.ptrtoint),
    inttoptr_inst(p.inttoptr),
    bitcast_inst(p.bitcast),
    addrspacecast_inst(p.addrspacecast),
    call_inst(p.call),
    vaarg_inst(p.vaarg),
    landingpad_inst(p.landingpad),
    alloca_inst(p.alloca),
    load_inst(p.load),
    store_inst(p.store),
    fence_inst(p.fence),
    shl_inst(p.shl),
    lshr_inst(p.lshr),
    ashr_inst(p.ashr),
    and_inst(p.and_inst),
    or_inst(p.or_inst),
    xor_inst(p.xor_inst),
    add_inst(p.add),
    sub_inst(p.sub),
    mul_inst(p.mul),
    udiv_inst(p.udiv),
    sdiv_inst(p.sdiv),
    urem_inst(p.urem),
    srem_inst(p.srem),
    fadd_inst(p.fadd),
    fsub_inst(p.fsub),
    fmul_inst(p.fmul),
    fdiv_inst(p.fdiv),
    frem_inst(p.frem)
    { 
        usage.insert(std::pair<int, int>(counter_inst, 0));
        usage.insert(std::pair<int, int>(gep_inst, 0));
        usage.insert(std::pair<int, int>(phi_inst, 0));
        usage.insert(std::pair<int, int>(select_inst, 0));
        usage.insert(std::pair<int, int>(ret_inst, 0));
        usage.insert(std::pair<int, int>(br_inst, 0));
        usage.insert(std::pair<int, int>(switch_inst, 0));
        usage.insert(std::pair<int, int>(indirectbr_inst, 0));
        usage.insert(std::pair<int, int>(invoke_inst, 0));
        usage.insert(std::pair<int, int>(resume_inst, 0));
        usage.insert(std::pair<int, int>(unreachable_inst, 0));
        usage.insert(std::pair<int, int>(icmp_inst, 0));
        usage.insert(std::pair<int, int>(fcmp_inst, 0));
        usage.insert(std::pair<int, int>(trunc_inst, 0));
        usage.insert(std::pair<int, int>(zext_inst, 0));
        usage.insert(std::pair<int, int>(sext_inst, 0));
        usage.insert(std::pair<int, int>(fptrunc_inst, 0));
        usage.insert(std::pair<int, int>(fpext_inst, 0));
        usage.insert(std::pair<int, int>(fptoui_inst, 0));
        usage.insert(std::pair<int, int>(fptosi_inst, 0));
        usage.insert(std::pair<int, int>(uitofp_inst, 0));
        usage.insert(std::pair<int, int>(sitofp_inst, 0));
        usage.insert(std::pair<int, int>(ptrtoint_inst, 0));
        usage.insert(std::pair<int, int>(inttoptr_inst, 0));
        usage.insert(std::pair<int, int>(bitcast_inst, 0));
        usage.insert(std::pair<int, int>(addrspacecast_inst, 0));
        usage.insert(std::pair<int, int>(call_inst, 0));
        usage.insert(std::pair<int, int>(vaarg_inst, 0));
        usage.insert(std::pair<int, int>(landingpad_inst, 0));
        usage.insert(std::pair<int, int>(catchpad_inst, 0));
        usage.insert(std::pair<int, int>(alloca_inst, 0));
        usage.insert(std::pair<int, int>(load_inst, 0));
        usage.insert(std::pair<int, int>(store_inst, 0));
        usage.insert(std::pair<int, int>(fence_inst, 0));
        usage.insert(std::pair<int, int>(cmpxchg_inst, 0));
        usage.insert(std::pair<int, int>(atomicrmw_inst, 0));
        usage.insert(std::pair<int, int>(extractvalue_inst, 0));
        usage.insert(std::pair<int, int>(insertvalue_inst, 0));
        usage.insert(std::pair<int, int>(extractelement_inst, 0));
        usage.insert(std::pair<int, int>(insertelement_inst, 0));
        usage.insert(std::pair<int, int>(shufflevector_inst, 0));
        usage.insert(std::pair<int, int>(shl_inst, 0));
        usage.insert(std::pair<int, int>(lshr_inst, 0));
        usage.insert(std::pair<int, int>(ashr_inst, 0));
        usage.insert(std::pair<int, int>(and_inst, 0));
        usage.insert(std::pair<int, int>(or_inst, 0));
        usage.insert(std::pair<int, int>(xor_inst, 0));
        usage.insert(std::pair<int, int>(add_inst, 0));
        usage.insert(std::pair<int, int>(sub_inst, 0));
        usage.insert(std::pair<int, int>(mul_inst, 0));
        usage.insert(std::pair<int, int>(udiv_inst, 0));
        usage.insert(std::pair<int, int>(sdiv_inst, 0));
        usage.insert(std::pair<int, int>(urem_inst, 0));
        usage.insert(std::pair<int, int>(srem_inst, 0));
        usage.insert(std::pair<int, int>(fadd_inst, 0));
        usage.insert(std::pair<int, int>(fsub_inst, 0));
        usage.insert(std::pair<int, int>(fmul_inst, 0));
        usage.insert(std::pair<int, int>(fdiv_inst, 0));
        usage.insert(std::pair<int, int>(frem_inst, 0));
    }
