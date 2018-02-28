#include "operations.hh"



/* Operations::llvm Terminator Instructions */
void Operations::llvm_move(const struct Attributes& attributes){
    //Not in current Operations::llvm reference. memmove perhaps?
}
void Operations::llvm_ret(const struct Attributes& attributes){
    //Operation of device is finished
}
std::string Operations::llvm_br(const struct Attributes& attributes){
    //Store currentBB as prevBB
    //Target of branch becomes currentBB
    return "";
}
void Operations::llvm_switch(const struct Attributes&  attributes){
	
}
void Operations::llvm_indirectbr(const struct Attributes&  attributes){
	
}
void Operations::llvm_invoke(const struct Attributes&  attributes){
	
}
void Operations::llvm_resume(const struct Attributes&  attributes){
	
}
void Operations::llvm_unreachable(const struct Attributes&  attributes){
	
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Integer Operations*/

void Operations::llvm_add(const struct Attributes&  attributes){
    // Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_sub(const struct Attributes&  attributes) {

	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_mul(const struct Attributes&  attributes) {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_udiv(const struct Attributes&  attributes) {
	// Unsigned Division


}
void Operations::llvm_sdiv(const struct Attributes&  attributes) {
	// Signed Division


}
void Operations::llvm_urem(const struct Attributes&  attributes) {
	//Unsigned modulo division


}
void Operations::llvm_srem(const struct Attributes&  attributes) {
	//Signed modulo division


}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Floating Point Operations*/
	/* Data Types: half, float, double, fp128, x86_fp80, ppc_fp128*/

void Operations::llvm_fadd(const struct Attributes&  attributes){
    // Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result


}
void Operations::llvm_fsub(const struct Attributes&  attributes){
	// Floating point Subtraction
	// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
void Operations::llvm_fmul(const struct Attributes&  attributes){
	// Floating point Multiplication
	// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
void Operations::llvm_fdiv(const struct Attributes&  attributes){
	// Floating point Division
	// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result



}
void Operations::llvm_frem(const struct Attributes&  attributes){
    // Floating point modulo division
	// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Bitwise Binary Operations */

void Operations::llvm_shl(const struct Attributes&  attributes){
	// Shift Left Operation
	// <result> = shl <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_lshr(const struct Attributes&  attributes){
	// Logical Shift Right Operation
	// <result> = lshr <ty> <op1>, <op2>; yields ty : result
	// <result> = lshr exact <ty> <op1>, <op2>; yields ty : result



}
void Operations::llvm_ashr(const struct Attributes&  attributes){
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result



}
void Operations::llvm_and(const struct Attributes&  attributes){
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result

	
}
void Operations::llvm_or(const struct Attributes&  attributes){
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_xor(const struct Attributes&  attributes){
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result


}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Memory Access Operations */

void Operations::llvm_alloca(const struct Attributes&  attributes){
	// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result
	// C++ Code - <type>* result = new <ty>[NumElements]
	// Unsure how align will be used here if not == sizeOf(<type>)/8

	
}
void Operations::llvm_load(const struct Attributes&  attributes){
	// <result> = load [volatile] <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>]
	// address from which to load
	
}
void Operations::llvm_store(const struct Attributes&  attributes){
	// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void Operations::}
	// Value to store, address at which to store it
}
void Operations::llvm_getelementptr(const struct Attributes&  attributes){
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
	const struct Attributes&  RT {
	char A;
	int B[10][20];
	char C;
	};
	const struct Attributes&  ST {
	int X;
	double Y;
	const struct Attributes&  RT Z;
	};

	int *foo(const struct Attributes&  ST *s) {
	return &s[1].Z.B[5][13];
	}

	LLVM - Code
	%const struct Attributes& .RT = type { i8, [10 x [20 x i32]], i8 }
	%const struct Attributes& .ST = type { i32, double, %const struct Attributes& .RT }

	define i32* @foo(%const struct Attributes& .ST* %s) nounwind uwtable readnone optsize ssp {
	entry:
	%arrayidx = getelementptr inbounds %const struct Attributes& .ST* %s, i64 1, i32 2, i32 1, i64 5, i64 13
	ret i32* %arrayidx
	}
	
	*/

	
}
void Operations::llvm_fence(const struct Attributes&  attributes){ }
void Operations::llvm_cmpxchg(const struct Attributes&  attributes){ }
void Operations::llvm_atomicrmw(const struct Attributes&  attributes){ }

/* Operations::llvm Conversion Operations */
void Operations::llvm_trunc(const struct Attributes&  attributes){ }
void Operations::llvm_zext(const struct Attributes&  attributes){ }
void Operations::llvm_sext(const struct Attributes&  attributes){ }
void Operations::llvm_fptoui(const struct Attributes&  attributes){ }
void Operations::llvm_fptosi(const struct Attributes&  attributes){ }
void Operations::llvm_uitofp(const struct Attributes&  attributes){ }
void Operations::llvm_sitofp(const struct Attributes&  attributes){ }
void Operations::llvm_fptrunc(const struct Attributes&  attributes){ }
void Operations::llvm_fpext(const struct Attributes&  attributes){ }
void Operations::llvm_ptrtoint(const struct Attributes&  attributes){ }
void Operations::llvm_inttoptr(const struct Attributes&  attributes){ }
void Operations::llvm_bitcast(const struct Attributes&  attributes){ }
void Operations::llvm_addrspacecast(const struct Attributes&  attributes){ }

/* Operations::llvm Control Operations */
void Operations::llvm_icmp(const struct Attributes&  attributes){ }
void Operations::llvm_fcmp(const struct Attributes&  attributes){ }
void Operations::llvm_phi(const struct Attributes&  attributes, std::string prevBB){ }
void Operations::llvm_call(const struct Attributes&  attributes){ }
void Operations::llvm_select(const struct Attributes&  attributes){ }
void Operations::llvm_vaarg(const struct Attributes&  attributes){ }
void Operations::llvm_landingpad(const struct Attributes&  attributes){ }

/* Operations::llvm Vector Operations*/
void Operations::llvm_extractelement(const struct Attributes&  attributes){ }
void Operations::llvm_insertelement(const struct Attributes&  attributes){ }
void Operations::llvm_shufflevector(const struct Attributes&  attributes){ }

/* Operations::llvm Aggregate Operations */
void Operations::llvm_extractvalue(const struct Attributes&  attributes){ }
void Operations::llvm_insertvalue(const struct Attributes&  attributes){ }

void Operations::llvm_dmafence(const struct Attributes& attributes) { }
void Operations::llvm_dmastore(const struct Attributes& attributes) { }
void Operations::llvm_dmaload(const struct Attributes& attributes) { }
void Operations::llvm_indexadd(const struct Attributes& attributes){ }
void Operations::llvm_silentstore(const struct Attributes& attributes) { }
void Operations::llvm_sine(const struct Attributes& attributes) { }
void Operations::llvm_cosine(const struct Attributes& attributes) { }