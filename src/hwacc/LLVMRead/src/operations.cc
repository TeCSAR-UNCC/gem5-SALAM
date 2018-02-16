#include "operations.hh"

/* Operations::llvm Terminator Instructions */
void Operations::llvm_move(Parameters &&parameters){
    //Not in current Operations::llvm reference. memmove perhaps?
}
void Operations::llvm_ret(Parameters &parameters){
    //Operation of device is finished
}
std::string Operations::llvm_br(Parameters &parameters){
    //Store currentBB as prevBB
    //Target of branch becomes currentBB
    return "";
}
void Operations::llvm_switch(Parameters &parameters){
	
}
void Operations::llvm_indirectbr(Parameters &parameters){
	
}
void Operations::llvm_invoke(Parameters &parameters){
	
}
void Operations::llvm_resume(Parameters &parameters){
	
}
void Operations::llvm_unreachable(Parameters &parameters){
	
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Integer Operations*/

void Operations::llvm_add(Parameters &parameters){
    // Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_sub(Parameters &parameters) {

	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_mul(Parameters &parameters) {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_udiv(Parameters &parameters) {
	// Unsigned Division


}
void Operations::llvm_sdiv(Parameters &parameters) {
	// Signed Division


}
void Operations::llvm_urem(Parameters &parameters) {
	//Unsigned modulo division


}
void Operations::llvm_srem(Parameters &parameters) {
	//Signed modulo division


}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Floating Point Operations*/
	/* Data Types: half, float, double, fp128, x86_fp80, ppc_fp128*/

void Operations::llvm_fadd(Parameters &parameters){
    // Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result


}
void Operations::llvm_fsub(Parameters &parameters){
	// Floating point Subtraction
	// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
void Operations::llvm_fmul(Parameters &parameters){
	// Floating point Multiplication
	// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
void Operations::llvm_fdiv(Parameters &parameters){
	// Floating point Division
	// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result



}
void Operations::llvm_frem(Parameters &parameters){
    // Floating point modulo division
	// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	

}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Bitwise Binary Operations */

void Operations::llvm_shl(Parameters &parameters){
	// Shift Left Operation
	// <result> = shl <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_lshr(Parameters &parameters){
	// Logical Shift Right Operation
	// <result> = lshr <ty> <op1>, <op2>; yields ty : result
	// <result> = lshr exact <ty> <op1>, <op2>; yields ty : result



}
void Operations::llvm_ashr(Parameters &parameters){
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result



}
void Operations::llvm_and(Parameters &parameters){
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result

	
}
void Operations::llvm_or(Parameters &parameters){
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result


}
void Operations::llvm_xor(Parameters &parameters){
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result


}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Memory Access Operations */

void Operations::llvm_alloca(Parameters &parameters){
	// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result
	// C++ Code - <type>* result = new <ty>[NumElements]
	// Unsure how align will be used here if not == sizeOf(<type>)/8

	
}
void Operations::llvm_load(Parameters &parameters){
	// <result> = load [volatile] <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>]
	// address from which to load
	
}
void Operations::llvm_store(Parameters &parameters){
	// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void Operations::}
	// Value to store, address at which to store it
}
void Operations:: Operations::llvm_getelementptr(Parameters &parameters){
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
	Parameters RT {
	char A;
	int B[10][20];
	char C;
	};
	Parameters ST {
	int X;
	double Y;
	Parameters RT Z;
	};

	int *foo(Parameters ST *s) {
	return &s[1].Z.B[5][13];
	}

	LLVM - Code
	%Parameters.RT = type { i8, [10 x [20 x i32]], i8 }
	%Parameters.ST = type { i32, double, %Parameters.RT }

	define i32* @foo(%Parameters.ST* %s) nounwind uwtable readnone optsize ssp {
	entry:
	%arrayidx = getelementptr inbounds %Parameters.ST* %s, i64 1, i32 2, i32 1, i64 5, i64 13
	ret i32* %arrayidx
	}
	
	*/

	
}
void Operations::llvm_fence(Parameters &parameters){ }
void Operations::llvm_cmpxchg(Parameters &parameters){ }
void Operations::llvm_atomicrmw(Parameters &parameters){ }

/* Operations::llvm Conversion Operations */
void Operations::llvm_trunc(Parameters &parameters){ }
void Operations::llvm_zext(Parameters &parameters){ }
void Operations::llvm_sext(Parameters &parameters){ }
void Operations::llvm_fptoui(Parameters &parameters){ }
void Operations::llvm_fptosi(Parameters &parameters){ }
void Operations::llvm_uitofp(Parameters &parameters){ }
void Operations::llvm_sitofp(Parameters &parameters){ }
void Operations::llvm_fptrunc(Parameters &parameters){ }
void Operations::llvm_fpext(Parameters &parameters){ }
void Operations::llvm_ptrtoint(Parameters &parameters){ }
void Operations::llvm_inttoptr(Parameters &parameters){ }
void Operations::llvm_bitcast(Parameters &parameters){ }
void Operations::llvm_addrspacecast(Parameters &parameters){ }

/* Operations::llvm Control Operations */
void Operations::llvm_icmp(Parameters &parameters){ }
void Operations::llvm_fcmp(Parameters &parameters){ }
void Operations::llvm_phi(Parameters &parameters, std::string prevBB){ }
void Operations::llvm_call(Parameters &parameters){ }
void Operations::llvm_select(Parameters &parameters){ }
void Operations::llvm_vaarg(Parameters &parameters){ }
void Operations::llvm_landingpad(Parameters &parameters){ }

/* Operations::llvm Vector Operations*/
void Operations::llvm_extractelement(Parameters &parameters){ }
void Operations::llvm_insertelement(Parameters &parameters){ }
void Operations::llvm_shufflevector(Parameters &parameters){ }

/* Operations::llvm Aggregate Operations */
void Operations::llvm_extractvalue(Parameters &parameters){ }
void Operations::llvm_insertvalue(Parameters &parameters){ }
