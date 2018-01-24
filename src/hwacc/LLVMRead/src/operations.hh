#ifndef __OPERATIONS_HH__

#define __OPERATIONS_HH__

#include <string>
#include <map>
#include <cstdint>
//#include "base/types.hh"

class Operations {
  private:  
    enum opValue {
      evNotDefined,
      LLVM_IR_Move,
      LLVM_IR_Ret,
      LLVM_IR_Br,
      LLVM_IR_Switch,
      LLVM_IR_IndirectBr,
      LLVM_IR_Invoke,
      LLVM_IR_Resume,
      LLVM_IR_Unreachable,
      LLVM_IR_Add,
      LLVM_IR_FAdd,
      LLVM_IR_Sub,
      LLVM_IR_FSub,
      LLVM_IR_Mul,
      LLVM_IR_FMul,
      LLVM_IR_UDiv,
      LLVM_IR_SDiv,
      LLVM_IR_FDiv,
      LLVM_IR_URem,
      LLVM_IR_SRem,
      LLVM_IR_FRem,
      LLVM_IR_Shl,
      LLVM_IR_LShr,
      LLVM_IR_AShr,
      LLVM_IR_And,
      LLVM_IR_Or,
      LLVM_IR_Xor,
      LLVM_IR_Alloca,
      LLVM_IR_Load,
      LLVM_IR_Store,
      LLVM_IR_GetElementPtr,
      LLVM_IR_Fence,
      LLVM_IR_AtomicCmpXchg,
      LLVM_IR_AtomicRMW,
      LLVM_IR_Trunc,
      LLVM_IR_ZExt,
      LLVM_IR_SExt,
      LLVM_IR_FPToUI,
      LLVM_IR_FPToSI,
      LLVM_IR_UIToFP,
      LLVM_IR_SIToFP,
      LLVM_IR_FPTrunc,
      LLVM_IR_FPExt,
      LLVM_IR_PtrToInt,
      LLVM_IR_IntToPtr,
      LLVM_IR_BitCast,
      LLVM_IR_AddrSpaceCast,
      LLVM_IR_ICmp,
      LLVM_IR_FCmp,
      LLVM_IR_PHI,
      LLVM_IR_Call,
      LLVM_IR_Select,
      LLVM_IR_VAArg,
      LLVM_IR_ExtractElement,
      LLVM_IR_InsertElement,
      LLVM_IR_ShuffleVector,
      LLVM_IR_ExtractValue,
      LLVM_IR_InsertValue,
      LLVM_IR_LandingPad,
      LLVM_IR_DMAFence,
      LLVM_IR_DMAStore,
      LLVM_IR_DMALoad,
      LLVM_IR_IndexAdd,
      LLVM_IR_SilentStore,
      LLVM_IR_Sine,
      LLVM_IR_Cosine
    };

    static std::map<std::string, opValue> s_opMap;
  public:
    static void initMap() {
      s_opMap["move"] = LLVM_IR_Move;
      s_opMap["ret"] = LLVM_IR_Ret;
      s_opMap["br"] = LLVM_IR_Br;
      s_opMap["switch"] = LLVM_IR_Switch;
      s_opMap["indirectbr"] = LLVM_IR_IndirectBr;
      s_opMap["invoke"] = LLVM_IR_Invoke;
      s_opMap["resume"] = LLVM_IR_Resume;
      s_opMap["unreachable"] = LLVM_IR_Unreachable;
      s_opMap["add"] = LLVM_IR_Add;
      s_opMap["fadd"] = LLVM_IR_FAdd;
      s_opMap["sub"] = LLVM_IR_Sub;
      s_opMap["fsub"] = LLVM_IR_FSub;
      s_opMap["mul"] = LLVM_IR_Mul;
      s_opMap["fmul"] = LLVM_IR_FMul;
      s_opMap["udiv"] = LLVM_IR_UDiv;
      s_opMap["sdiv"] = LLVM_IR_SDiv;
      s_opMap["fdiv"] = LLVM_IR_FDiv;
      s_opMap["urem"] = LLVM_IR_URem;
      s_opMap["srem"] = LLVM_IR_SRem;
      s_opMap["frem"] = LLVM_IR_FRem;
      s_opMap["shl"] = LLVM_IR_Shl;
      s_opMap["lshr"] = LLVM_IR_LShr;
      s_opMap["ashr"] = LLVM_IR_AShr;
      s_opMap["and"] = LLVM_IR_And;
      s_opMap["or"] = LLVM_IR_Or;
      s_opMap["xor"] = LLVM_IR_Xor;
      s_opMap["alloca"] = LLVM_IR_Alloca;
      s_opMap["load"] = LLVM_IR_Load;
      s_opMap["store"] = LLVM_IR_Store;
      s_opMap["getelementptr"] = LLVM_IR_GetElementPtr;
      s_opMap["fence"] = LLVM_IR_Fence;
      s_opMap["cmpxchg"] = LLVM_IR_AtomicCmpXchg;
      s_opMap["atomicrmw"] = LLVM_IR_AtomicRMW;
      s_opMap["trunc"] = LLVM_IR_Trunc;
      s_opMap["zext"] = LLVM_IR_ZExt;
      s_opMap["sext"] = LLVM_IR_SExt;
      s_opMap["fptoui"] = LLVM_IR_FPToUI;
      s_opMap["fptosi"] = LLVM_IR_FPToSI;
      s_opMap["uitofp"] = LLVM_IR_UIToFP;
      s_opMap["sitofp"] = LLVM_IR_SIToFP;
      s_opMap["fptrunc"] = LLVM_IR_FPTrunc;
      s_opMap["fpext"] = LLVM_IR_FPExt;
      s_opMap["ptrtoint"] = LLVM_IR_PtrToInt;
      s_opMap["inttoptr"] = LLVM_IR_IntToPtr;
      s_opMap["bitcast"] = LLVM_IR_BitCast;
      s_opMap["addrspacecast"] = LLVM_IR_AddrSpaceCast;
      s_opMap["icmp"] = LLVM_IR_ICmp;
      s_opMap["fcmp"] = LLVM_IR_FCmp;
      s_opMap["phi"] = LLVM_IR_PHI;
      s_opMap["call"] = LLVM_IR_Call;
      s_opMap["select"] = LLVM_IR_Select;
      s_opMap["va_arg"] = LLVM_IR_VAArg;
      s_opMap["extractelement"] = LLVM_IR_ExtractElement;
      s_opMap["insertelement"] = LLVM_IR_InsertElement;
      s_opMap["shufflevector"] = LLVM_IR_ShuffleVector;
      s_opMap["extractvalue"] = LLVM_IR_ExtractValue;
      s_opMap["insertvalue"] = LLVM_IR_InsertValue;
      s_opMap["landingpad"] = LLVM_IR_LandingPad;
      s_opMap["dmafence"] = LLVM_IR_DMAFence;
      s_opMap["dmastore"] = LLVM_IR_DMAStore;
      s_opMap["dmaload"] = LLVM_IR_DMALoad;
      s_opMap["indexadd"] = LLVM_IR_IndexAdd;
      s_opMap["silentstore"] = LLVM_IR_SilentStore;
      s_opMap["sine"] = LLVM_IR_Sine;
      s_opMap["cosine"] = LLVM_IR_Cosine;
    }

