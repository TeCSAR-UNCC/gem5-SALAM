#include "instructions.hh"

void 
Add::compute() {
// Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation (%s)\n", _OpCode, _ReturnRegister->getName());
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand + _Ops.at(0);
		else _Result = _Ops.at(0) + _Operand;
	} else {
		_Result = _Ops.at(0) + _Ops.at(1);
		DPRINTF(LLVMOp, "%i + %i \n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %i \n", _OpCode, _Result);
}

void 
Sub::compute() {
	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
		if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand - _Ops.at(0);
		else _Result = _Ops.at(0) - _Operand;
	}
	else _Result = _Ops.at(0) - _Ops.at(1);
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
Mul::compute() {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result
		DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand * _Ops.at(0);
		else _Result = _Ops.at(0) * _Operand;
	}
	else _Result = _Ops.at(0) * _Ops.at(1);
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
UDiv::compute() {
	// Unsigned Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
		if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (uint32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (uint32_t) _Ops.at(0);
			_Ops.at(1) = (uint32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _UOperand / _Ops.at(0);
		else _Result = _Ops.at(0) / _UOperand;
	}
	else _Result = _Ops.at(0) / _Ops.at(1);
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
SDiv::compute() {
	// Signed Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	DPRINTF(LLVMOp, "Op1: %d, Op2: %d\n", _Ops.at(0), _Ops.at(1));
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand / _Ops.at(0);
		else _Result = _Ops.at(0) / _Operand;
	}
	else _Result = _Ops.at(0) / _Ops.at(1);
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
URem::compute() {
	//Unsigned modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (uint32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (uint32_t) _Ops.at(0);
			_Ops.at(1) = (uint32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand % _Ops.at(0);
		else _Result = _Ops.at(0) % _Operand;
	}
	else _Result = _Ops.at(0) % _Ops.at(1);
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
	}

void 
SRem::compute() {
	//Signed modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand % _Ops.at(0);
		else _Result = _Ops.at(0) % _Operand;
	}
	else _Result = _Ops.at(0) % _Ops.at(1);
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
FAdd::compute() {
	// Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	double result;
	float fresult;
	// If immediate values convert from string, else load from register
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(double *)&OP1;
			if(_ImmFirst) result = _OperandDP + op1;
			else result = op1 + _OperandDP;
			setResult(&result);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			result = op1 + op2;
			setResult(&result);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, result);
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(float *)&OP1;
			if(_ImmFirst) result = _OperandSP + op1;
			else result = op1 + _OperandSP;
			fresult = (float) result;
			setResult(&fresult);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			result = op1 + op2;
			fresult = (float) result;
			setResult(&fresult);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, fresult);
		}
	}
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
	// If immediate values convert from string, else load from register
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(double *)&OP1;
			if(_ImmFirst) result = _OperandDP - op1;
			else result = op1 - _OperandDP;
			setResult(&result);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			result = op1 - op2;
			setResult(&result);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, result);
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(float *)&OP1;
			if(_ImmFirst) result = _OperandSP - op1;
			else result = op1 - _OperandSP;
			fresult = (float) result;
			setResult(&fresult);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			result = op1 - op2;
			fresult = (float) result;
			setResult(&fresult);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, fresult);
		}
	}
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
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(double *)&OP1;
			if(_ImmFirst) result = _OperandDP * op1;
			else result = op1 * _OperandDP;
			setResult(&result);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			result = op1 * op2;
			setResult(&result);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, result);
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(float *)&OP1;
			if(_ImmFirst) result = _OperandSP * op1;
			else result = op1 * _OperandSP;
			fresult = (float) result;
			setResult(&fresult);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			result = op1 * op2;
			fresult = (float) result;
			setResult(&fresult);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, fresult);
		}
	}
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
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(double *)&OP1;
			if(_ImmFirst) result = _OperandDP / op1;
			else result = op1 / _OperandDP;
			setResult(&result);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandDP, op1, result);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			result = op1 / op2;
			setResult(&result);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, result);
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Ops.at(0);
			op1 = *(float *)&OP1;
			if(_ImmFirst) result = _OperandSP / op1;
			else result = op1 / _OperandSP;
			fresult = (float) result;
			setResult(&fresult);
			if(_ImmFirst) DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
			else DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", _OperandSP, op1, fresult);
		} else {
		    uint64_t OP1 = _Ops.at(0);
		    uint64_t OP2 = _Ops.at(1);
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			result =  op1 / op2;
			fresult = (float) result;
			setResult(&fresult);
			DPRINTF(LLVMOp, "Op1: %f, Op2: %f, Result: %f\n", op1, op2, fresult);
		}
	}
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
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand << _Ops.at(0);
		else _Result = _Ops.at(0) << _Operand;
		DPRINTF(LLVMOp, "%d << %d\n", _Ops.at(0), _Operand);
	} else {
		_Result = _Ops.at(0) << _Ops.at(1);
		DPRINTF(LLVMOp, "%d << %d\n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
	
}

void 
LShr::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand >> _Ops.at(0);
		else _Result = _Ops.at(0) >> _Operand;
		DPRINTF(LLVMOp, "%d >> %d\n", _Ops.at(0), _Operand);
	} else {
		_Result = _Ops.at(0) >> _Ops.at(1);
		DPRINTF(LLVMOp, "%d >> %d\n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
AShr::compute() {
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand >> _Ops.at(0);
		else _Result = _Ops.at(0) >> _Operand;
		DPRINTF(LLVMOp, "%d >> %d\n", _Ops.at(0), _Operand);
	} else {
		_Result = _Ops.at(0) >> _Ops.at(1);
		DPRINTF(LLVMOp, "%d >> %d\n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
And::compute() {
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand & _Ops.at(0);
		else _Result = _Ops.at(0) & _Operand;
		DPRINTF(LLVMOp, "%d & %d\n", _Ops.at(0), _Operand);
	} else {
		_Result = _Ops.at(0) & _Ops.at(1);
		DPRINTF(LLVMOp, "%d & %d\n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
Or::compute() {
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand | _Ops.at(0);
		else _Result = _Ops.at(0) | _Operand;
		DPRINTF(LLVMOp, "%d | %d\n", _Ops.at(0), _Operand);
	} else {
		_Result = _Ops.at(0) | _Ops.at(1);
		DPRINTF(LLVMOp, "%d | %d\n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}

void 
Xor::compute() {
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Operands.size() == 1) {
		if(_ImmFirst) _Result = _Operand ^ _Ops.at(0);
		else _Result = _Ops.at(0) ^ _Operand;
		DPRINTF(LLVMOp, "%d ^ %d\n", _Ops.at(0), _Operand);
	} else {
		_Result = _Ops.at(0) ^ _Ops.at(1);
		DPRINTF(LLVMOp, "%d ^ %d\n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	setResult(&_Result);
	DPRINTF(LLVMOp, "%s Complete. Result = %u \n", _OpCode, _Result);
}


void
Load::compute() {
    //uint64_t src = _Pointer->getValue();
	uint64_t src = _Ops.at(0);
	_ReturnRegister->setSize(_ReturnType);
	DPRINTF(LLVMOp, "Load From: %x\n", _Ops.at(0));
	_Req = new MemoryRequest((Addr)src, _ReturnRegister->getSize());
	_Comm->enqueueRead(_Req);
}

void
Store::compute() {
	uint64_t data;
	uint64_t size = 0;
	//uint64_t dst = _Pointer->getValue();
	uint64_t dst = _Ops.at(0);
	DPRINTF(LLVMOp, "Starting Store!\n");
	if(_ImmVal) {
		data = (uint64_t) _Imm;
		//size = getSize(_ReturnType);
		size = ((std::stoi(_ReturnType.substr(1))-1)/8)+1;
		_Req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), size);
		DPRINTF(LLVMOp,"Store (Imm) Operation:(%s) Addr = %x Data = %x, Size = %u\n",_Pointer->getName(), dst, data, size);
	} else {
	    data = (uint64_t) _Ops.at(1);
        _Req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), _Value->getSize());		
		DPRINTF(LLVMOp,"Store Operation:(%s) Addr = %x Data = %x, Size = %u\n",_Pointer->getName(), dst, data, _Value->getSize());
	}
	_Comm->enqueueWrite(_Req);
}


void
Ret::compute() {

}

void
Br::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation!\n", _OpCode);
	unsigned long long int condition = 0;
	if (!(_Unconditional)) {
		DPRINTF(LLVMOp, "Conditional Branch Operation! \n");
		condition = _Condition->getValue();
		if (condition != 0) _Destination = _Branches.at(0);
		else _Destination = _Branches.at(1);
		//DPRINTF(LLVMOp, " True: %s, False: %s, Reg: %s, Condition: %d\n", instruction.terminator.iftrue, instruction.terminator.iffalse, instruction.terminator.cond->getName(), condition);
	}
	else DPRINTF(LLVMOp, "Unconditonal Branch Operation! \n"); 
	_Dest = _Destination;

}

void
LLVMSwitch::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	uint64_t mainValue = _Ops.at(0);
	bool found = false;
	DPRINTF(LLVMOp, "Register Name: %s\n", _Condition->getName());
	for(int i = 1; i < _Branches.size(); i++) {
		DPRINTF(LLVMOp, "Comparing main value %d to case value %d: \n", mainValue, _CaseValues.at(i));
		if(mainValue == _CaseValues.at(i)){
			_Dest = _Branches.at(i);
			DPRINTF(LLVMOp, "Found!\n");
			found = true;
			return;
		}
	}
	if(!found) _Dest = _Branches.at(0);
	DPRINTF(LLVMOp, "Switch selected, destination is %s!\n",_Dest);
}


void
GetElementPtr::compute() {
	// <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
	DPRINTF(LLVMOp, "Performing %s Operation (%s)\n", _OpCode, _ReturnRegister->getName());
	uint64_t elements[_Dependencies.size()];
	uint64_t currentValue[_Dependencies.size()];
	uint64_t size[_Dependencies.size()];
	uint64_t newAddress = 0;
	uint64_t dataSize = 0;
	uint64_t totalElements = 1;
	uint64_t finalCount = 0;
	DPRINTF(LLVMOp, "Base Address: %x\n", _ActivePtr);
	int j = 0;
	// Initialize array values
	for(int i = 0; i<_Dependencies.size(); i++) {
		currentValue[i] = 0;
		size[i] = 1;
		elements[i] = 1;
	}

	if(_Pty[0] == '%') { // Return Type is a custom data type
		if(_LLVMType != NULL) {
			dataSize = _LLVMType->getSize();
			DPRINTF(LLVMOp, "Custom Return Type: Size = %d\n", dataSize);
			j++;
		} 
	for(int i = 0; i <= j; i++){
			if(_Dependencies.at(i) == NULL) {  // Immediate Values
				if(i == j) {
					// Final offset variable
					totalElements*=(_ImmIdx.at(i));
				} else {				
					// Total data size in memory offset variable 
				totalElements*=(dataSize*_ImmIdx.at(i));
				}
			} else { // Register loaded values
				if(i == j) {
					// Final offset variable
					totalElements*=(_Ops.at(i));
				} else {
				totalElements*=(dataSize*_Ops.at(i));
				}
			}
			finalCount += totalElements;
			totalElements = 1;
		}
		if(_LLVMType != NULL) finalCount *=8; // Final offset calculation, total elements * memory size in bytes
		else finalCount *=4;
		newAddress += (_ActivePtr + finalCount);
		setResult(&newAddress);
		////////////////////////////////////////
	} else if(_Pty[0] == '[') { // Return type is a struct
		DPRINTF(LLVMOp, "Struct Data Type\n");
		for(int i = 0; i < _Pty.size(); i++){
			if(_Pty[i] == '[') {
				// Find the next integer value Ex: [A x [B x [C x dataSize]]] returns A, B, C
				elements[j] = stoi(_Pty.substr(i+1, _Pty.find(' ', i)-i-1));
				j++;
			}
			if(_Pty[i] == ']') break;
		}
		if(_LLVMType != NULL) {
		dataSize = _LLVMType->getSize();
		j++;
		} else {
			dataSize = 1;
		}
		for(int i = 0; i <= j; i++){
			for(int k = 0; k <= j; k++){
				totalElements*=elements[k];
			}
			if(_Dependencies.at(i) == NULL) {
				if(i == j) {
					totalElements*=(_ImmIdx.at(i));
				} else {				
				totalElements*=(dataSize*_ImmIdx.at(i));
				}
			} else {
				if(i == j) {
					totalElements*=(_Ops.at(i));
				} else {
				totalElements*=(dataSize*_Ops.at(i));
				}
			}
			finalCount += totalElements;
			totalElements = 1;
			elements[i] = 1;
		}
		DPRINTF(LLVMOp, "Potential: %x\n", _ActivePtr + finalCount);
		if(_LLVMType != NULL) finalCount *=8; // Final offset calculation, total elements * memory size in bytes
		else finalCount *=8;
		newAddress += (_ActivePtr + finalCount);
		setResult(&newAddress);
	} else { // GEP With Imm and no structs
		DPRINTF(LLVMOp, "DepSize: %d, Index: %d\n",_Dependencies.size(), _Index);
		for(int i = 0; i < _Dependencies.size(); i++) {
			if (_Pty[0] == 'i') {
				size[i] = (stoi(_Pty.substr(1))) / 8;
			} else if (_Pty.find("double") == 0) {
				DPRINTF(LLVMOp, "Double Pointer\n");
				size[i] = 8;
			} else if (_Pty.find("float") == 0) {
				size[i] = 4;
			}
		}
		for (int i = 0; i < _Index; i++) {
			if(_Dependencies.at(i) != NULL) currentValue[i] = _Ops.at(i);
			else {
				currentValue[i] = _ImmIdx.at(i);
			}
		}
		DPRINTF(LLVMOp, "Current Value: %x\n",currentValue[0]);
		for (int i = 0; i < _Index; i++) {
			newAddress = newAddress + currentValue[i]*size[i];
		}
		newAddress += _ActivePtr;
		setResult(&newAddress);
	}
	DPRINTF(LLVMOp, "New Address: %x\n", newAddress);
}


void
Trunc::compute() {
	if (_OriginalType == "i32") _Result = 0xffffffff & _Ops.at(0);
	else if (_OriginalType == "i16") _Result = 0xffff & _Ops.at(0);
	else if (_OriginalType == "i8") _Result = 0xff & _Ops.at(0);
	else if (_OriginalType == "i1") {
		if(_Ops.at(0)) _Result = 1;
		else _Result = 0;
	}
	setResult(&_Result);
	DPRINTF(LLVMOp, "Trunc:(%s) %d from %d\n", _ReturnType, _Result, _Ops.at(0));
}

void
ZExt::compute() {
	if (_OriginalType == "i64") _Result = (uint64_t) _Ops.at(0);
	else if (_OriginalType == "i32") _Result = (uint32_t) _Ops.at(0);
	else if (_OriginalType == "i16") _Result = (uint16_t) _Ops.at(0);
	else if (_OriginalType == "i8") _Result = (uint8_t) _Ops.at(0);
	else if (_OriginalType == "i1") {
		if(_Ops.at(0)) _Result = 1;
		else _Result = 0;
	}
	setResult(&_Result); 
	DPRINTF(LLVMOp, "ZExt:(%s) %d from %d\n", _OriginalType, _Result, _Ops.at(0));	
}

void
SExt::compute() {
	if (_OriginalType == "i64") _Result = (int64_t) _Ops.at(0);
	else if (_OriginalType == "i32") _Result = (int32_t) _Ops.at(0);
	else if (_OriginalType == "i16") _Result = (int16_t) _Ops.at(0);
	else if (_OriginalType == "i8") _Result = (int8_t) _Ops.at(0);
	else if (_OriginalType == "i1") {
		if(_Ops.at(0)) _Result = 1;
		else _Result = 0;
	}
	setResult(&_Result);
	DPRINTF(LLVMOp, "SExt:(%s) %d from %d\n", _OriginalType, _Result, _Ops.at(0)); 	
}

void 
FPToUI::compute() {
	if (_OriginalType == "i64") _Result = (uint64_t) _Ops.at(0);
	else if (_OriginalType == "i32") _Result = (uint32_t) _Ops.at(0);
	else if (_OriginalType == "i16") _Result = (uint16_t) _Ops.at(0);
	else if (_OriginalType == "i8") _Result = (uint8_t) _Ops.at(0);
	else if (_OriginalType == "i1") {
		if(_Ops.at(0)) _Result = 1;
		else _Result = 0;
	}
	setResult(&_Result);
}
void
FPToSI::compute() {
	if (_OriginalType == "i64") _Result = (int64_t) _Ops.at(0);
	else if (_OriginalType == "i32") _Result = (int32_t) _Ops.at(0);
	else if (_OriginalType == "i16") _Result = (int16_t) _Ops.at(0);
	else if (_OriginalType == "i8") _Result = (int8_t) _Ops.at(0);
	else if (_OriginalType == "i1") {
		if(_Ops.at(0)) _Result = 1;
		else _Result = 0;
	}
	setResult(&_Result);
}
void
UIToFP::compute() {
	if (_OriginalType == "double") _Result = (double) _Ops.at(0);
	else if (_OriginalType == "float") _Result = (float) _Ops.at(0);
	setResult(&_Result);
}
void
SIToFP::compute() {
	if (_OriginalType == "double") _Result = (double) _Ops.at(0);
	else if (_OriginalType == "float") _Result = (float) _Ops.at(0);
	setResult(&_Result);
}
void 
FPTrunc::compute() {
	if (_OriginalType == "float") _Result = (float) _Ops.at(0);
	setResult(&_Result);	
}
void
FPExt::compute() {
	if (_OriginalType == "double") _Result = (double) _Ops.at(0);
	setResult(&_Result);
}
void
PtrToInt::compute() { }
void
IntToPtr::compute() { }
void
BitCast::compute() { }
void 
AddrSpaceCast::compute() { }

void
FCmp::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	// Determine if comparison is being made between registers or immediate values
	if (_Ops.size() == 1) {
		if(_Flags & CONDFALSE) _Result = 0;
		else if(_Flags & OEQ) _Result = (_Ops.at(0) == _OperandDP);
		else if(_Flags & OGT) _Result = (_Ops.at(0) > _OperandDP);
		else if(_Flags & OGE) _Result = (_Ops.at(0) >= _OperandDP);
		else if(_Flags & OLT) _Result = (_Ops.at(0) < _OperandDP);
		else if(_Flags & OLE) _Result = (_Ops.at(0) <= _OperandDP);
		else if(_Flags & ONE) _Result = (_Ops.at(0) != _OperandDP);
		else if(_Flags & ORD) _Result = (_Ops.at(0) && _OperandDP);
		else if(_Flags & UEQ) _Result = (_Ops.at(0) == _OperandDP);
		else if(_Flags & UGT) _Result = (_Ops.at(0) > _OperandDP);
		else if(_Flags & UGE) _Result = (_Ops.at(0) >= _OperandDP);
		else if(_Flags & ULT) _Result = (_Ops.at(0) < _OperandDP);
		else if(_Flags & ULE) _Result = (_Ops.at(0) <= _OperandDP);
		else if(_Flags & UNE) _Result = (_Ops.at(0) != _OperandDP);
		else if(_Flags & UNO) _Result = (_Ops.at(0) && _OperandDP);
		else if(_Flags & CONDTRUE) _Result = 1;
	} else {
		if(_Flags & CONDFALSE) _Result = 0;
		else if(_Flags & OEQ) _Result = (_Ops.at(0) == _Ops.at(1));
		else if(_Flags & OGT) _Result = (_Ops.at(0) > _Ops.at(1));
		else if(_Flags & OGE) _Result = (_Ops.at(0) >= _Ops.at(1));
		else if(_Flags & OLT) _Result = (_Ops.at(0) < _Ops.at(1));
		else if(_Flags & OLE) _Result = (_Ops.at(0) <= _Ops.at(1));
		else if(_Flags & ONE) _Result = (_Ops.at(0) != _Ops.at(1));
		else if(_Flags & ORD) _Result = (_Ops.at(0) && _Ops.at(1));
		else if(_Flags & UEQ) _Result = (_Ops.at(0) == _Ops.at(1));
		else if(_Flags & UGT) _Result = (_Ops.at(0) > _Ops.at(1));
		else if(_Flags & UGE) _Result = (_Ops.at(0) >= _Ops.at(1));
		else if(_Flags & ULT) _Result = (_Ops.at(0) < _Ops.at(1));
		else if(_Flags & ULE) _Result = (_Ops.at(0) <= _Ops.at(1));
		else if(_Flags & UNE) _Result = (_Ops.at(0) != _Ops.at(1));
		else if(_Flags & UNO) _Result = (_Ops.at(0) && _Ops.at(1));
		else if(_Flags & CONDTRUE) _Result = 1;
	}
	// Store result in return register
	setResult(&_Result);
	//DPRINTF(LLVMOp, "Comparing %d and %d, result is %u.\n", op1, op2, result);
}
void
ICmp::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation (%x)\n", _OpCode, _Flags);
	if(_ReturnType == "i32") {
		if (_Operands.size() == 1) {
			//if(0x80000000 & _Ops.at(0)) { }
			_Ops.at(0) = (int32_t) _Ops.at(0);
		} else {
			_Ops.at(0) = (int32_t) _Ops.at(0);
			_Ops.at(1) = (int32_t) _Ops.at(1);
		}
	}
	if (_Ops.size() == 1) {
		if(_Flags & EQ) _Result = (_Ops.at(0) == _Operand);
		else if(_Flags & NE) _Result = (_Ops.at(0) != _Operand);
		else if(_Flags & UGT) _Result = ((uint64_t)_Ops.at(0) > _UOperand);
		else if(_Flags & UGE) _Result = ((uint64_t)_Ops.at(0) >= _UOperand);
		else if(_Flags & ULT) _Result = ((uint64_t)_Ops.at(0) < _UOperand);
		else if(_Flags & ULE) _Result = ((uint64_t)_Ops.at(0) <= _UOperand);
		else if(_Flags & SGT) _Result = (_Ops.at(0) > _SOperand);
		else if(_Flags & SGE) _Result = (_Ops.at(0) >= _SOperand);
		else if(_Flags & SLT) _Result = (_Ops.at(0) < _SOperand);
		else if(_Flags & SLE) _Result = (_Ops.at(0) <= _SOperand);
		DPRINTF(LLVMOp, "%u compared to %u\n", _Ops.at(0), _Operand);
	} else {
		if(_Flags & EQ) _Result = (_Ops.at(0) == _Ops.at(1));
		else if(_Flags & NE) _Result = (_Ops.at(0) != _Ops.at(1));
		else if(_Flags & UGT) _Result = ((uint64_t)_Ops.at(0) > (uint64_t)_Ops.at(1));
		else if(_Flags & UGE) _Result = ((uint64_t)_Ops.at(0) >= (uint64_t)_Ops.at(1));
		else if(_Flags & ULT) _Result = ((uint64_t)_Ops.at(0) < (uint64_t)_Ops.at(1));
		else if(_Flags & ULE) _Result = ((uint64_t)_Ops.at(0) <= (uint64_t)_Ops.at(1));
		else if(_Flags & SGT) _Result = (_Ops.at(0) > _Ops.at(1));
		else if(_Flags & SGE) _Result = (_Ops.at(0) >= _Ops.at(1));
		else if(_Flags & SLT) _Result = (_Ops.at(0) < _Ops.at(1));
		else if(_Flags & SLE) _Result = (_Ops.at(0) <= _Ops.at(1));
		DPRINTF(LLVMOp, "%u compared to %u\n", _Ops.at(0), _Ops.at(1));
	}
	// Store result in return register
	//uint64_t finalResult = (uint64_t) _Result;
	setResult(&_Result);

	DPRINTF(LLVMOp, "Result: %d\n", _Result&&0x1);
}

void
Phi::compute() {
	// <result> = phi <ty> [ <val0>, <label0>], ...
	DPRINTF(LLVMOp, "Performing %s Operation, Previous BB was #%s\n", _OpCode, _PrevBB);
	for(auto i = 0; i < _PhiVal.size(); ++i) {
		if (_PhiLabel.at(i) == _PrevBB) {
			if(_PhiVal.at(i) == "reg") {
				_Result = _PhiReg.at(i)->getValue();
				break;
			} else if(_ReturnType[0] == 'i') {
				_Result = std::stoi(_PhiVal.at(i));
				break;
			}
		}
	}
	setResult(&_Result);
	DPRINTF(LLVMOp, "Storing %u in Register %s\n", _Result, _ReturnRegister->getName());
}

std::vector<Register*>
Phi::runtimeDependencies(std::string PrevBB) {
	std::vector<Register*> dependencies;
	_PrevBB = PrevBB;
	for(auto i = 0; i < _PhiVal.size(); ++i) {
		if (_PhiLabel.at(i) == _PrevBB) {
			if(_PhiVal.at(i) == "reg") dependencies.push_back(_PhiReg.at(i));
		}
	}
	return dependencies;
}

void
Select::compute() {
	// Currently only supports integer types but the framework for doubles and floats
	// exists within compute_node.cc and instruction.hh already
	DPRINTF(LLVMOp, "Performing %s Operation (%s)\n", _OpCode, _ReturnRegister->getName());
	
	if(_ReturnType[0] == 'i') {
		if(_Ops.at(0)) {
			DPRINTF(LLVMOp, "True Condition!\n");
			if(_Imm.at(0))  _Result = _ImmValues.at(0);
			else _Result = _Ops.at(1);
		} else {
			DPRINTF(LLVMOp, "False Condition!\n"); 		
			if(_Imm.at(1)) _Result = _ImmValues.at(1);
			else if(_Imm.at(0)) _Result = _Ops.at(1);
			else _Result = _Ops.at(2);
		}
	}
	DPRINTF(LLVMOp, "Selected Value: %i\n", _Result);	
	setResult(&_Result);		
}
