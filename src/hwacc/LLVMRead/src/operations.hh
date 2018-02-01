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
     void llvm_ret(struct parameters);
     std::string llvm_br(struct parameters);
     void llvm_switch(struct parameters);
     void llvm_indirectbr(struct parameters);
     void llvm_invoke(struct parameters);
     void llvm_resume(struct parameters);
     void llvm_unreachable(struct parameters);

    /* LLVM Binary Operations*/
     void llvm_add(struct parameters);
     void llvm_fadd(struct parameters);
     void llvm_sub(struct parameters);
     void llvm_fsub(struct parameters);
     void llvm_mul(struct parameters);
     void llvm_fmul(struct parameters);
     void llvm_udiv(struct parameters);
     void llvm_sdiv(struct parameters);
     void llvm_fdiv(struct parameters);
     void llvm_urem(struct parameters);
     void llvm_srem(struct parameters);
     void llvm_frem(struct parameters);

    /* LLVM Bitwise Binary Operations */
     void llvm_shl(struct parameters);
     void llvm_lshr(struct parameters);
     void llvm_ashr(struct parameters);
     void llvm_and(struct parameters);
     void llvm_or(struct parameters);
     void llvm_xor(struct parameters);

    /* LLVM Memory Access Operations */
     void llvm_alloca(struct parameters);
     void llvm_load(struct parameters);
     void llvm_store(struct parameters);
     void llvm_getelementptr(struct parameters);
     void llvm_fence(struct parameters);
     void llvm_cmpxchg(struct parameters);
     void llvm_atomicrmw(struct parameters);

    /* LLVM Conversion Operations */
     void llvm_trunc(struct parameters);
     void llvm_zext(struct parameters);
     void llvm_sext(struct parameters);
     void llvm_fptoui(struct parameters);
     void llvm_fptosi(struct parameters);
     void llvm_uitofp(struct parameters);
     void llvm_sitofp(struct parameters);
     void llvm_fptrunc(struct parameters);
     void llvm_fpext(struct parameters);
     void llvm_ptrtoint(struct parameters);
     void llvm_inttoptr(struct parameters);
     void llvm_bitcast(struct parameters);
     void llvm_addrspacecast(struct parameters);

    /* LLVM Control Operations */
     void llvm_icmp(struct parameters);
     void llvm_fcmp(struct parameters);
     void llvm_phi(struct parameters, std::string prevBB);
     void llvm_call(struct parameters);
     void llvm_select(struct parameters);
     void llvm_vaarg(struct parameters);
     void llvm_landingpad(struct parameters);

    /* LLVM Vector Operations*/
     void llvm_extractelement(struct parameters);
     void llvm_insertelement(struct parameters);
     void llvm_shufflevector(struct parameters);

    /* LLVM Aggregate Operations */
     void llvm_extractvalue(struct parameters);
     void llvm_insertvalue(struct parameters);

    /* Custome Operations (Not implemented yet) */
     void llvm_dmafence(struct parameters);
     void llvm_dmastore(struct parameters);
     void llvm_dmaload(struct parameters);
     void llvm_indexadd(struct parameters);
     void llvm_silentstore(struct parameters);
     void llvm_sine(struct parameters);
     void llvm_cosine(struct parameters);
};

#endif //__OPERATIONS_HH__
