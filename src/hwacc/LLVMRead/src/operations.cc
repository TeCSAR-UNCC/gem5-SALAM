#include "operations.hh"

/* Operations::llvm Terminator Instructions */
void Operations::llvm_move(struct parameters){
    //Not in current Operations::llvm reference. memmove perhaps?
}
void Operations::llvm_ret(struct parameters){ 
    //Operation of device is finished
}
std::string Operations::llvm_br(struct parameters){
    //Store currentBB as prevBB
    //Target of branch becomes currentBB
    return "";
}
void Operations::llvm_switch(struct parameters){ 
	
}
void Operations::llvm_indirectbr(struct parameters){ 
	
}
void Operations::llvm_invoke(struct parameters){ 
	
}
void Operations::llvm_resume(struct parameters){ 
	
}
void Operations::llvm_unreachable(struct parameters){
	
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Integer Operations*/

void Operations::llvm_add(struct parameters){
    // Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_sub(struct parameters) {

	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_mul(struct parameters) {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_udiv(struct parameters) {
	// Unsigned Division


}
void Operations::llvm_sdiv(struct parameters) {
	// Signed Division


}
void Operations::llvm_urem(struct parameters) {
	//Unsigned modulo division


}
void Operations::llvm_srem(struct parameters) {
	//Signed modulo division


}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Floating Point Operations*/
	/* Data Types: half, float, double, fp128, x86_fp80, ppc_fp128*/

void Operations::llvm_fadd(struct parameters){
    // Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result


}
void Operations::llvm_fsub(struct parameters){
	// Floating point Subtraction
	// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
void Operations::llvm_fmul(struct parameters){
	// Floating point Multiplication
	// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
void Operations::llvm_fdiv(struct parameters){
	// Floating point Division
	// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result



}
void Operations::llvm_frem(struct parameters){
    // Floating point modulo division
	// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Bitwise Binary Operations */

void Operations::llvm_shl(struct parameters){
	// Shift Left Operation
	// <result> = shl <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_lshr(struct parameters){
	// Logical Shift Right Operation
	// <result> = lshr <ty> <op1>, <op2>; yields ty : result
	// <result> = lshr exact <ty> <op1>, <op2>; yields ty : result



}
void Operations::llvm_ashr(struct parameters){
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result



}
void Operations::llvm_and(struct parameters){
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result

	
}
void Operations::llvm_or(struct parameters){
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_xor(struct parameters){
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result


}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Memory Access Operations */

void Operations::llvm_alloca(struct parameters){
	// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result
	// C++ Code - <type>* result = new <ty>[NumElements]
	// Unsure how align will be used here if not == sizeOf(<type>)/8

	
}
void Operations::llvm_load(struct parameters){
	// <result> = load [volatile] <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>]
	// address from which to load
	
}
void Operations::llvm_store(struct parameters){ 
	// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void Operations::}
	// Value to store, address at which to store it
}
void Operations:: Operations::llvm_getelementptr(struct parameters){
	// <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
	// The getelementptr instruction is used to get the address of a subelement of an aggregate data structure.
	// It performs address calculation only and does not access memory.
	//
	// Example
	//  %6 contains a pointer to an array of some type (Call it array[])
	//  %5 contains some integer value (In this example %5 = 1)
	// 	%arrayidx = getelementptr inbounds i32* %6, i32 %5
	//  %arrayidx = &%6[%5] or array[1] 
	//  
	// Note:
	//  Because of the syntax of LLVM even if the first element is not an
	//  array or vector it is still referenced in this way
	// More Detailed Example:
	/*
	// C - Code
	struct RT {
	char A;
	int B[10][20];
	char C;
	};
	struct ST {
	int X;
	double Y;
	struct RT Z;
	};

	int *foo(struct ST *s) {
	return &s[1].Z.B[5][13];
	}

	LLVM - Code
	%struct.RT = type { i8, [10 x [20 x i32]], i8 }
	%struct.ST = type { i32, double, %struct.RT }

	define i32* @foo(%struct.ST* %s) nounwind uwtable readnone optsize ssp {
	entry:
	%arrayidx = getelementptr inbounds %struct.ST* %s, i64 1, i32 2, i32 1, i64 5, i64 13
	ret i32* %arrayidx
	}
	
	*/

	
}
void Operations::llvm_fence(struct parameters){ }
void Operations::llvm_cmpxchg(struct parameters){ }
void Operations::llvm_atomicrmw(struct parameters){ }

/* Operations::llvm Conversion Operations */
void Operations::llvm_trunc(struct parameters){ }
void Operations::llvm_zext(struct parameters){ }
void Operations::llvm_sext(struct parameters){ }
void Operations::llvm_fptoui(struct parameters){ }
void Operations::llvm_fptosi(struct parameters){ }
void Operations::llvm_uitofp(struct parameters){ }
void Operations::llvm_sitofp(struct parameters){ }
void Operations::llvm_fptrunc(struct parameters){ }
void Operations::llvm_fpext(struct parameters){ }
void Operations::llvm_ptrtoint(struct parameters){ }
void Operations::llvm_inttoptr(struct parameters){ }
void Operations::llvm_bitcast(struct parameters){ }
void Operations::llvm_addrspacecast(struct parameters){ }

/* Operations::llvm Control Operations */
void Operations::llvm_icmp(struct parameters){ }
void Operations::llvm_fcmp(struct parameters){ }
void Operations::llvm_phi(struct parameters, std::string prevBB){ }
void Operations::llvm_call(struct parameters){ }
void Operations::llvm_select(struct parameters){ }
void Operations::llvm_vaarg(struct parameters){ }
void Operations::llvm_landingpad(struct parameters){ }

/* Operations::llvm Vector Operations*/
void Operations::llvm_extractelement(struct parameters){ }
void Operations::llvm_insertelement(struct parameters){ }
void Operations::llvm_shufflevector(struct parameters){ }

/* Operations::llvm Aggregate Operations */
void Operations::llvm_extractvalue(struct parameters){ }
void Operations::llvm_insertvalue(struct parameters){ }
