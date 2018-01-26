#include "operations.hh"

/* Operations::llvm Terminator Instructions */
void Operations::llvm_move(std::string params){
    //Not in current Operations::llvm reference. memmove perhaps?
}
void Operations::llvm_ret(std::string params){ 
    //Operation of device is finished
}
std::string Operations::llvm_br(std::string params){
    //Store currentBB as prevBB
    //Target of branch becomes currentBB
    return "";
}
void Operations::llvm_switch(std::string params){ }
void Operations::llvm_indirectbr(std::string params){ }
void Operations::llvm_invoke(std::string params){ }
void Operations::llvm_resume(std::string params){ }
void Operations::llvm_unreachable(std::string params){ }

/* Operations::llvm Binary Operations*/
uint64_t Operations::llvm_add(std::string params){
    //Integer Addition
    return 0;
}
uint64_t Operations::llvm_fadd(std::string params){
    //Floating point Addition
    return 0;
}
uint64_t Operations::llvm_sub(std::string params){
    //Subtraction
    return 0;
}
uint64_t Operations::llvm_fsub(std::string params){
    //Floating Point Subtraction
    return 0;
}
uint64_t Operations::llvm_mul(std::string params){
    //Multiplication
    return 0;
}
uint64_t Operations::llvm_fmul(std::string params){
    //Floating Point Multiplication
    return 0;
}
uint64_t Operations::llvm_udiv(std::string params){
    //Unsigned Division
    return 0;
}
uint64_t Operations::llvm_sdiv(std::string params){
    //Signed Division
    return 0;
}
uint64_t Operations::llvm_fdiv(std::string params){
    //Floating Point Division
    return 0;
}
uint64_t Operations::llvm_urem(std::string params){
    //Unsigned modulo division
    return 0;
}
uint64_t Operations::llvm_srem(std::string params){
    //Signed modulo division
    return 0;
}
uint64_t Operations::llvm_frem(std::string params){
    //Floating point modulo division - Apparently this exists
    return 0;
}

/* Operations::llvm Bitwise Binary Operations */
uint64_t Operations::llvm_shl(std::string params){ return 0;}
uint64_t Operations::llvm_lshr(std::string params){ return 0;}
uint64_t Operations::llvm_ashr(std::string params){ return 0;}
uint64_t Operations::llvm_and(std::string params){ return 0;}
uint64_t Operations::llvm_or(std::string params){ return 0;}
uint64_t Operations::llvm_xor(std::string params){ return 0;}

/* Operations::llvm Memory Access Operations */
uint64_t Operations::llvm_alloca(std::string params){ return 0;}
uint64_t Operations::llvm_load(std::string params){ return 0;}
void Operations::llvm_store(std::string params){ }
uint64_t Operations::llvm_getelementptr(std::string params){ return 0;}
void Operations::llvm_fence(std::string params){ }
uint64_t Operations::llvm_cmpxchg(std::string params){ return 0;}
uint64_t Operations::llvm_atomicrmw(std::string params){ return 0;}

/* Operations::llvm Conversion Operations */
uint64_t Operations::llvm_trunc(std::string params){ return 0;}
uint64_t Operations::llvm_zext(std::string params){ return 0;}
uint64_t Operations::llvm_sext(std::string params){ return 0;}
uint64_t Operations::llvm_fptoui(std::string params){ return 0;}
uint64_t Operations::llvm_fptosi(std::string params){ return 0;}
uint64_t Operations::llvm_uitofp(std::string params){ return 0;}
uint64_t Operations::llvm_sitofp(std::string params){ return 0;}
uint64_t Operations::llvm_fptrunc(std::string params){ return 0;}
uint64_t Operations::llvm_fpext(std::string params){ return 0;}
uint64_t Operations::llvm_ptrtoint(std::string params){ return 0;}
uint64_t Operations::llvm_inttoptr(std::string params){ return 0;}
uint64_t Operations::llvm_bitcast(std::string params){ return 0;}
uint64_t Operations::llvm_addrspacecast(std::string params){ return 0;}

/* Operations::llvm Control Operations */
uint64_t Operations::llvm_icmp(std::string params){ return 0;}
uint64_t Operations::llvm_fcmp(std::string params){ return 0;}
uint64_t Operations::llvm_phi(std::string params, std::string prevBB){ return 0;}
uint64_t Operations::llvm_call(std::string params){ return 0;}
uint64_t Operations::llvm_select(std::string params){ return 0;}
uint64_t Operations::llvm_vaarg(std::string params){ return 0;}
uint64_t Operations::llvm_landingpad(std::string params){ return 0;}

/* Operations::llvm Vector Operations*/
uint64_t Operations::llvm_extractelement(std::string params){ return 0;}
uint64_t Operations::llvm_insertelement(std::string params){ return 0;}
uint64_t Operations::llvm_shufflevector(std::string params){ return 0;}

/* Operations::llvm Aggregate Operations */
uint64_t Operations::llvm_extractvalue(std::string params){ return 0;}
uint64_t Operations::llvm_insertvalue(std::string params){ return 0;}
