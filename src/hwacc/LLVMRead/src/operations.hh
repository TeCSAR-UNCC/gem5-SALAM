#ifndef __OPERATIONS_HH__

#define __OPERATIONS_HH__

#include <string>
#include "compute_node.hh"
#include "instruction.hh"

class Operations
{
public:
  /* LLVM Terminator Instructions */
  static void llvm_move(const struct Instruction &instruction);
  static void llvm_ret(const struct Instruction &instruction);
  static void llvm_br(const struct Instruction &instruction);
  static void llvm_switch(const struct Instruction &instruction);
  static void llvm_indirectbr(const struct Instruction &instruction);
  static void llvm_invoke(const struct Instruction &instruction);
  static void llvm_resume(const struct Instruction &instruction);
  static void llvm_unreachable(const struct Instruction &instruction);

  /* LLVM Binary Operations*/
  static void llvm_add(const struct Instruction &instruction);
  static void llvm_fadd(const struct Instruction &instruction);
  static void llvm_sub(const struct Instruction &instruction);
  static void llvm_fsub(const struct Instruction &instruction);
  static void llvm_mul(const struct Instruction &instruction);
  static void llvm_fmul(const struct Instruction &instruction);
  static void llvm_udiv(const struct Instruction &instruction);
  static void llvm_sdiv(const struct Instruction &instruction);
  static void llvm_fdiv(const struct Instruction &instruction);
  static void llvm_urem(const struct Instruction &instruction);
  static void llvm_srem(const struct Instruction &instruction);
  static void llvm_frem(const struct Instruction &instruction);

  /* LLVM Bitwise Binary Operations */
  static void llvm_shl(const struct Instruction &instruction);
  static void llvm_lshr(const struct Instruction &instruction);
  static void llvm_ashr(const struct Instruction &instruction);
  static void llvm_and(const struct Instruction &instruction);
  static void llvm_or(const struct Instruction &instruction);
  static void llvm_xor(const struct Instruction &instruction);

  /* LLVM Memory Access Operations */
  static void llvm_alloca(const struct Instruction &instruction);
  static void llvm_load(const struct Instruction &instruction);
  static void llvm_store(const struct Instruction &instruction);
  static void llvm_getelementptr(const struct Instruction &instruction);
  static void llvm_fence(const struct Instruction &instruction);
  static void llvm_cmpxchg(const struct Instruction &instruction);
  static void llvm_atomicrmw(const struct Instruction &instruction);

  /* LLVM Conversion Operations */
  static void llvm_trunc(const struct Instruction &instruction);
  static void llvm_zext(const struct Instruction &instruction);
  static void llvm_sext(const struct Instruction &instruction);
  static void llvm_fptoui(const struct Instruction &instruction);
  static void llvm_fptosi(const struct Instruction &instruction);
  static void llvm_uitofp(const struct Instruction &instruction);
  static void llvm_sitofp(const struct Instruction &instruction);
  static void llvm_fptrunc(const struct Instruction &instruction);
  static void llvm_fpext(const struct Instruction &instruction);
  static void llvm_ptrtoint(const struct Instruction &instruction);
  static void llvm_inttoptr(const struct Instruction &instruction);
  static void llvm_bitcast(const struct Instruction &instruction);
  static void llvm_addrspacecast(const struct Instruction &instruction);

  /* LLVM Control Operations */
  static void llvm_icmp(const struct Instruction &instruction);
  static void llvm_fcmp(const struct Instruction &instruction);
  static void llvm_phi(const struct Instruction &instruction, std::string prevBB);
  static void llvm_call(const struct Instruction &instruction);
  static void llvm_select(const struct Instruction &instruction);
  static void llvm_vaarg(const struct Instruction &instruction);
  static void llvm_landingpad(const struct Instruction &instruction);

  /* LLVM Vector Operations*/
  static void llvm_extractelement(const struct Instruction &instruction);
  static void llvm_insertelement(const struct Instruction &instruction);
  static void llvm_shufflevector(const struct Instruction &instruction);

  /* LLVM Aggregate Operations */
  static void llvm_extractvalue(const struct Instruction &instruction);
  static void llvm_insertvalue(const struct Instruction &instruction);

  /* Custome Operations (Not implemented yet) */
  static void llvm_dmafence(const struct Instruction &instruction);
  static void llvm_dmastore(const struct Instruction &instruction);
  static void llvm_dmaload(const struct Instruction &instruction);
  static void llvm_indexadd(const struct Instruction &instruction);
  static void llvm_silentstore(const struct Instruction &instruction);
  static void llvm_sine(const struct Instruction &instruction);
  static void llvm_cosine(const struct Instruction &instruction);
};

#endif //__OPERATIONS_HH__
