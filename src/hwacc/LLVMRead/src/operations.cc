#include "operations.hh"



/* Operations::llvm Terminator Instructions */
void Operations::llvm_move(const struct Instruction& instruction){
    //Not in current Operations::llvm reference. memmove perhaps?
}
void Operations::llvm_ret(const struct Instruction& instruction){
    //Operation of device is finished
}
std::string Operations::llvm_br(const struct Instruction& instruction){
    //Store currentBB as prevBB
    //Target of branch becomes currentBB
	//int condition;
	// if(!(instruction.terminator.unconditional)){
	// 	//condition = instruction.terminator.cond->getValue();
		
	// }

    return "";
}
void Operations::llvm_switch(const struct Instruction&  instruction){
	
}
void Operations::llvm_indirectbr(const struct Instruction&  instruction){
	
}
void Operations::llvm_invoke(const struct Instruction&  instruction){
	
}
void Operations::llvm_resume(const struct Instruction&  instruction){
	
}
void Operations::llvm_unreachable(const struct Instruction&  instruction){
	
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Integer Operations*/

void Operations::llvm_add(const struct Instruction&  instruction){
    // Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result

	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 + op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
}
void Operations::llvm_sub(const struct Instruction&  instruction) {

	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result
	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 - op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_mul(const struct Instruction&  instruction) {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result

	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 * op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_udiv(const struct Instruction&  instruction) {
	// Unsigned Division
	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 / op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_sdiv(const struct Instruction&  instruction) {
	// Signed Division
	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 / op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_urem(const struct Instruction&  instruction) {
	//Unsigned modulo division
	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 % op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_srem(const struct Instruction&  instruction) {
	//Signed modulo division
	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 % op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
	/* Floating Point Operations*/
	/* Data Types: half, float, double, fp128, x86_fp80, ppc_fp128*/

void Operations::llvm_fadd(const struct Instruction&  instruction){
    // Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	// Currently floating point fast math flags are not implemented, they are based around
	// how the operation is performed which is not a concern of this function, but are important
	// variables when calculating the time it takes for the computation to complete

	// if(instruction.binary.ty.find("half")){
	// 	// No current implementation of half in c++, must implement an outside code for this functionality
	// }
	
	double op1;
	double op2;
	double result;
	float fresult;
	

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 + op2;

	// Store result back into memory
	if(instruction.binary.ty.find("float")){
	fresult = (float) result;
	memcpy(&instruction.general.returnRegister->value, &fresult, instruction.general.returnRegister->size);
	}
	else 	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);


}
void Operations::llvm_fsub(const struct Instruction&  instruction){
	// Floating point Subtraction
	// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	// Currently floating point fast math flags are not implemented, they are based around
	// how the operation is performed which is not a concern of this function, but are important
	// variables when calculating the time it takes for the computation to complete

	// if(instruction.binary.ty.find("half")){
	// 	// No current implementation of half in c++, must implement an outside code for this functionality
	// }
	
	double op1;
	double op2;
	double result;
	float fresult;
	

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 - op2;

	// Store result back into memory
	if(instruction.binary.ty.find("float")){
	fresult = (float) result;
	memcpy(&instruction.general.returnRegister->value, &fresult, instruction.general.returnRegister->size);
	}
	else 	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
}
void Operations::llvm_fmul(const struct Instruction&  instruction){
	// Floating point Multiplication
	// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	// Currently floating point fast math flags are not implemented, they are based around
	// how the operation is performed which is not a concern of this function, but are important
	// variables when calculating the time it takes for the computation to complete

	// if(instruction.binary.ty.find("half")){
	// 	// No current implementation of half in c++, must implement an outside code for this functionality
	// }
	double op1;
	double op2;
	double result;
	float fresult;
	

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 * op2;

	// Store result back into memory
	if(instruction.binary.ty.find("float")){
	fresult = (float) result;
	memcpy(&instruction.general.returnRegister->value, &fresult, instruction.general.returnRegister->size);
	}
	else 	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
}
void Operations::llvm_fdiv(const struct Instruction&  instruction){
	// Floating point Division
	// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	// Currently floating point fast math flags are not implemented, they are based around
	// how the operation is performed which is not a concern of this function, but are important
	// variables when calculating the time it takes for the computation to complete

	// if(instruction.binary.ty.find("half")){
	// 	// No current implementation of half in c++, must implement an outside code for this functionality
	// }
	
	double op1;
	double op2;
	double result;
	float fresult;
	

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 / op2;

	// Store result back into memory
	if(instruction.binary.ty.find("float")){
	fresult = (float) result;
	memcpy(&instruction.general.returnRegister->value, &fresult, instruction.general.returnRegister->size);
	}
	else 	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
}
void Operations::llvm_frem(const struct Instruction&  instruction){
    // Floating point modulo division
	// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

	// Currently floating point fast math flags are not implemented, they are based around
	// how the operation is performed which is not a concern of this function, but are important
	// variables when calculating the time it takes for the computation to complete

	// For this instruction I need to write an overload of the modulo operator to work for floating
	// point numbers, not implemented for now

	// if(instruction.binary.ty.find("half")){
	// 	// No current implementation of half in c++, must implement an outside code for this functionality
	// }
	
	// double op1;
	// double op2;
	// double result;
	// float fresult;
	

	// // If immediate values convert from string, else load from register
	// if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	// else{
	// 	memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	// }

	// if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	// else{
	// 	memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	// }
	// // Perform arithmatic
	// result = op1 % op2;

	// // Store result back into memory
	// if(instruction.binary.ty.find("float")){
	// fresult = (float) result;
	// memcpy(&instruction.general.returnRegister->value, &fresult, instruction.general.returnRegister->size);
	// }
	// else 	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Bitwise Binary Operations */

void Operations::llvm_shl(const struct Instruction&  instruction){
	// Shift Left Operation
	// <result> = shl <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result

	// Bitwise operations must be performed on integers or a vector of integers
	// Vectors currently unsupported

	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 << op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
}
void Operations::llvm_lshr(const struct Instruction&  instruction){
	// Logical Shift Right Operation
	// <result> = lshr <ty> <op1>, <op2>; yields ty : result
	// <result> = lshr exact <ty> <op1>, <op2>; yields ty : result

		// Bitwise operations must be performed on integers or a vector of integers
	// Vectors currently unsupported

	uint op1;
	uint op2;
	uint result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 >> op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_ashr(const struct Instruction&  instruction){
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result

		// Bitwise operations must be performed on integers or a vector of integers
	// Vectors currently unsupported

	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 >> op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_and(const struct Instruction&  instruction){
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result

		// Bitwise operations must be performed on integers or a vector of integers
	// Vectors currently unsupported

	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 & op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
	
}
void Operations::llvm_or(const struct Instruction&  instruction){
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result

		// Bitwise operations must be performed on integers or a vector of integers
	// Vectors currently unsupported

	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 | op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_xor(const struct Instruction&  instruction){
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result

		// Bitwise operations must be performed on integers or a vector of integers
	// Vectors currently unsupported

	int op1;
	int op2;
	int result;

	// If immediate values convert from string, else load from register
	if(instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else{
		memcpy(&op1, &instruction.bitwise.op1->value, instruction.bitwise.op1->size);
	}

	if(instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else{
		memcpy(&op2, &instruction.bitwise.op2->value, instruction.bitwise.op2->size);
	}
	// Perform arithmatic
	result = op1 ^ op2;

	// Store result back into memory
	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Memory Access Operations */

void Operations::llvm_alloca(const struct Instruction&  instruction){
	// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result
	// C++ Code - <type>* result = new <ty>[NumElements]
	// Unsure how align will be used here if not == sizeOf(<type>)/8

	
}
void Operations::llvm_load(const struct Instruction&  instruction){
	// <result> = load [volatile] <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>]
	// address from which to load
	
}
void Operations::llvm_store(const struct Instruction&  instruction){
	// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void Operations::}
	// Value to store, address at which to store it
}
void Operations::llvm_getelementptr(const struct Instruction&  instruction){
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
	const struct Instruction&  RT {
	char A;
	int B[10][20];
	char C;
	};
	const struct Instruction&  ST {
	int X;
	double Y;
	const struct Instruction&  RT Z;
	};

	int *foo(const struct Instruction&  ST *s) {
	return &s[1].Z.B[5][13];
	}

	LLVM - Code
	%const struct Instruction& .RT = type { i8, [10 x [20 x i32]], i8 }
	%const struct Instruction& .ST = type { i32, double, %const struct Instruction& .RT }

	define i32* @foo(%const struct Instruction& .ST* %s) nounwind uwtable readnone optsize ssp {
	entry:
	%arrayidx = getelementptr inbounds %const struct Instruction& .ST* %s, i64 1, i32 2, i32 1, i64 5, i64 13
	ret i32* %arrayidx
	}
	
	*/

	
}
void Operations::llvm_fence(const struct Instruction&  instruction){ }
void Operations::llvm_cmpxchg(const struct Instruction&  instruction){ }
void Operations::llvm_atomicrmw(const struct Instruction&  instruction){ }

/* Operations::llvm Conversion Operations */
void Operations::llvm_trunc(const struct Instruction&  instruction){ }
void Operations::llvm_zext(const struct Instruction&  instruction){ }
void Operations::llvm_sext(const struct Instruction&  instruction){ }
void Operations::llvm_fptoui(const struct Instruction&  instruction){ }
void Operations::llvm_fptosi(const struct Instruction&  instruction){ }
void Operations::llvm_uitofp(const struct Instruction&  instruction){ }
void Operations::llvm_sitofp(const struct Instruction&  instruction){ }
void Operations::llvm_fptrunc(const struct Instruction&  instruction){ }
void Operations::llvm_fpext(const struct Instruction&  instruction){ }
void Operations::llvm_ptrtoint(const struct Instruction&  instruction){ }
void Operations::llvm_inttoptr(const struct Instruction&  instruction){ }
void Operations::llvm_bitcast(const struct Instruction&  instruction){ }
void Operations::llvm_addrspacecast(const struct Instruction&  instruction){ }

/* Operations::llvm Control Operations */
void Operations::llvm_icmp(const struct Instruction&  instruction){ 






}
void Operations::llvm_fcmp(const struct Instruction&  instruction){ 






	
}
void Operations::llvm_phi(const struct Instruction&  instruction, std::string prevBB){ 






}
void Operations::llvm_call(const struct Instruction&  instruction){ }
void Operations::llvm_select(const struct Instruction&  instruction){ }
void Operations::llvm_vaarg(const struct Instruction&  instruction){ }
void Operations::llvm_landingpad(const struct Instruction&  instruction){ }

/* Operations::llvm Vector Operations*/
void Operations::llvm_extractelement(const struct Instruction&  instruction){ }
void Operations::llvm_insertelement(const struct Instruction&  instruction){ }
void Operations::llvm_shufflevector(const struct Instruction&  instruction){ }

/* Operations::llvm Aggregate Operations */
void Operations::llvm_extractvalue(const struct Instruction&  instruction){ }
void Operations::llvm_insertvalue(const struct Instruction&  instruction){ }

/* Custom Operations */
void Operations::llvm_dmafence(const struct Instruction& instruction) { }
void Operations::llvm_dmastore(const struct Instruction& instruction) { }
void Operations::llvm_dmaload(const struct Instruction& instruction) { }
void Operations::llvm_indexadd(const struct Instruction& instruction){ }
void Operations::llvm_silentstore(const struct Instruction& instruction) { }
void Operations::llvm_sine(const struct Instruction& instruction) { }
void Operations::llvm_cosine(const struct Instruction& instruction) { }


bool Operations::to_bool(std::string const& i1){
	return i1 != "0";
}