    /* LLVM Terminator Instructions */
    static void llvm_move(std::string params);
    static void llvm_ret(std::string params);
    static std::string llvm_br(std::string params);
    static void llvm_switch(std::string params);
    static void llvm_indirectbr(std::string params);
    static void llvm_invoke(std::string params);
    static void llvm_resume(std::string params);
    static void llvm_unreachable(std::string params);

    /* LLVM Binary Operations*/
    static uint64_t llvm_add(std::string params);
    static uint64_t llvm_fadd(std::string params);
    static uint64_t llvm_sub(std::string params);
    static uint64_t llvm_fsub(std::string params);
    static uint64_t llvm_mul(std::string params);
    static uint64_t llvm_fmul(std::string params);
    static uint64_t llvm_udiv(std::string params);
    static uint64_t llvm_sdiv(std::string params);
    static uint64_t llvm_fdiv(std::string params);
    static uint64_t llvm_urem(std::string params);
    static uint64_t llvm_srem(std::string params);
    static uint64_t llvm_frem(std::string params);

    /* LLVM Bitwise Binary Operations */
    static uint64_t llvm_shl(std::string params);
    static uint64_t llvm_lshr(std::string params);
    static uint64_t llvm_ashr(std::string params);
    static uint64_t llvm_and(std::string params);
    static uint64_t llvm_or(std::string params);
    static uint64_t llvm_xor(std::string params);

    /* LLVM Memory Access Operations */
    static uint64_t llvm_alloca(std::string params);
    static uint64_t llvm_load(std::string params);
    static void llvm_store(std::string params);
    static uint64_t llvm_getelementptr(std::string params);
    static void llvm_fence(std::string params);
    static uint64_t llvm_cmpxchg(std::string params);
    static uint64_t llvm_atomicrmw(std::string params);

    /* LLVM Conversion Operations */
    static uint64_t llvm_trunc(std::string params);
    static uint64_t llvm_zext(std::string params);
    static uint64_t llvm_sext(std::string params);
    static uint64_t llvm_fptoui(std::string params);
    static uint64_t llvm_fptosi(std::string params);
    static uint64_t llvm_uitofp(std::string params);
    static uint64_t llvm_sitofp(std::string params);
    static uint64_t llvm_fptrunc(std::string params);
    static uint64_t llvm_fpext(std::string params);
    static uint64_t llvm_ptrtoint(std::string params);
    static uint64_t llvm_inttoptr(std::string params);
    static uint64_t llvm_bitcast(std::string params);
    static uint64_t llvm_addrspacecast(std::string params);

    /* LLVM Control Operations */
    static uint64_t llvm_icmp(std::string params);
    static uint64_t llvm_fcmp(std::string params);
    static uint64_t llvm_phi(std::string params, std::string prevBB);
    static uint64_t llvm_call(std::string params);
    static uint64_t llvm_select(std::string params);
    static uint64_t llvm_vaarg(std::string params);
    static uint64_t llvm_landingpad(std::string params);

    /* LLVM Vector Operations*/
    static uint64_t llvm_extractelement(std::string params);
    static uint64_t llvm_insertelement(std::string params);
    static uint64_t llvm_shufflevector(std::string params);

    /* LLVM Aggregate Operations */
    static uint64_t llvm_extractvalue(std::string params);
    static uint64_t llvm_insertvalue(std::string params);

    /* Custome Operations (Not implemented yet) */
    static uint64_t llvm_dmafence(std::string params);
    static uint64_t llvm_dmastore(std::string params);
    static uint64_t llvm_dmaload(std::string params);
    static uint64_t llvm_indexadd(std::string params);
    static uint64_t llvm_silentstore(std::string params);
    static uint64_t llvm_sine(std::string params);
    static uint64_t llvm_cosine(std::string params);
};

#endif //__OPERATIONS_HH__
