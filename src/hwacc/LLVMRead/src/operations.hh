#ifndef __OPERATIONS_HH__

#define __OPERATIONS_HH__

#include <string>
#include <map>
//#include <cstdint>
#include <sstream>
#include <vector>
#include <cmath>
#include "base/types.hh"

#define DEFAULT -1

struct parameters{
	std::string returnReg, operand1, operand2, dataType;
	
	
};

class Operations {
  public:  
    /* LLVM Terminator Instructions */
    void llvm_move(struct parameters);
    static void llvm_ret(struct parameters);
    static std::string llvm_br(struct parameters);
    static void llvm_switch(struct parameters);
    static void llvm_indirectbr(struct parameters);
    static void llvm_invoke(struct parameters);
    static void llvm_resume(struct parameters);
    static void llvm_unreachable(struct parameters);

    /* LLVM Binary Operations*/
    static void llvm_add(struct parameters);
    static void llvm_fadd(struct parameters);
    static void llvm_sub(struct parameters);
    static void llvm_fsub(struct parameters);
    static void llvm_mul(struct parameters);
    static void llvm_fmul(struct parameters);
    static void llvm_udiv(struct parameters);
    static void llvm_sdiv(struct parameters);
    static void llvm_fdiv(struct parameters);
    static void llvm_urem(struct parameters);
    static void llvm_srem(struct parameters);
    static void llvm_frem(struct parameters);

    /* LLVM Bitwise Binary Operations */
    static void llvm_shl(struct parameters);
    static void llvm_lshr(struct parameters);
    static void llvm_ashr(struct parameters);
    static void llvm_and(struct parameters);
    static void llvm_or(struct parameters);
    static void llvm_xor(struct parameters);

    /* LLVM Memory Access Operations */
    static void llvm_alloca(struct parameters);
    static void llvm_load(struct parameters);
    static void llvm_store(struct parameters);
    static void llvm_getelementptr(struct parameters);
    static void llvm_fence(struct parameters);
    static void llvm_cmpxchg(struct parameters);
    static void llvm_atomicrmw(struct parameters);

    /* LLVM Conversion Operations */
    static void llvm_trunc(struct parameters);
    static void llvm_zext(struct parameters);
    static void llvm_sext(struct parameters);
    static void llvm_fptoui(struct parameters);
    static void llvm_fptosi(struct parameters);
    static void llvm_uitofp(struct parameters);
    static void llvm_sitofp(struct parameters);
    static void llvm_fptrunc(struct parameters);
    static void llvm_fpext(struct parameters);
    static void llvm_ptrtoint(struct parameters);
    static void llvm_inttoptr(struct parameters);
    static void llvm_bitcast(struct parameters);
    static void llvm_addrspacecast(struct parameters);

    /* LLVM Control Operations */
    static void llvm_icmp(struct parameters);
    static void llvm_fcmp(struct parameters);
    static void llvm_phi(struct parameters, std::string prevBB);
    static void llvm_call(struct parameters);
    static void llvm_select(struct parameters);
    static void llvm_vaarg(struct parameters);
    static void llvm_landingpad(struct parameters);

    /* LLVM Vector Operations*/
    static void llvm_extractelement(struct parameters);
    static void llvm_insertelement(struct parameters);
    static void llvm_shufflevector(struct parameters);

    /* LLVM Aggregate Operations */
    static void llvm_extractvalue(struct parameters);
    static void llvm_insertvalue(struct parameters);

    /* Custome Operations (Not implemented yet) */
    static void llvm_dmafence(struct parameters);
    static void llvm_dmastore(struct parameters);
    static void llvm_dmaload(struct parameters);
    static void llvm_indexadd(struct parameters);
    static void llvm_silentstore(struct parameters);
    static void llvm_sine(struct parameters);
    static void llvm_cosine(struct parameters);
};

#endif //__OPERATIONS_HH__
