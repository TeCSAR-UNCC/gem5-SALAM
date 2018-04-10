#include "operations.hh"



/* Operations::llvm Terminator Instructions */
void Operations::llvm_move(const struct Instruction& instruction){
    //Not in current Operations::llvm reference. memmove perhaps?
}
void Operations::llvm_ret(const struct Instruction& instruction){
    //Operation of device is finished
}
std::string Operations::llvm_br(const struct Instruction& instruction){

	int condition;

	if(!(instruction.terminator.unconditional)){
	memcpy(&condition, &instruction.terminator.cond->value, instruction.terminator.cond->size);
	if(condition != 0) instruction.terminator.dest = instruction.terminator.iftrue;
	else instruction.terminator.dest = instruction.terminator.iffalse;
	}

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
	// uint8_t* getCurData() { return curData; }
	// Code below is for debugging without using GEM5 for memory managment


}
void Operations::llvm_store(const struct Instruction&  instruction){
	// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void Operations::}
	// Value to store, address at which to store it
	// int prepRead(Addr src, size_t length);
	// int prepWrite(Addr dst, uint8_t* value, size_t length);
	// Code below is for debuugging withouut using GEM5 for memory managent


}
void Operations::llvm_getelementptr(const struct Instruction&  instruction){
	
	// <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>

	
	
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

	int op1;
	int op2;
	bool result;

	if(instruction.other.compare.immediate1) op1 = stoi(instruction.other.compare.iop1);
		else{
			memcpy(&op1, &instruction.other.compare.op1->value, instruction.other.compare.op1->size);
		}

		if(instruction.other.compare.immediate2) op2 = stoi(instruction.other.compare.iop2);
		else{
			memcpy(&op2, &instruction.other.compare.op2->value, instruction.other.compare.op2->size);
		}

	if (instruction.other.compare.condition.eq) result = (op1 == op2);
	else if (instruction.other.compare.condition.ne) result = (op1 != op2);
	else if (instruction.other.compare.condition.ugt) result = ((uint) op1 > (uint) op2);
	else if (instruction.other.compare.condition.uge) result = ((uint) op1 >= (uint) op2);
	else if (instruction.other.compare.condition.ult) result = ((uint) op1 < (uint) op2);
	else if (instruction.other.compare.condition.ule) result = ((uint) op1 <= (uint) op2); 
	else if (instruction.other.compare.condition.sgt) result = (op1 > op2);
	else if (instruction.other.compare.condition.sge) result = (op1 >= op2);
	else if (instruction.other.compare.condition.slt) result = (op1 < op2);
	else if (instruction.other.compare.condition.sle) result = (op1 <= op2);


	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);


}
void Operations::llvm_fcmp(const struct Instruction&  instruction){ 


	double op1;
	double op2;
	bool result;

	if(instruction.other.compare.immediate1) op1 = stoi(instruction.other.compare.iop1);
		else{
			memcpy(&op1, &instruction.other.compare.op1->value, instruction.other.compare.op1->size);
		}

		if(instruction.other.compare.immediate2) op2 = stoi(instruction.other.compare.iop2);
		else{
			memcpy(&op2, &instruction.other.compare.op2->value, instruction.other.compare.op2->size);
		}

	if (instruction.other.compare.condition.condFalse) result = false;
	else if (instruction.other.compare.condition.condTrue) result = true;

	else if (instruction.other.compare.condition.oeq)  {
		if(!(std::isnan(op1) && std::isnan(op2))){
			result = (op1 == op2);
		}
		else{
			result = false;
		}
	}
	else if (instruction.other.compare.condition.ogt) {
		if(!(std::isnan(op1) && std::isnan(op2))){
			result = (op1 > op2);
		}
		else{
			result = false;
		}
	}
	else if (instruction.other.compare.condition.oge) {
		if(!(std::isnan(op1) && std::isnan(op2))){
			result = (op1 >= op2);
		}
		else{
			result = false;
		}
	} 
	else if (instruction.other.compare.condition.olt) {
		if(!(std::isnan(op1) && std::isnan(op2))){
			result = (op1 < op2);
		}
		else{
			result = false;
		}
	}
	else if (instruction.other.compare.condition.ole) {
		if(!(std::isnan(op1) && std::isnan(op2))){
			result = (op1 <= op2);
		}
		else{
			result = false;
		}
	} 
	else if (instruction.other.compare.condition.one)  {
		if(!(std::isnan(op1) && std::isnan(op2))){
			result = (op1 != op2);
		}
		else{
			result = false;
		}
	}
	else if (instruction.other.compare.condition.ord) {
		if(!(std::isnan(op1) && std::isnan(op2))){
			result = true;
		}
		else{
			result = false;
		}
	}
	else if (instruction.other.compare.condition.ueq)  result = (op1 == op2);
	else if (instruction.other.compare.condition.ugt)  result = (op1 > op2); 
	else if (instruction.other.compare.condition.uge)  result = (op1 >= op2);  
	else if (instruction.other.compare.condition.ult)  result = (op1 < op2); 
	else if (instruction.other.compare.condition.ule)  result = (op1 <= op2); 
	else if (instruction.other.compare.condition.une)  result = (op1 != op2); 
	else if (instruction.other.compare.condition.uno)  result = (op1 != op2); 

	memcpy(&instruction.general.returnRegister->value, &result, instruction.general.returnRegister->size);

}
void Operations::llvm_phi(const struct Instruction&  instruction, std::string prevBB){ 

	int val;

	// <result> = phi <ty> [ <val0>, <label0>], ...
	for(int i = 0; i < MAXPHI; i++){
		if(prevBB == instruction.other.phi.label[i]->getName()){
			if(instruction.other.phi.immVal[i]) val = stoi(instruction.other.phi.ival[i]);
			else memcpy(&val, &instruction.other.phi.val[i]->value, instruction.other.phi.val[i]->size);
		}
	}

	memcpy(&instruction.general.returnRegister->value, &val, instruction.general.returnRegister->size);

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