#include "operations.hh"
#include <string>
#include "debug/LLVMOp.hh"
#include <cstdint>
#include <vector>

/* Operations::llvm Terminator Instructions */
void Operations::llvm_move(const struct Instruction &instruction) {
	//Not in current Operations::llvm reference. memmove perhaps?
}
void Operations::llvm_ret(const struct Instruction &instruction) {
	//Operation of device is finished
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
}
void Operations::llvm_br(const struct Instruction &instruction) {
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);

	unsigned long long int condition = 0;

	if (!(instruction.terminator.unconditional)) {
		DPRINTF(LLVMOp, "Conditional Branch Operation \n");
		condition = instruction.terminator.cond->value;
		if (condition != 0) instruction.terminator.dest = instruction.terminator.iftrue;
		else instruction.terminator.dest = instruction.terminator.iffalse;
		DPRINTF(LLVMOp, " True: %s, False: %s, Reg: %s, Condition: %d\n", instruction.terminator.iftrue, instruction.terminator.iffalse, instruction.terminator.cond->getName(), condition);
	}
	else DPRINTF(LLVMOp, "Unconditonal Branch Operation \n");
}
void Operations::llvm_switch(const struct Instruction &instruction) {}
void Operations::llvm_indirectbr(const struct Instruction &instruction) {}
void Operations::llvm_invoke(const struct Instruction &instruction) {}
void Operations::llvm_resume(const struct Instruction &instruction) {}
void Operations::llvm_unreachable(const struct Instruction &instruction) {}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
/* Integer Operations*/
void Operations::llvm_add(const struct Instruction &instruction) {
	// Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 + op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u + %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_sub(const struct Instruction &instruction) {
	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 - op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u - %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_mul(const struct Instruction &instruction) {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 * op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u * %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_udiv(const struct Instruction &instruction) {
	// Unsigned Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	uint64_t op1 = 0;
	uint64_t op2 = 0;
	uint64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 / op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_sdiv(const struct Instruction &instruction) {
	// Signed Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 / op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_urem(const struct Instruction &instruction) {
	//Unsigned modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	uint64_t op1 = 0;
	uint64_t op2 = 0;
	uint64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 % op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u %% %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_srem(const struct Instruction &instruction) {
	//Signed modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 % op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u %% %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Binary Operations*/
/* Floating Point Operations*/
void Operations::llvm_fadd(const struct Instruction &instruction) {
	// Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 + op2;
	// Store result in return register
	if (instruction.binary.ty.find("float") == 0) {
		// Typecast to float before returning value
		fresult = (float)result;
		instruction.general.returnRegister->setValue(&fresult);
	} else instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u + %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_fsub(const struct Instruction &instruction) {
	// Floating point Subtraction
	// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 - op2;
	// Store result in return register
	if (instruction.binary.ty.find("float") == 0) {
		// Typecast to float before returning value
		fresult = (float)result;
		instruction.general.returnRegister->setValue(&fresult);
	} else instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u - %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_fmul(const struct Instruction &instruction) {
	// Floating point Multiplication
	// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 * op2;
	// Store result in return register
	if (instruction.binary.ty.find("float") == 0) {
		// Typecast to float before returning value
		fresult = (float)result;
		instruction.general.returnRegister->setValue(&fresult);
	} else instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u * %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_fdiv(const struct Instruction &instruction) {
	// Floating point Division
	// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 / op2;
	// Store result in return register
	if (instruction.binary.ty.find("float") == 0) {
		// Typecast to float before returning value
		fresult = (float)result;
		instruction.general.returnRegister->setValue(&fresult);
	} else instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_frem(const struct Instruction &instruction) {
	// Floating point modulo division
	// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	/* Not possible until modulo operator is overloaded to accept floating point numbers
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) op1 = stoi(instruction.binary.iop1);
	else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) op2 = stoi(instruction.binary.iop2);
	else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 % op2;
	// Store result in return register
	if (instruction.binary.ty.find("float") == 0) {
		// Typecast to float before returning value
		fresult = (float)result;
		instruction.general.returnRegister->setValue(&fresult);
	} else instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u %% %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
	*/
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Bitwise Binary Operations */
void Operations::llvm_shl(const struct Instruction &instruction) {
	// Shift Left Operation
	// <result> = shl <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) op1 = stoi(instruction.bitwise.iop1);
	else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) op2 = stoi(instruction.bitwise.iop2);
	else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 << op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u << %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_lshr(const struct Instruction &instruction) {
	// Logical Shift Right Operation
	// <result> = lshr <ty> <op1>, <op2>; yields ty : result
	// <result> = lshr exact <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) op1 = stoi(instruction.bitwise.iop1);
	else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) op2 = stoi(instruction.bitwise.iop2);
	else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 >> op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u >> %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_ashr(const struct Instruction &instruction) {
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) op1 = stoi(instruction.bitwise.iop1);
	else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) op2 = stoi(instruction.bitwise.iop2);
	else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 >> op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u >> %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_and(const struct Instruction &instruction) {
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) op1 = stoi(instruction.bitwise.iop1);
	else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) op2 = stoi(instruction.bitwise.iop2);
	else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 & op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u & %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_or(const struct Instruction &instruction) {
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) op1 = stoi(instruction.bitwise.iop1);
	else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) op2 = stoi(instruction.bitwise.iop2);
	else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 | op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u | %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_xor(const struct Instruction &instruction) {
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) op1 = stoi(instruction.bitwise.iop1);
	else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) op2 = stoi(instruction.bitwise.iop2);
	else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 ^ op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u ^ %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
///////////////////////////////////////////////////////////////////////////////
/* LLVM Memory Access Operations */

void Operations::llvm_alloca(const struct Instruction &instruction) {
	// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result
	// C++ Code - <type>* result = new <ty>[NumElements]
	// Unsure how align will be used here if not == sizeOf(<type>)/8
}

void Operations::llvm_getelementptr(const struct Instruction &instruction) {
	// <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	uint64_t index = instruction.memory.getptr.index;
	uint64_t currentValue[MAXGPE];
	uint64_t size[MAXGPE];
	uint64_t newAddress = 0;

	for(int i = 0; i<MAXGPE; i++) {
		currentValue[i] = 0;
		size[i] = 1;
	}
	if (instruction.memory.getptr.ty[0][0] == 'i') {
		size[0] = (stoi(instruction.memory.getptr.ty[0].substr(1))) / 8;
	} else {
		if (instruction.memory.getptr.ty[0].find("double") > 0) {
			size[0] = 8;
		} else if (instruction.memory.getptr.ty[0].find("float") > 0) {
			size[0] = 4;
		}
	}
//	size[0] = stoi(instruction.memory.getptr.ty[0].substr(1)) / 8;

	for (int i = 1; i < index; i++) {
//		if (instruction.memory.getptr.ty[i][0] == 'i') {
//			size[i] = (stoi(instruction.memory.getptr.ty[i].substr(1))) / 8;
//		} else {
//			if (instruction.memory.getptr.ty[i].find("double") > 0) {
//				size[i] = 8;
//			} else if (instruction.memory.getptr.ty[i].find("float") > 0) {
//				size[i] = 4;
//			}
//		}
		currentValue[i] = instruction.memory.getptr.idx[i]->value;
		DPRINTF(LLVMOp, "Size: %d, Current Value: %d\n", size[i], currentValue[i]);
	}

	for (int i = 1; i < index; i++) {
		instruction.memory.getptr.reference[i] = size[i] * currentValue[i];
		newAddress = newAddress + instruction.memory.getptr.reference[i];
	}
	newAddress *= size[0];
	newAddress += instruction.memory.getptr.idx[0]->getValue();
	instruction.general.returnRegister->setValue(&newAddress);
	DPRINTF(LLVMOp, "Base Address in Register %s: %X\n", instruction.memory.getptr.idx[0]->getName(), instruction.memory.getptr.idx[0]->getValue());
	DPRINTF(LLVMOp, "Memory Location =  %X (%d)\n", instruction.general.returnRegister->value, instruction.general.returnRegister->value);
}
void Operations::llvm_fence(const struct Instruction &instruction) {}
void Operations::llvm_cmpxchg(const struct Instruction &instruction) {}
void Operations::llvm_atomicrmw(const struct Instruction &instruction) {}

/* Operations::llvm Conversion Operations */
void Operations::llvm_trunc(const struct Instruction &instruction) {}
void Operations::llvm_zext(const struct Instruction &instruction) {}
void Operations::llvm_sext(const struct Instruction &instruction) {}
void Operations::llvm_fptoui(const struct Instruction &instruction) {}
void Operations::llvm_fptosi(const struct Instruction &instruction) {}
void Operations::llvm_uitofp(const struct Instruction &instruction) {}
void Operations::llvm_sitofp(const struct Instruction &instruction) {}
void Operations::llvm_fptrunc(const struct Instruction &instruction) {}
void Operations::llvm_fpext(const struct Instruction &instruction) {}
void Operations::llvm_ptrtoint(const struct Instruction &instruction) {}
void Operations::llvm_inttoptr(const struct Instruction &instruction) {}
void Operations::llvm_bitcast(const struct Instruction &instruction) {}
void Operations::llvm_addrspacecast(const struct Instruction &instruction) {}

/* Operations::llvm Control Operations */
void Operations::llvm_icmp(const struct Instruction &instruction) {
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// Determine if comparison is being made between registers or immediate values
	if (instruction.other.compare.immediate1) op1 = stoi(instruction.other.compare.iop1);
	else op1 = instruction.other.compare.op1->getValue();
	if (instruction.other.compare.immediate2) op2 = stoi(instruction.other.compare.iop2);
	else op2 = instruction.other.compare.op2->getValue();
	// Perform Comparison
	if (instruction.other.compare.condition.eq) result = (op1 == op2);
	else if (instruction.other.compare.condition.ne) result = (op1 != op2);
	else if (instruction.other.compare.condition.ugt) result = ((uint)op1 > (uint)op2);
	else if (instruction.other.compare.condition.uge) result = ((uint)op1 >= (uint)op2);
	else if (instruction.other.compare.condition.ult) result = ((uint)op1 < (uint)op2);
	else if (instruction.other.compare.condition.ule) result = ((uint)op1 <= (uint)op2);
	else if (instruction.other.compare.condition.sgt) result = (op1 > op2);
	else if (instruction.other.compare.condition.sge) result = (op1 >= op2);
	else if (instruction.other.compare.condition.slt) result = (op1 < op2);
	else if (instruction.other.compare.condition.sle) result = (op1 <= op2);
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "Comparing %u and %u, result is %u.\n", op1, op2, instruction.general.returnRegister->getValue());
}
void Operations::llvm_fcmp(const struct Instruction &instruction) {
	DPRINTF(LLVMOp, "Performing %s Operation\n", instruction.general.opCode);
	double op1 = 0.0;
	double op2 = 0.0;
	uint64_t result = 0;
	// Determine if comparison is being made between registers or immediate values
	if (instruction.other.compare.immediate1) op1 = stoi(instruction.other.compare.iop1);
	else op1 = instruction.other.compare.op1->getValue();
	if (instruction.other.compare.immediate2) op2 = stoi(instruction.other.compare.iop2);
	else op2 = instruction.other.compare.op2->getValue();
	// Perform Comparison
	if (instruction.other.compare.condition.condFalse) result = false;
	else if (instruction.other.compare.condition.condTrue) result = true;
	else if (instruction.other.compare.condition.oeq) {if (!(std::isnan(op1) && std::isnan(op2))) {result = (op1 == op2);}
	} else if (instruction.other.compare.condition.ogt) { if (!(std::isnan(op1) && std::isnan(op2))) { result = (op1 > op2); }
	} else if (instruction.other.compare.condition.oge) { if (!(std::isnan(op1) && std::isnan(op2))) { result = (op1 >= op2); } 
	} else if (instruction.other.compare.condition.olt) { if (!(std::isnan(op1) && std::isnan(op2))) { result = (op1 < op2); } 
	} else if (instruction.other.compare.condition.ole) { if (!(std::isnan(op1) && std::isnan(op2))) { result = (op1 <= op2); } 
	} else if (instruction.other.compare.condition.one) { if (!(std::isnan(op1) && std::isnan(op2))) { result = (op1 != op2); } 
	} else if (instruction.other.compare.condition.ord) { if (!(std::isnan(op1) && std::isnan(op2))) { result = true; }
	} else if (instruction.other.compare.condition.ueq) result = (op1 == op2);
 	else if (instruction.other.compare.condition.ugt) result = (op1 > op2);
	else if (instruction.other.compare.condition.uge) result = (op1 >= op2);
	else if (instruction.other.compare.condition.ult) result = (op1 < op2);
	else if (instruction.other.compare.condition.ule) result = (op1 <= op2);
	else if (instruction.other.compare.condition.une) result = (op1 != op2);
	else if (instruction.other.compare.condition.uno) result = (op1 != op2);
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "Comparing %f and %f, result is %u.\n", op1, op2, instruction.general.returnRegister->getValue());
}
void Operations::llvm_phi(const struct Instruction &instruction, std::string prevBB) {
	// <result> = phi <ty> [ <val0>, <label0>], ...
	DPRINTF(LLVMOp, "Performing %s Operation, Previous BB was #%s\n", instruction.general.opCode, prevBB);
	uint64_t val;
	for (int i = 0; i < MAXPHI; i++) {
		// Look for phi label that matches the previous basic block
		if (prevBB == instruction.other.phi.label[i]) {
			// Store the value associated with the label into val
			if (instruction.other.phi.immVal[i]) val = stoi(instruction.other.phi.ival[i]);
			else val = instruction.other.phi.val[i]->getValue();
			break;
		}
	}
	// Store val in return register
	instruction.general.returnRegister->setValue(&val);
	DPRINTF(LLVMOp, "Storing %u in Register %s\n", instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void Operations::llvm_call(const struct Instruction &instruction) {}
void Operations::llvm_select(const struct Instruction &instruction) {}
void Operations::llvm_vaarg(const struct Instruction &instruction) {}
void Operations::llvm_landingpad(const struct Instruction &instruction) {}

/* Operations::llvm Vector Operations*/
void Operations::llvm_extractelement(const struct Instruction &instruction) {}
void Operations::llvm_insertelement(const struct Instruction &instruction) {}
void Operations::llvm_shufflevector(const struct Instruction &instruction) {}

/* Operations::llvm Aggregate Operations */
void Operations::llvm_extractvalue(const struct Instruction &instruction) {}
void Operations::llvm_insertvalue(const struct Instruction &instruction) {}

/* Custom Operations */
void Operations::llvm_dmafence(const struct Instruction &instruction) {}
void Operations::llvm_dmastore(const struct Instruction &instruction) {}
void Operations::llvm_dmaload(const struct Instruction &instruction) {}
void Operations::llvm_indexadd(const struct Instruction &instruction) {}
void Operations::llvm_silentstore(const struct Instruction &instruction) {}
void Operations::llvm_sine(const struct Instruction &instruction) {}
void Operations::llvm_cosine(const struct Instruction &instruction) {}
