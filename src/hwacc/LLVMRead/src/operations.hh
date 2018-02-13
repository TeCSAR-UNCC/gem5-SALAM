#ifndef __OPERATIONS_HH__

#define __OPERATIONS_HH__

#include <string>
#include <map>
//#include <cstdint>
#include <sstream>
#include <vector>
#include <cmath>
//#include "base/types.hh"

#define DEFAULT -1

struct Parameters{
	std::string returnReg, operand1, operand2, dataType;
	
	
};

class Operations {
  public:  
    /* LLVM Terminator Instructions */
     void llvm_move(Parameters &&parameters);
     void llvm_ret(Parameters &parameters);
     std::string llvm_br(Parameters &parameters);
     void llvm_switch(Parameters &parameters);
     void llvm_indirectbr(Parameters &parameters);
     void llvm_invoke(Parameters &parameters);
     void llvm_resume(Parameters &parameters);
     void llvm_unreachable(Parameters &parameters);

    /* LLVM Binary Operations*/
     void llvm_add(Parameters &parameters);
     void llvm_fadd(Parameters &parameters);
     void llvm_sub(Parameters &parameters);
     void llvm_fsub(Parameters &parameters);
     void llvm_mul(Parameters &parameters);
     void llvm_fmul(Parameters &parameters);
     void llvm_udiv(Parameters &parameters);
     void llvm_sdiv(Parameters &parameters);
     void llvm_fdiv(Parameters &parameters);
     void llvm_urem(Parameters &parameters);
     void llvm_srem(Parameters &parameters);
     void llvm_frem(Parameters &parameters);

    /* LLVM Bitwise Binary Operations */
     void llvm_shl(Parameters &parameters);
     void llvm_lshr(Parameters &parameters);
     void llvm_ashr(Parameters &parameters);
     void llvm_and(Parameters &parameters);
     void llvm_or(Parameters &parameters);
     void llvm_xor(Parameters &parameters);

    /* LLVM Memory Access Operations */
     void llvm_alloca(Parameters &parameters);
     void llvm_load(Parameters &parameters);
     void llvm_store(Parameters &parameters);
     void llvm_getelementptr(Parameters &parameters);
     void llvm_fence(Parameters &parameters);
     void llvm_cmpxchg(Parameters &parameters);
     void llvm_atomicrmw(Parameters &parameters);

    /* LLVM Conversion Operations */
     void llvm_trunc(Parameters &parameters);
     void llvm_zext(Parameters &parameters);
     void llvm_sext(Parameters &parameters);
     void llvm_fptoui(Parameters &parameters);
     void llvm_fptosi(Parameters &parameters);
     void llvm_uitofp(Parameters &parameters);
     void llvm_sitofp(Parameters &parameters);
     void llvm_fptrunc(Parameters &parameters);
     void llvm_fpext(Parameters &parameters);
     void llvm_ptrtoint(Parameters &parameters);
     void llvm_inttoptr(Parameters &parameters);
     void llvm_bitcast(Parameters &parameters);
     void llvm_addrspacecast(Parameters &parameters);

    /* LLVM Control Operations */
     void llvm_icmp(Parameters &parameters);
     void llvm_fcmp(Parameters &parameters);
     void llvm_phi(Parameters &parameters, std::string prevBB);
     void llvm_call(Parameters &parameters);
     void llvm_select(Parameters &parameters);
     void llvm_vaarg(Parameters &parameters);
     void llvm_landingpad(Parameters &parameters);

    /* LLVM Vector Operations*/
     void llvm_extractelement(Parameters &parameters);
     void llvm_insertelement(Parameters &parameters);
     void llvm_shufflevector(Parameters &parameters);

    /* LLVM Aggregate Operations */
     void llvm_extractvalue(Parameters &parameters);
     void llvm_insertvalue(Parameters &parameters);

    /* Custome Operations (Not implemented yet) */
     void llvm_dmafence(Parameters &parameters);
     void llvm_dmastore(Parameters &parameters);
     void llvm_dmaload(Parameters &parameters);
     void llvm_indexadd(Parameters &parameters);
     void llvm_silentstore(Parameters &parameters);
     void llvm_sine(Parameters &parameters);
     void llvm_cosine(Parameters &parameters);
};

#endif //__OPERATIONS_HH__
