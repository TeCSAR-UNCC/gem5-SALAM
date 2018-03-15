#ifndef __OPERATIONS_HH__

#define __OPERATIONS_HH__

#include <string>
#include "compute_node.hh"
#include "instruction.hh"


class Operations {
  public:  
	 
    /* LLVM Terminator Instructions */
      static void llvm_move(const struct Attributes& attributes);
      static void llvm_ret(const struct Attributes& attributes);
      static std::string llvm_br(const struct Attributes& attributes);
      static void llvm_switch(const struct Attributes& attributes);
      static void llvm_indirectbr(const struct Attributes& attributes);
      static void llvm_invoke(const struct Attributes& attributes);
      static void llvm_resume(const struct Attributes& attributes);
      static void llvm_unreachable(const struct Attributes& attributes);

    /* LLVM Binary Operations*/
      static void llvm_add(const struct Attributes& attributes);
      static void llvm_fadd(const struct Attributes& attributes);
      static void llvm_sub(const struct Attributes& attributes);
      static void llvm_fsub(const struct Attributes& attributes);
      static void llvm_mul(const struct Attributes& attributes);
      static void llvm_fmul(const struct Attributes& attributes);
      static void llvm_udiv(const struct Attributes& attributes);
      static void llvm_sdiv(const struct Attributes& attributes);
      static void llvm_fdiv(const struct Attributes& attributes);
      static void llvm_urem(const struct Attributes& attributes);
      static void llvm_srem(const struct Attributes& attributes);
      static void llvm_frem(const struct Attributes& attributes);

    /* LLVM Bitwise Binary Operations */
      static void llvm_shl(const struct Attributes& attributes);
      static void llvm_lshr(const struct Attributes& attributes);
      static void llvm_ashr(const struct Attributes& attributes);
      static void llvm_and(const struct Attributes& attributes);
      static void llvm_or(const struct Attributes& attributes);
      static void llvm_xor(const struct Attributes& attributes);

    /* LLVM Memory Access Operations */
      static void llvm_alloca(const struct Attributes& attributes);
      static void llvm_load(const struct Attributes& attributes);
      static void llvm_store(const struct Attributes& attributes);
      static void llvm_getelementptr(const struct Attributes& attributes);
      static void llvm_fence(const struct Attributes& attributes);
      static void llvm_cmpxchg(const struct Attributes& attributes);
      static void llvm_atomicrmw(const struct Attributes& attributes);

    /* LLVM Conversion Operations */
      static void llvm_trunc(const struct Attributes& attributes);
      static void llvm_zext(const struct Attributes& attributes);
      static void llvm_sext(const struct Attributes& attributes);
      static void llvm_fptoui(const struct Attributes& attributes);
      static void llvm_fptosi(const struct Attributes& attributes);
      static void llvm_uitofp(const struct Attributes& attributes);
      static void llvm_sitofp(const struct Attributes& attributes);
      static void llvm_fptrunc(const struct Attributes& attributes);
      static void llvm_fpext(const struct Attributes& attributes);
      static void llvm_ptrtoint(const struct Attributes& attributes);
      static void llvm_inttoptr(const struct Attributes& attributes);
      static void llvm_bitcast(const struct Attributes& attributes);
      static void llvm_addrspacecast(const struct Attributes& attributes);

    /* LLVM Control Operations */
      static void llvm_icmp(const struct Attributes& attributes);
      static void llvm_fcmp(const struct Attributes& attributes);
      static void llvm_phi(const struct Attributes& attributes, std::string prevBB);
      static void llvm_call(const struct Attributes& attributes);
      static void llvm_select(const struct Attributes& attributes);
      static void llvm_vaarg(const struct Attributes& attributes);
      static void llvm_landingpad(const struct Attributes& attributes);

    /* LLVM Vector Operations*/
      static void llvm_extractelement(const struct Attributes& attributes);
      static void llvm_insertelement(const struct Attributes& attributes);
      static void llvm_shufflevector(const struct Attributes& attributes);

    /* LLVM Aggregate Operations */
      static void llvm_extractvalue(const struct Attributes& attributes);
      static void llvm_insertvalue(const struct Attributes& attributes);

    /* Custome Operations (Not implemented yet) */
      static void llvm_dmafence(const struct Attributes& attributes);
      static void llvm_dmastore(const struct Attributes& attributes);
      static void llvm_dmaload(const struct Attributes& attributes);
      static void llvm_indexadd(const struct Attributes& attributes);
      static void llvm_silentstore(const struct Attributes& attributes);
      static void llvm_sine(const struct Attributes& attributes);
      static void llvm_cosine(const struct Attributes& attributes);
};

#endif //__OPERATIONS_HH__
