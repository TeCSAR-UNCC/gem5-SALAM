#include "instructions.hh"

void 
Add::compute() {
// Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) {
		op1 = stoi(instruction.binary.iop1);
		instruction.general.immediateCount->accessedRead();
	} else {
		op1 = instruction.binary.op1->getValue();
	}
	if (instruction.binary.immediate2) {
		op2 = stoi(instruction.binary.iop2);
		instruction.general.immediateCount->accessedRead();
	} else {
		op2 = instruction.binary.op2->getValue();
	}
	// Perform arithmetic
	result = op1 + op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	
	DPRINTF(LLVMOp, "%u + %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
Sub::compute() {
	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) {
		op1 = stoi(instruction.binary.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) {
		op2 = stoi(instruction.binary.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 - op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u - %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
Mul::compute() {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) {
		op1 = stoi(instruction.binary.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) {
		op2 = stoi(instruction.binary.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 * op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u * %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
UDiv::compute() {
	// Unsigned Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	uint64_t op1 = 0;
	uint64_t op2 = 0;
	uint64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) {
		op1 = stoi(instruction.binary.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) {
		op2 = stoi(instruction.binary.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 / op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
SDiv::compute() {
	// Signed Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) {
		op1 = stoi(instruction.binary.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) {
		op2 = stoi(instruction.binary.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 / op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
URem::compute() {
	//Unsigned modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	uint64_t op1 = 0;
	uint64_t op2 = 0;
	uint64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) {
		op1 = stoi(instruction.binary.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) {
		op2 = stoi(instruction.binary.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 % op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u %% %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
SRem::compute() {
	//Signed modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1 = 0;
	int64_t op2 = 0;
	int64_t result = 0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.immediate1) {
		op1 = stoi(instruction.binary.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.binary.op1->getValue();
	if (instruction.binary.immediate2) {
		op2 = stoi(instruction.binary.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.binary.op2->getValue();
	// Perform arithmetic
	result = op1 % op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u %% %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
FAdd::compute() {
	// Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	double result;
	float fresult = 0.0;
	// If immediate values convert from string, else load from register
	if (instruction.binary.ty.find("double") == 0) {
		if (instruction.binary.immediate1) {
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(double *)&temp1;
		}
		if (instruction.binary.immediate2) {
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(double *)&temp2;
		}
	} else {
		if (instruction.binary.immediate1) {
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(float *)&temp1;
		}
		if (instruction.binary.immediate2) {
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(float *)&temp2;
		}
	}
	// Perform arithmetic
	result = op1 + op2;
	// Store result in return register
	
	if (instruction.binary.ty.find("float") == 0) {
		// Typecast to float before returning value
		fresult = (float)result;
		instruction.general.returnRegister->setValue(&fresult);
	} else instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp,"%u\n", fresult);
	DPRINTF(LLVMOp, "%u + %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
FSub::compute() {
	// Floating point Subtraction
	// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	double result;
	float fresult;
	if (instruction.binary.ty.find("double") == 0) {
		if (instruction.binary.immediate1) {
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(double *)&temp1;
		}
		if (instruction.binary.immediate2) {
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(double *)&temp2;
		}
	} else {
		if (instruction.binary.immediate1) {
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(float *)&temp1;
		}
		if (instruction.binary.immediate2) {
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(float *)&temp2;
		}
	}
	// Perform arithmetic
	result = op1 - op2;
	// Store result in return register
	if (instruction.binary.ty.find("float") == 0) {
		// Typecast to float before returning value
		fresult = (float)result;
		instruction.general.returnRegister->setValue(&fresult);
	} else instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%f - %f = %f: Stored in Register %s. \n", op1, op2, result, instruction.general.returnRegister->getName());

}

void 
FMul::compute() {
	// Floating point Multiplication
	// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (instruction.binary.ty.find("double") == 0) {
		if (instruction.binary.immediate1) { 
			DPRINTF(IOAcc, "Gibberish\n");
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(double *)&temp1;
		}
		if (instruction.binary.immediate2) {
			DPRINTF(IOAcc, "Gibberish\n");
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(double *)&temp2;
		}
	} else {
		if (instruction.binary.immediate1) {
			DPRINTF(IOAcc, "Gibberish\n");
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(float *)&temp1;
		}
		if (instruction.binary.immediate2) {
			DPRINTF(IOAcc, "Gibberish\n");
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(float *)&temp2;
		}
	}
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

void 
FDiv::compute() {
	// Floating point Division
	// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (instruction.binary.ty.find("double") == 0) {
		if (instruction.binary.immediate1) { 
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(double *)&temp1;
		}
		if (instruction.binary.immediate2) {
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(double *)&temp2;
		}
	} else {
		if (instruction.binary.immediate1) { 
			op1 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop1));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp1 = instruction.binary.op1->getValue();
		    op1 = *(float *)&temp1;
		}
		if (instruction.binary.immediate2) {
			op2 = stof(convertImmediate(instruction.binary.ty, instruction.binary.iop2));
			instruction.general.immediateCount->accessedRead();
		} else {
		    uint64_t temp2 = instruction.binary.op2->getValue();
		    op2 = *(float *)&temp2;
		}
	}
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

void 
FRem::compute() { }

void 
Shl::compute() {
	// Shift Left Operation
	// <result> = shl <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) {
		op1 = stoi(instruction.bitwise.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) {
		op2 = stoi(instruction.bitwise.iop2);
		instruction.general.immediateCount->accessedRead();	
	} else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 << op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u << %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
LShr::compute() {
	// Logical Shift Right Operation
	// <result> = lshr <ty> <op1>, <op2>; yields ty : result
	// <result> = lshr exact <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) {
		op1 = stoi(instruction.bitwise.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) {
		op2 = stoi(instruction.bitwise.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = ((unsigned) op1) >> op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u >> %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
AShr::compute() {
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) {
		op1 = stoi(instruction.bitwise.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) {
		op2 = stoi(instruction.bitwise.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	if (op1 < 0 && op2 > 0)
        result = op1 >> op2 | ~(~0U >> op2);
    else
        result = op1 >> op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u >> %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
And::compute() {
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) {
		op1 = stoi(instruction.bitwise.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) {
		op2 = stoi(instruction.bitwise.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 & op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u & %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
Or::compute() {
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) {
		op1 = stoi(instruction.bitwise.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) {
		op2 = stoi(instruction.bitwise.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 | op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u | %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

void 
Xor::compute() {
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	int64_t op1;
	int64_t op2;
	int64_t result;
	// If immediate values convert from string, else load from register
	if (instruction.bitwise.immediate1) {
		op1 = stoi(instruction.bitwise.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.bitwise.op1->getValue();
	if (instruction.bitwise.immediate2) {
		op2 = stoi(instruction.bitwise.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.bitwise.op2->getValue();
	// Perform arithmetic
	result = op1 ^ op2;
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "%u ^ %u = %u: Stored in Register %s. \n", op1, op2, instruction.general.returnRegister->value, instruction.general.returnRegister->getName());

}

/*
void
Load::compute() {
    uint64_t src = instruction.memory.load.pointer->value;
	instruction.general.returnRegister->setSize();
	DPRINTF(LLVMGEP,"Load Operation: Name = %s, Size = %d\n", instruction.memory.load.pointer->getName(), instruction.general.returnRegister->size);
	req = new MemoryRequest((Addr)src, instruction.general.returnRegister->size);
	comm->enqueueRead(req);
	break;  
}

void
Store::compute() {
	uint64_t data;
	uint64_t size = 0;
	uint64_t dst = instruction.memory.store.pointer->value;
	if(instruction.memory.store.immediate) {
		DPRINTF(ComputeNode, "Immediate value store. \n");
		data = (uint64_t) instruction.memory.store.ival;
		size = setSize(instruction.memory.store.ty);
		req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), size);
	} else {
	    data = instruction.memory.store.value->value;
        req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), instruction.memory.store.value->size);
		DPRINTF(LLVMGEP,"Store Operation: Type = %s, Size = %d\n", instruction.memory.store.value->getType(), instruction.memory.store.value->size);
	}
	comm->enqueueWrite(req);
	break;    
}

void
Move::compute() {

}

void
Ret::compute() {

}

void
Br::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation!\n", _OpCode);

	unsigned long long int condition = 0;

	if (!(instruction.terminator.unconditional)) {
		DPRINTF(LLVMOp, "Conditional Branch Operation! \n");
		condition = instruction.terminator.cond->getValue();
		instruction.general.labelCount->accessedRead();
		if (condition != 0) instruction.terminator.dest = instruction.terminator.iftrue;
		else instruction.terminator.dest = instruction.terminator.iffalse;
		DPRINTF(LLVMOp, " True: %s, False: %s, Reg: %s, Condition: %d\n", instruction.terminator.iftrue, instruction.terminator.iffalse, instruction.terminator.cond->getName(), condition);
		instruction.terminator.cond->accessedRead();
	}
	else DPRINTF(LLVMOp, "Unconditonal Branch Operation! \n");    
}

void
Switch::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	uint64_t mainValue = instruction.terminator.value->getValue();
	bool found = false;
	DPRINTF(LLVMOp, "Register Name: %s\n", instruction.terminator.value->getName());
	for(int i = 0; i < instruction.terminator.cases.statements; i++) {
		DPRINTF(LLVMOp, "Comparing main value %d to case value %d: \n", mainValue, instruction.terminator.cases.value[i]);
		instruction.general.immediateCount->accessedRead();
		if(mainValue == instruction.terminator.cases.value[i]){
			instruction.terminator.dest = instruction.terminator.cases.dest[i];
			DPRINTF(LLVMOp, "Found!\n");
			found = true;
			return;
		}
	}
	if(!found) instruction.terminator.dest = instruction.terminator.defaultdest;
	DPRINTF(LLVMOp, "Switch selected, destination is %s!", instruction.terminator.dest);

}

void
IndirectBr::compute() { }

void
Invoke::compute() { }

void
Resume::compute() { }

void
Unreachable::compute() { }

void
Alloca::compute() { }

void
GetElementPtr::compute {
	// <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
	DPRINTF(LLVMGEP, "Performing %s Operation  (%s)\n", _OpCode, instruction.general.returnRegister->getName());
	uint64_t index = instruction.memory.getptr.index;
	uint64_t elements[MAXGPE];
	uint64_t currentValue[MAXGPE];
	uint64_t size[MAXGPE];
	uint64_t newAddress = 0;
	uint64_t dataSize = 0;
	uint64_t totalElements = 1;
	uint64_t finalCount = 0;
	
	int j = 0;
	// Initialize array values
	for(int i = 0; i<MAXGPE; i++) {
		currentValue[i] = 0;
		size[i] = 1;
		elements[i] = 1;
	}

	if(instruction.memory.getptr.pty[0] == '%') { // Return Type is a custom data type
		if(instruction.memory.getptr.llvmType != NULL) {
			dataSize = instruction.memory.getptr.llvmType->getSize();
			DPRINTF(LLVMGEP, "Custom Data Type: (%s), Number of Elements = (%d)\n", instruction.memory.getptr.llvmType->getName(), dataSize);
			j++;
		} else {
			DPRINTF(LLVMGEP, "Unknown Case - Custom Data Type!\n", dataSize);
		}

	for(int i = 0; i <= j; i++){
			if(instruction.memory.getptr.immediate[i]) {  // Immediate Values
				instruction.general.immediateCount->accessedRead();
				if(i == j) {
					// Final offset variable
					totalElements*=(instruction.memory.getptr.immdx[i]);
					DPRINTF(LLVMGEP, "Adding Final Offset: (%d)\n", instruction.memory.getptr.immdx[i]);
				} else {				
					// Total data size in memory offset variable 
				DPRINTF(LLVMGEP, "Immediate Values Offset:\n");
				DPRINTF(LLVMGEP, "Total Elements * idx[%d]: (%d) * (%d)  =  (%d)\n", i, dataSize, instruction.memory.getptr.immdx[i],  dataSize * instruction.memory.getptr.immdx[i]);
				totalElements*=(dataSize*instruction.memory.getptr.immdx[i]);
				}
			} else { // Register loaded values
				if(i == j) {
					// Final offset variable
					totalElements*=(instruction.memory.getptr.idx[i]->getValue());
					DPRINTF(LLVMGEP, "Adding Final Offset: (%d)\n", instruction.memory.getptr.idx[i]->getValue());
				} else {
				DPRINTF(LLVMGEP, "Register Loaded Value:\n");
				DPRINTF(LLVMGEP, "Total Elements * idx[%d]: (%d) * (%d) = (%d)\n", i, dataSize, instruction.memory.getptr.idx[i]->value, dataSize * instruction.memory.getptr.idx[i]->value);				
				totalElements*=(dataSize*instruction.memory.getptr.idx[i]->getValue());
				}
			}
			finalCount += totalElements;
			DPRINTF(LLVMGEP, " Final Count = %d\n", finalCount);
			totalElements = 1;
		}

		if(instruction.memory.getptr.llvmType != NULL) finalCount *=8; // Final offset calculation, total elements * memory size in bytes
		else finalCount *=4;
		DPRINTF(LLVMGEP, "Final Offset: %d\n", finalCount);
		newAddress += (instruction.memory.getptr.ptrval->getValue() + finalCount);
		instruction.general.returnRegister->setValue(&newAddress);
		////////////////////////////////////////
	} else if(instruction.memory.getptr.pty[0] == '[') { // Return type is a struct
		for(int i = 0; i < instruction.memory.getptr.pty.size(); i++){
			if(instruction.memory.getptr.pty[i] == '[') {
				// Find the next integer value Ex: [A x [B x [C x dataSize]]] returns A, B, C
				elements[j] = stoi(instruction.memory.getptr.pty.substr(i+1, instruction.memory.getptr.pty.find(' ', i)-i-1));
				DPRINTF(LLVMGEP, "Element Size = %d\n", elements[j]);
				j++;
			}
			if(instruction.memory.getptr.pty[i] == ']') break;
		}
		
		if(instruction.memory.getptr.llvmType != NULL) {
		dataSize = instruction.memory.getptr.llvmType->getSize();
		j++;
		DPRINTF(LLVMGEP, "Custom Data Type: %s, Size = %d\n", instruction.memory.getptr.llvmType->getName(), dataSize);
		} else {
			dataSize = 1;
			DPRINTF(LLVMGEP, "Data Type Size = %d\n", dataSize);
		}
		for(int i = 0; i <= j; i++){
			for(int k = 0; k <= j; k++){
				totalElements*=elements[k];
			}
			if(instruction.memory.getptr.immediate[i]) {
				instruction.general.immediateCount->accessedRead();
				if(i == j) {
					totalElements*=(instruction.memory.getptr.immdx[i]);
				} else {				
				DPRINTF(LLVMGEP, "Immediate \n");
				DPRINTF(LLVMGEP, "Total Elements * dataSize * idx[%d]: %d * %d * %d = \n", i, totalElements, dataSize, instruction.memory.getptr.immdx[i]);
				totalElements*=(dataSize*instruction.memory.getptr.immdx[i]);
				}
			} else {
				if(i == j) {
					totalElements*=(instruction.memory.getptr.idx[i]->getValue());
				} else {
				DPRINTF(LLVMGEP, "Register \n");
				DPRINTF(LLVMGEP, "Total Elements * dataSize * idx[%d]: %d * %d * %d = \n", i, totalElements, dataSize, instruction.memory.getptr.idx[i]->value);				
				totalElements*=(dataSize*instruction.memory.getptr.idx[i]->getValue());
				}
			}
			finalCount += totalElements;
			DPRINTF(LLVMGEP, " Final Count = %d\n", finalCount);
			totalElements = 1;
			elements[i] = 1;
		}

		if(instruction.memory.getptr.llvmType != NULL) finalCount *=8; // Final offset calculation, total elements * memory size in bytes
		else finalCount *=4;
		DPRINTF(LLVMGEP, "Final Offset: %d\n", finalCount);
		newAddress += (instruction.memory.getptr.ptrval->getValue() + finalCount);
		instruction.general.returnRegister->setValue(&newAddress);
	} else {
		for(int i = 0; i < MAXGPE; i++) {
			if (instruction.memory.getptr.pty[i] == 'i') {
				size[i] = (stoi(instruction.memory.getptr.pty.substr(1))) / 8;
			} else if (instruction.memory.getptr.pty.find("double") == 0) {
				size[i] = 8;
			} else if (instruction.memory.getptr.pty.find("float") == 0) {
				size[i] = 4;
			}
		}
	
		DPRINTF(LLVMGEP, "Size of data type: %d\n", size[0]);

		for (int i = 0; i < index; i++) {
			if(!instruction.memory.getptr.immediate[i]) currentValue[i] = instruction.memory.getptr.idx[i]->getValue();
			else {
				currentValue[i] = instruction.memory.getptr.immdx[i];
				instruction.general.immediateCount->accessedRead();
			}
			DPRINTF(LLVMGEP, "Size: %d, Current Value: %d\n", size[i], currentValue[i]);
		}

		for (int i = 0; i < index; i++) {
			instruction.memory.getptr.reference[i] = currentValue[i];
			newAddress = newAddress + instruction.memory.getptr.reference[i]*size[i];
		}
		//newAddress *= size[0];
		newAddress += instruction.memory.getptr.ptrval->getValue();
		instruction.general.returnRegister->setValue(&newAddress);
	}
	DPRINTF(LLVMGEP, "Global Register Read: (%s)\n",instruction.memory.getptr.ptrval->getName());
	instruction.memory.getptr.ptrval->accessedRead();
	DPRINTF(LLVMGEP, "Base Address in Register %s: %X\n", instruction.memory.getptr.ptrval->getName(), instruction.memory.getptr.ptrval->value);
	DPRINTF(LLVMGEP, "Memory Location =  %X (%d)\n\n", instruction.general.returnRegister->value, instruction.general.returnRegister->value);

}

void
Fence::compute() { }

void
CmpXChg::compute() { }

void
AtomicRMW::compute() { }

void
Trunc::compute() {
	int64_t value;
	int64_t result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();

	if (instruction.conversion.ty2 == "i32") result = 0xffffffff & value;
	else if (instruction.conversion.ty2 == "i16") result = 0xffff & value;
	else if (instruction.conversion.ty2 == "i8") result = 0xff & value;
	else if (instruction.conversion.ty2 == "i1") {
		if(value) result = 1;
		else result = 0;
	}
	instruction.general.returnRegister->setValue(&result);

}

void
ZExt::compute() {
	uint64_t value;
	uint64_t result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "i64") result = (uint64_t) value;
	else if (instruction.conversion.ty2 == "i32") result = (uint32_t) value;
	else if (instruction.conversion.ty2 == "i16") result = (uint16_t) value;
	else if (instruction.conversion.ty2 == "i8") result = (uint8_t) value;
	else if (instruction.conversion.ty2 == "i1") {
		if(value) result = 1;
		else result = 0;
	}
	instruction.general.returnRegister->setValue(&result);    
}

void
SExt::compute() {
	int64_t value;
	int64_t result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "i64") result = (int64_t) value;
	else if (instruction.conversion.ty2 == "i32") result = (int32_t) value;
	else if (instruction.conversion.ty2 == "i16") result = (int16_t) value;
	else if (instruction.conversion.ty2 == "i8") result = (int8_t) value;
	else if (instruction.conversion.ty2 == "i1") {
		if(value) result = -1;
		else result = 0;
	}
	instruction.general.returnRegister->setValue(&result);
}
////////////////////////////////////////////////////////

void 
FPtoUI::compute() {
	double value;
	uint64_t result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "i64") result = (uint64_t) value;
	else if (instruction.conversion.ty2 == "i32") result = (uint32_t) value;
	else if (instruction.conversion.ty2 == "i16") result = (uint16_t) value;
	else if (instruction.conversion.ty2 == "i8") result = (uint8_t) value;
	else if (instruction.conversion.ty2 == "i1") {
		if(value) result = 1;
		else result = 0;
	}
	instruction.general.returnRegister->setValue(&result);
}
void
FPtoSI::compute() {
	double value;
	int64_t result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "i64") result = (int64_t) value;
	else if (instruction.conversion.ty2 == "i32") result = (int32_t) value;
	else if (instruction.conversion.ty2 == "i16") result = (int16_t) value;
	else if (instruction.conversion.ty2 == "i8") result = (int8_t) value;
	else if (instruction.conversion.ty2 == "i1") {
		if(value) result = 1;
		else result = 0;
	}
	instruction.general.returnRegister->setValue(&result);	
}
void
UItoFP::compute() {
	uint64_t value;
	double result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "double") result = (double) value;
	else if (instruction.conversion.ty2 == "float") result = (float) value;

	instruction.general.returnRegister->setValue(&result);
}
void
SItoFP::compute() {
	int64_t value;
	double result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "double") result = (double) value;
	else if (instruction.conversion.ty2 == "float") result = (float) value;

	instruction.general.returnRegister->setValue(&result);
}
void 
FPTrunc::compute() {
	double value;
	double result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "float") result = (float) value;

	instruction.general.returnRegister->setValue(&result);	
}
void
FPExt::compute() {
	double value;
	double result;
	if(instruction.conversion.immediate) {
		value = instruction.conversion.immVal;
		instruction.general.immediateCount->accessedRead();
	} else value = instruction.conversion.value->getValue();
	
	if (instruction.conversion.ty2 == "double") result = (float) value;

	instruction.general.returnRegister->setValue(&result);		
}
void
PtrtoInt::compute() {
	int64_t value;
	int64_t result;
	if(instruction.conversion.ty.compare(instruction.conversion.ty2) > 0) {
		if(instruction.conversion.immediate) {
			value = instruction.conversion.immVal;
			instruction.general.immediateCount->accessedRead();
		} else value = instruction.conversion.value->getValue();

		if (instruction.conversion.ty2 == "i32") result = (int32_t) value;
		else if (instruction.conversion.ty2 == "i16") result = (int16_t) value;
		else if (instruction.conversion.ty2 == "i8") result = (int8_t) value;
		else if (instruction.conversion.ty2 == "i1") {
			if(value) result = 1;
			else result = 0;
		}
		instruction.general.returnRegister->setValue(&result);
	}
	else {
		if(instruction.conversion.immediate) {
			value = instruction.conversion.immVal;
			instruction.general.immediateCount->accessedRead();
		} else value = instruction.conversion.value->getValue();
	
		if (instruction.conversion.ty2 == "i64") result = (uint64_t) value;
		else if (instruction.conversion.ty2 == "i32") result = (uint32_t) value;
		else if (instruction.conversion.ty2 == "i16") result = (uint16_t) value;
		else if (instruction.conversion.ty2 == "i8") result = (uint8_t) value;
		else if (instruction.conversion.ty2 == "i1") {
			if(value) result = 1;
			else result = 0;
		}
	instruction.general.returnRegister->setValue(&result);
	}
}
void
InttoPtr::compute() {
	int64_t value;
	int64_t result;
	if(instruction.conversion.ty.compare(instruction.conversion.ty2) > 0) {
		if(instruction.conversion.immediate) {
			value = instruction.conversion.immVal;
			instruction.general.immediateCount->accessedRead();
		} else value = instruction.conversion.value->getValue();

		if (instruction.conversion.ty2 == "i32") result = (int32_t) value;
		else if (instruction.conversion.ty2 == "i16") result = (int16_t) value;
		else if (instruction.conversion.ty2 == "i8") result = (int8_t) value;
		else if (instruction.conversion.ty2 == "i1") {
			if(value) result = 1;
			else result = 0;
		}
		instruction.general.returnRegister->setValue(&result);
	}
	else {
		if(instruction.conversion.immediate) {
			value = instruction.conversion.immVal;
			instruction.general.immediateCount->accessedRead();
		} else value = instruction.conversion.value->getValue();
	
		if (instruction.conversion.ty2 == "i64") result = (uint64_t) value;
		else if (instruction.conversion.ty2 == "i32") result = (uint32_t) value;
		else if (instruction.conversion.ty2 == "i16") result = (uint16_t) value;
		else if (instruction.conversion.ty2 == "i8") result = (uint8_t) value;
		else if (instruction.conversion.ty2 == "i1") {
			if(value) result = 1;
			else result = 0;
		}
	instruction.general.returnRegister->setValue(&result);
	}	
}
void
Bitcast::compute() { }
void 
AddrSpaceCast::compute() { }

void
ICmp::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	// uint64_t op1 = 0;
	// uint64_t op2 = 0;
	uint64_t result = 0;
	int64_t op1 = 0;
	int64_t op2 = 0;
	
	// Determine if comparison is being made between registers or immediate values
	if (instruction.other.compare.immediate1) {
		op1 = stoi(instruction.other.compare.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.other.compare.op1->getValue();
	if (instruction.other.compare.immediate2) {
		op2 = stoi(instruction.other.compare.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 =  instruction.other.compare.op2->getValue();
	// Perform Comparison
	/*
	if (op1 > MAXINT) {
		op1 = ((op1 ^ 0xFFFFFFFF)+1)*-1;
	}
	if (op2 > MAXINT) {
		op2 = ((op2 ^ 0xFFFFFFFF)+1)*-1;
	}
	DPRINTF(LLVMOp, "Op1 = %d, Op2 = %d\n", op1, op2);	
	
	if (instruction.other.compare.condition.eq) result = (op1 == op2);
	else if (instruction.other.compare.condition.ne) result = (op1 != op2);
	else if (instruction.other.compare.condition.ugt) result = (op1 > op2);
	else if (instruction.other.compare.condition.uge) result = (op1 >= op2);
	else if (instruction.other.compare.condition.ult) result = (op1 < op2);
	else if (instruction.other.compare.condition.ule) result = (op1 <= op2);
	else if (instruction.other.compare.condition.sgt) result = ((int)op1 > (int)op2);
	else if (instruction.other.compare.condition.sge) result = ((int)op1 >= (int)op2);
	else if (instruction.other.compare.condition.slt) result = ((int)op1 < (int)op2);
	else if (instruction.other.compare.condition.sle) result = ((int)op1 <= (int)op2);
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "Comparing %d and %d, result is %u.\n", op1, op2, result);
}
void
FCmp::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1 = 0.0;
	double op2 = 0.0;
	uint64_t result = 0;
	// Determine if comparison is being made between registers or immediate values
	if (instruction.other.compare.immediate1) {
		op1 = stoi(instruction.other.compare.iop1);
		instruction.general.immediateCount->accessedRead();
	} else op1 = instruction.other.compare.op1->getValue();
	if (instruction.other.compare.immediate2) {
		op2 = stoi(instruction.other.compare.iop2);
		instruction.general.immediateCount->accessedRead();
	} else op2 = instruction.other.compare.op2->getValue();
	// Perform Comparison
	if (instruction.other.compare.condition.condFalse) {
		result = false;
		DPRINTF(LLVMOp, "Always false:\n");
	} else if (instruction.other.compare.condition.condTrue) {
		result = true;
		DPRINTF(LLVMOp, "Always true:\n");				
	} else if (instruction.other.compare.condition.oeq) {
		if (!(std::isnan(op1) && std::isnan(op2))) 
		DPRINTF(LLVMOp, "Ordered Equal:\n");
		result = (op1 == op2);
	} else if (instruction.other.compare.condition.ogt) { 
		if (!(std::isnan(op1) && std::isnan(op2))) 
		result = (op1 > op2); 
		DPRINTF(LLVMOp, "Ordered Greater Then:\n");
	} else if (instruction.other.compare.condition.oge) { 
		if (!(std::isnan(op1) && std::isnan(op2)))  
		result = (op1 >= op2);  
		DPRINTF(LLVMOp, "Ordered Greater Than or Equal:\n");
	} else if (instruction.other.compare.condition.olt) { 
		if (!(std::isnan(op1) && std::isnan(op2)))  
		result = (op1 < op2);  
		DPRINTF(LLVMOp, "Ordered Less Than:\n");
	} else if (instruction.other.compare.condition.ole) { 
		if (!(std::isnan(op1) && std::isnan(op2)))  
		result = (op1 <= op2);  
		DPRINTF(LLVMOp, "Ordered Less Than or Equal:\n");
	} else if (instruction.other.compare.condition.one) { 
		if (!(std::isnan(op1) && std::isnan(op2))) 
		result = (op1 != op2);  
		DPRINTF(LLVMOp, "Ordered Not Equal:\n");
	} else if (instruction.other.compare.condition.ord) { 
		if (!(std::isnan(op1) && std::isnan(op2))) 
		 result = true; 
		 DPRINTF(LLVMOp, "Ordered:\n");
	} else if (instruction.other.compare.condition.ueq) {
		result = (op1 == op2);
		DPRINTF(LLVMOp, "Unordered. Equal to:\n");
 	} else if (instruction.other.compare.condition.ugt) {
		result = (op1 > op2);
		DPRINTF(LLVMOp, "Unordered. Greater Than:\n");
	} else if (instruction.other.compare.condition.uge) {
		result = (op1 >= op2);
		DPRINTF(LLVMOp, "Unordered. Greater or Equal:\n");
	}else if (instruction.other.compare.condition.ult) {
		result = (op1 < op2);
		DPRINTF(LLVMOp, "Unordered. Less than:\n");
	}else if (instruction.other.compare.condition.ule) {
		result = (op1 <= op2);
		DPRINTF(LLVMOp, "Unordered. Less than or equal:\n");
	}else if (instruction.other.compare.condition.une) {
		result = (op1 != op2);
		DPRINTF(LLVMOp, "Unordered. Not equal:\n");
	}else if (instruction.other.compare.condition.uno) {
		result = (op1 != op2);
		DPRINTF(LLVMOp, "Unordered:\n");
	}
	// Store result in return register
	instruction.general.returnRegister->setValue(&result);
	DPRINTF(LLVMOp, "Comparing %f and %f, result is %u.\n", op1, op2, instruction.general.returnRegister->value);
}
void
Phi::compute() {
	// <result> = phi <ty> [ <val0>, <label0>], ...
	DPRINTF(LLVMOp, "Performing %s Operation, Previous BB was #%s\n", _OpCode, prevBB);
	uint64_t val;
	for (int i = 0; i < MAXPHI; i++) {
		// Look for phi label that matches the previous basic block
		if (prevBB == instruction.other.phi.label[i]) {
			// Store the value associated with the label into val
			if (instruction.other.phi.immVal[i]) {
				val = stoi(instruction.other.phi.ival[i]);
				instruction.general.immediateCount->accessedRead();
			} else val = instruction.other.phi.val[i]->getValue();
			break;
		}
	}
	// Store val in return register
	instruction.general.returnRegister->setValue(&val);
	DPRINTF(LLVMOp, "Storing %u in Register %s\n", instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}
void 
Call::compute() { }

void
Select::compute() {
	// Currently only supports integer types but the framework for doubles and floats
	// exists within compute_node.cc and instruction.hh already
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(instruction.other.select.intTy) {
		int val1 = 0;
		int val2 = 0;
		int condition = 0;
		if(instruction.other.select.immediate[0]) {
			val1 = instruction.other.select.immVal[0];
			instruction.general.immediateCount->accessedRead();
		} else {
			val1 = instruction.other.select.val1->getValue();
		}
		if(instruction.other.select.immediate[1]) {
			val2 = instruction.other.select.immVal[1];
			instruction.general.immediateCount->accessedRead();
		} else val2 = instruction.other.select.val2->getValue();		
		
		if(instruction.other.select.icondFlag){
			if(instruction.other.select.icond) instruction.general.returnRegister->setValue(&val1);
			else instruction.general.returnRegister->setValue(&val2);
		} else {
			condition = instruction.other.select.cond->getValue();
			if(condition) instruction.general.returnRegister->setValue(&val1);
			else instruction.general.returnRegister->setValue(&val2);
		}
	DPRINTF(LLVMOp, "Selecting between [true] %d and [false] %d, based on condition [%d], %d chosen.\n", val1, val2, condition, (int) instruction.general.returnRegister->value);
	}
	DPRINTF(LLVMOp, "Storing %u in Register '%s'\n", instruction.general.returnRegister->value, instruction.general.returnRegister->getName());
}

*/

//////////////////////////

void 
Add::powerCycle() {

}

void 
Sub::powerCycle() {

}

void 
Mul::powerCycle() {

}

void 
UDiv::powerCycle() {

}

void 
SDiv::powerCycle() {

}

void 
URem::powerCycle() {

}

void 
SRem::powerCycle() {

}

void 
FAdd::powerCycle() {

}

void 
FSub::powerCycle() {

}

void 
FMul::powerCycle() {

}

void 
FDiv::powerCycle() {

}

void 
FRem::powerCycle() {

}

void 
Shl::powerCycle() {

}

void 
LShr::powerCycle() {

}

void 
AShr::powerCycle() {

}

void 
And::powerCycle() {

}

void 
Or::powerCycle() {

}

void 
Xor::powerCycle() {

}

///////////////////////////////////////////

bool 
InstructionBase::commit() {
    /*
	// If cycle count is = max cycle count, commit register value to memory
	DPRINTF(LLVMRegister, "Committing (%s) Compute Node:\n", _OpCode);
	if (instruction.general.returnRegister != NULL) {
		DPRINTF(LLVMRegister, "Attempting to Commit Register (%s)\n", instruction.general.returnRegister->getName());
		instruction.cycle.current++;
		DPRINTF(LLVMRegister, "Cycle: Current = (%d) || Max = (%d) || Remaining = (%d)\n", instruction.cycle.current, instruction.cycle.max, instruction.cycle.max - instruction.cycle.current);
		if (instruction.cycle.current >= instruction.cycle.max) {
			instruction.general.returnRegister->commit();
			DPRINTF(LLVMRegister, "Cycle Complete! Register (%s) = (%.16x)\n\n", instruction.general.returnRegister->getName(), instruction.general.returnRegister->value);
			return true;
		} else DPRINTF(LLVMRegister, "Cycle Incomplete!\n\n");
	}
	return false;
    */
}



std::vector<InstructionBase*> 
InstructionBase::checkDependency() {
	/*
    bool hot = false;
	bool phiBranchDependent = false;
	DPRINTF(LLVMRegister, "Checking Dependencies for (%s) Compute Node!\n", _OpCode);
	if(dependencies == 0) DPRINTF(LLVMRegister, "No Dependencies!\n");
	if(_OpCode == "phi"){
		DPRINTF(LLVMRegister,"Phi Instruction Detected: Previous BB = (%s)\n", prevBB);
		for (int i = 0; i < MAXPHI; i++) {
			if (prevBB == instruction.other.phi.label[i]) {
				if(!instruction.other.phi.immVal[i]) {
					phiBranchDependent = true;
					if(instruction.other.phi.val[i]->getStatus()){
						DPRINTF(LLVMRegister, "Register (%s) is Hot:\n", instruction.other.phi.val[i]->getName());
						hot = true;
					} else {
						DPRINTF(LLVMRegister, "Register (%s) is Ready:\n", instruction.other.phi.val[i]->getName());
					}
				} else {
					DPRINTF(LLVMRegister, "Immediate Value (%d) Loaded!\n", instruction.other.phi.val[i]);
				}
			}
		}
		if(!phiBranchDependent) DPRINTF(LLVMRegister, "No Dependencies!\n");
	} else {
		for (int i = 0; i < dependencies; i++) {
			DPRINTF(LLVMRegister, "Checking Dependency #%d:\n", i+1);
			// Increment counter for register dependency check
			if (instruction.dependencies.registers[i]->getStatus()) {
				DPRINTF(LLVMRegister, "Register (%s) is Hot:\n", instruction.dependencies.registers[i]->getName());
				hot = true;
			} else {
				DPRINTF(LLVMRegister, "Register (%s) is Ready:\n", instruction.dependencies.registers[i]->getName());
			}
		}
	}
	DPRINTF(LLVMRegister, "Checking Dependencies: Finished!\n\n");

	if(!hot) {
		if((instruction.general.returnRegister != NULL)) {
			DPRINTF(LLVMRegister, "Writing to Register (%s)!\n",instruction.general.returnRegister->getName());
			instruction.general.returnRegister->accessedWrite();
		}
		if(!instruction.general.terminator) {
			for(int i = 0; i < dependencies; i++) {
				DPRINTF(LLVMRegister, "Reading from register (%s)!\n", instruction.dependencies.registers[i]->getName());
				instruction.dependencies.registers[i]->accessedRead();
			}
		}
	}
	return hot;
    */
}
