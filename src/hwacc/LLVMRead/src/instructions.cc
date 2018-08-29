#include "instructions.hh"

void 
Add::compute() {
// Integer Addition
	// <result> = add <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	// If immediate values convert from string, else load from register
	DPRINTF(LLVMOp, "Size (%d), Imm (%d)\n", _Operands.size(), _Operand);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() + _Operand;
	else _Result = _Operands.at(0)->getValue() + _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	
	DPRINTF(LLVMOp, "%u: Stored in Register %s. \n", _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
Sub::compute() {
	// Subtract Instruction
	// <result> = sub <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() - _Operand;
	else _Result = _Operands.at(0)->getValue() - _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u - %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
Mul::compute() {
	// Multiply Instruction
	// <result> = mul <ty> <op1>, <op2>          ; yields ty:result
	// <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
	// <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() * _Operand;
	else _Result = _Operands.at(0)->getValue() * _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u * %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
UDiv::compute() {
	// Unsigned Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() / _UOperand;
	else _Result = _Operands.at(0)->getValue() / _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
SDiv::compute() {
	// Signed Division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() / _SOperand;
	else _Result = _Operands.at(0)->getValue() / _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
URem::compute() {
	//Unsigned modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() % _UOperand;
	else _Result = _Operands.at(0)->getValue() % _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u %% %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
SRem::compute() {
	//Signed modulo division
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() % _SOperand;
	else _Result = _Operands.at(0)->getValue() % _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u %% %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
FAdd::compute() {
	// Floating point Addition
	// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	double result;
	// If immediate values convert from string, else load from register
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(double *)&OP1;
			result = op1 + _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			result = op1 + op2;
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(float *)&OP1;
			result = op1 + _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			result = op1 + op2;
		}
	}
	DPRINTF(LLVMOp, "%f + %f = %f \n", op1, op2, result);
	_ReturnRegister->setValue(&result);
	//DPRINTF(LLVMOp, "%u + %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
FSub::compute() {
	// Floating point Subtraction
	// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	// If immediate values convert from string, else load from register
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(double *)&OP1;
			_Result = op1 - _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			_Result = op1 - op2;
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(float *)&OP1;
			_Result = op1 - _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			_Result = op1 - op2;
		}
	}
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%f - %f = %f: Stored in Register %s. \n", op1, op2, result, _ReturnRegister->getName());
}

void 
FMul::compute() {
	// Floating point Multiplication
	// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	// If immediate values convert from string, else load from register
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(double *)&OP1;
			_Result = op1 * _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			_Result = op1 * op2;
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(float *)&OP1;
			_Result = op1 * _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			_Result = op1 * op2;
		}
	}
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u * %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
FDiv::compute() {
	// Floating point Division
	// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	double op1;
	double op2;
	// If immediate values convert from string, else load from register
	if (_ReturnType.find("double") == 0) {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(double *)&OP1;
			_Result = op1 / _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(double *)&OP1;
			op2 = *(double *)&OP2;
			_Result = op1 / op2;
		}
	} else {
		if (_Operands.size() == 1) {
			uint64_t OP1 = _Operands.at(0)->getValue();
			op1 = *(float *)&OP1;
			_Result = op1 / _OperandDP;
		} else {
		    uint64_t OP1 = _Operands.at(0)->getValue();
		    uint64_t OP2 = _Operands.at(1)->getValue();
			op1 = *(float *)&OP1;
			op2 = *(float *)&OP2;
			_Result = op1 / op2;
		}
	}
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u / %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
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
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() << _Operand;
	else _Result = _Operands.at(0)->getValue() << _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u << %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
LShr::compute() {
	// Logical Shift Right Operation
	// <result> = lshr <ty> <op1>, <op2>; yields ty : result
	// <result> = lshr exact <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() >> _Operand;
	else _Result = _Operands.at(0)->getValue() >> _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u >> %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
AShr::compute() {
	// Arithmatic Shift Right Operation
	// <result> = ashr <ty> <op1>, <op2>; yields ty : result
	// <result> = ashr exact <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() >> _Operand;
	else _Result = _Operands.at(0)->getValue() >> _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u >> %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
And::compute() {
	// And Operation
	// <result> = and <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() & _Operand;
	else _Result = _Operands.at(0)->getValue() & _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u & %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
Or::compute() {
	// Or Operation
	// <result> = or <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() | _Operand;
	else _Result = _Operands.at(0)->getValue() | _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u | %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void 
Xor::compute() {
	// Xor Operation
	// <result> = xor <ty> <op1>, <op2>; yields ty : result
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if (_Operands.size() == 1) _Result = _Operands.at(0)->getValue() ^ _Operand;
	else _Result = _Operands.at(0)->getValue() ^ _Operands.at(1)->getValue();
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "%u ^ %u = %u: Stored in Register %s. \n", op1, op2, _ReturnRegister->getValue(), _ReturnRegister->getName());
}


void
Load::compute() {
    uint64_t src = _Pointer->getValue();
	_ReturnRegister->setSize(_ReturnType);
	//DPRINTF(LLVMGEP,"Load Operation: Name = %s, Size = %d\n", instruction.memory.load.pointer->getName(), _ReturnRegister->size);
	_Req = new MemoryRequest((Addr)src, _ReturnRegister->getSize());
	_Comm->enqueueRead(_Req);
}

void
Store::compute() {
	uint64_t data;
	uint64_t size = 0;
	uint64_t dst = _Pointer->getValue();
	if(_Imm != 0) {
		data = (uint64_t) _Imm;
		//size = getSize(_ReturnType);
		size = 8;
		_Req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), size);
	} else {
	    data = _Value->getValue();
        _Req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), _Value->getSize());
		//DPRINTF(LLVMGEP,"Store Operation: Type = %s, Size = %d\n", instruction.memory.store.value->getType(), instruction.memory.store.value->size);
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
Switch::compute() {
	
}


void
GetElementPtr::compute() {
	// <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
	DPRINTF(LLVMGEP, "Performing %s Operation (%s)\n", _OpCode, _ReturnRegister->getName());
	uint64_t elements[_Idx.size()];
	uint64_t currentValue[_Idx.size()];
	uint64_t size[_Idx.size()];
	uint64_t newAddress = 0;
	uint64_t dataSize = 0;
	uint64_t totalElements = 1;
	uint64_t finalCount = 0;
	
	int j = 0;
	// Initialize array values
	for(int i = 0; i<_Idx.size(); i++) {
		currentValue[i] = 0;
		size[i] = 1;
		elements[i] = 1;
	}

	if(_Pty[0] == '%') { // Return Type is a custom data type
		if(_LLVMType != NULL) {
			dataSize = _LLVMType->getSize();
			j++;
		} 
	for(int i = 0; i <= j; i++){
			if(_Idx.at(i) == NULL) {  // Immediate Values
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
					totalElements*=(_Idx.at(i)->getValue());
				} else {
				totalElements*=(dataSize*_Idx.at(i)->getValue());
				}
			}
			finalCount += totalElements;
			totalElements = 1;
		}
		if(_LLVMType != NULL) finalCount *=8; // Final offset calculation, total elements * memory size in bytes
		else finalCount *=4;
		newAddress += (_PtrVal->getValue() + finalCount);
		_ReturnRegister->setValue(&newAddress);
		////////////////////////////////////////
	} else if(_Pty[0] == '[') { // Return type is a struct
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
			if(_Idx.at(i) == NULL) {
				if(i == j) {
					totalElements*=(_ImmIdx.at(i));
				} else {				
				totalElements*=(dataSize*_ImmIdx.at(i));
				}
			} else {
				if(i == j) {
					totalElements*=(_Idx.at(i)->getValue());
				} else {
				totalElements*=(dataSize*_Idx.at(i)->getValue());
				}
			}
			finalCount += totalElements;
			totalElements = 1;
			elements[i] = 1;
		}

		if(_LLVMType != NULL) finalCount *=8; // Final offset calculation, total elements * memory size in bytes
		else finalCount *=4;
		newAddress += (_PtrVal->getValue() + finalCount);
		_ReturnRegister->setValue(&newAddress);
	} else {
		for(int i = 0; i < _Idx.size(); i++) {
			if (_Pty[i] == 'i') {
				size[i] = (stoi(_Pty.substr(1))) / 8;
			} else if (_Pty.find("double") == 0) {
				size[i] = 8;
			} else if (_Pty.find("float") == 0) {
				size[i] = 4;
			}
		}
		for (int i = 0; i < _Index; i++) {
			if(_Idx.at(i) != NULL) currentValue[i] = _Idx.at(i)->getValue();
			else {
				currentValue[i] = _ImmIdx.at(i);
			}
		}

		for (int i = 0; i < _Index; i++) {
			newAddress = newAddress + currentValue[i]*size[i];
		}
		newAddress += _PtrVal->getValue();
		_ReturnRegister->setValue(&newAddress);
	}
}


void
Trunc::compute() {
	if (_ReturnType == "i32") _Result = 0xffffffff & _COperand->getValue();
	else if (_ReturnType == "i16") _Result = 0xffff & _COperand->getValue();
	else if (_ReturnType == "i8") _Result = 0xff & _COperand->getValue();
	else if (_ReturnType == "i1") {
		if(_COperand->getValue()) _Result = 1;
		else _Result = 0;
	}
	_ReturnRegister->setValue(&_Result);
}

void
ZExt::compute() {
	if (_ReturnType == "i64") _Result = (uint64_t) _COperand->getValue();
	else if (_ReturnType == "i32") _Result = (uint32_t) _COperand->getValue();
	else if (_ReturnType == "i16") _Result = (uint16_t) _COperand->getValue();
	else if (_ReturnType == "i8") _Result = (uint8_t) _COperand->getValue();
	else if (_ReturnType == "i1") {
		if(_COperand->getValue()) _Result = 1;
		else _Result = 0;
	}
	_ReturnRegister->setValue(&_Result); 	
}

void
SExt::compute() {
	if (_ReturnType == "i64") _Result = (int64_t) _COperand->getValue();
	else if (_ReturnType == "i32") _Result = (int32_t) _COperand->getValue();
	else if (_ReturnType == "i16") _Result = (int16_t) _COperand->getValue();
	else if (_ReturnType == "i8") _Result = (int8_t) _COperand->getValue();
	else if (_ReturnType == "i1") {
		if(_COperand->getValue()) _Result = 1;
		else _Result = 0;
	}
	_ReturnRegister->setValue(&_Result); 	
}

void 
FPToUI::compute() {
	if (_ReturnType == "i64") _Result = (uint64_t) _COperand->getValue();
	else if (_ReturnType == "i32") _Result = (uint32_t) _COperand->getValue();
	else if (_ReturnType == "i16") _Result = (uint16_t) _COperand->getValue();
	else if (_ReturnType == "i8") _Result = (uint8_t) _COperand->getValue();
	else if (_ReturnType == "i1") {
		if(_COperand->getValue()) _Result = 1;
		else _Result = 0;
	}
	_ReturnRegister->setValue(&_Result);
}
void
FPToSI::compute() {
	if (_ReturnType == "i64") _Result = (int64_t) _COperand->getValue();
	else if (_ReturnType == "i32") _Result = (int32_t) _COperand->getValue();
	else if (_ReturnType == "i16") _Result = (int16_t) _COperand->getValue();
	else if (_ReturnType == "i8") _Result = (int8_t) _COperand->getValue();
	else if (_ReturnType == "i1") {
		if(_COperand->getValue()) _Result = 1;
		else _Result = 0;
	}
	_ReturnRegister->setValue(&_Result);
}
void
UIToFP::compute() {
	if (_ReturnType == "double") _Result = (double) _COperand->getValue();
	else if (_ReturnType == "float") _Result = (float) _COperand->getValue();
	_ReturnRegister->setValue(&_Result);
}
void
SIToFP::compute() {
	if (_ReturnType == "double") _Result = (double) _COperand->getValue();
	else if (_ReturnType == "float") _Result = (float) _COperand->getValue();
	_ReturnRegister->setValue(&_Result);
}
void 
FPTrunc::compute() {
	if (_ReturnType == "float") _Result = (float) _COperand->getValue();
	_ReturnRegister->setValue(&_Result);	
}
void
FPExt::compute() {
	if (_ReturnType == "double") _Result = (double) _COperand->getValue();
	_ReturnRegister->setValue(&_Result);
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
	if (_Operands.size() == 1) {
		if(_Flags & CONDFALSE) _Result = 0;
		else if(_Flags & OEQ) _Result = (_Operands.at(0)->getValue() == _OperandDP);
		else if(_Flags & OGT) _Result = (_Operands.at(0)->getValue() > _OperandDP);
		else if(_Flags & OGE) _Result = (_Operands.at(0)->getValue() >= _OperandDP);
		else if(_Flags & OLT) _Result = (_Operands.at(0)->getValue() < _OperandDP);
		else if(_Flags & OLE) _Result = (_Operands.at(0)->getValue() <= _OperandDP);
		else if(_Flags & ONE) _Result = (_Operands.at(0)->getValue() != _OperandDP);
		else if(_Flags & ORD) _Result = (_Operands.at(0)->getValue() && _OperandDP);
		else if(_Flags & UEQ) _Result = (_Operands.at(0)->getValue() == _OperandDP);
		else if(_Flags & UGT) _Result = (_Operands.at(0)->getValue() > _OperandDP);
		else if(_Flags & UGE) _Result = (_Operands.at(0)->getValue() >= _OperandDP);
		else if(_Flags & ULT) _Result = (_Operands.at(0)->getValue() < _OperandDP);
		else if(_Flags & ULE) _Result = (_Operands.at(0)->getValue() <= _OperandDP);
		else if(_Flags & UNE) _Result = (_Operands.at(0)->getValue() != _OperandDP);
		else if(_Flags & UNO) _Result = (_Operands.at(0)->getValue() && _OperandDP);
		else if(_Flags & CONDTRUE) _Result = 1;
	} else {
		if(_Flags & CONDFALSE) _Result = 0;
		else if(_Flags & OEQ) _Result = (_Operands.at(0)->getValue() == _Operands.at(1)->getValue());
		else if(_Flags & OGT) _Result = (_Operands.at(0)->getValue() > _Operands.at(1)->getValue());
		else if(_Flags & OGE) _Result = (_Operands.at(0)->getValue() >= _Operands.at(1)->getValue());
		else if(_Flags & OLT) _Result = (_Operands.at(0)->getValue() < _Operands.at(1)->getValue());
		else if(_Flags & OLE) _Result = (_Operands.at(0)->getValue() <= _Operands.at(1)->getValue());
		else if(_Flags & ONE) _Result = (_Operands.at(0)->getValue() != _Operands.at(1)->getValue());
		else if(_Flags & ORD) _Result = (_Operands.at(0)->getValue() && _Operands.at(1)->getValue());
		else if(_Flags & UEQ) _Result = (_Operands.at(0)->getValue() == _Operands.at(1)->getValue());
		else if(_Flags & UGT) _Result = (_Operands.at(0)->getValue() > _Operands.at(1)->getValue());
		else if(_Flags & UGE) _Result = (_Operands.at(0)->getValue() >= _Operands.at(1)->getValue());
		else if(_Flags & ULT) _Result = (_Operands.at(0)->getValue() < _Operands.at(1)->getValue());
		else if(_Flags & ULE) _Result = (_Operands.at(0)->getValue() <= _Operands.at(1)->getValue());
		else if(_Flags & UNE) _Result = (_Operands.at(0)->getValue() != _Operands.at(1)->getValue());
		else if(_Flags & UNO) _Result = (_Operands.at(0)->getValue() && _Operands.at(1)->getValue());
		else if(_Flags & CONDTRUE) _Result = 1;
	}
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "Comparing %d and %d, result is %u.\n", op1, op2, result);
}
void
ICmp::compute() {
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
		if (_Operands.size() == 1) {
		if(_Flags & EQ) _Result = (_Operands.at(0)->getValue() == _Operand);
		else if(_Flags & NE) _Result = (_Operands.at(0)->getValue() != _Operand);
		else if(_Flags & UGT) _Result = (_Operands.at(0)->getValue() > _UOperand);
		else if(_Flags & UGE) _Result = (_Operands.at(0)->getValue() >= _UOperand);
		else if(_Flags & ULT) _Result = (_Operands.at(0)->getValue() < _UOperand);
		else if(_Flags & ULE) _Result = (_Operands.at(0)->getValue() <= _UOperand);
		else if(_Flags & SGT) _Result = (_Operands.at(0)->getValue() > _SOperand);
		else if(_Flags & SGE) _Result = (_Operands.at(0)->getValue() >= _SOperand);
		else if(_Flags & SLT) _Result = (_Operands.at(0)->getValue() < _SOperand);
		else if(_Flags & SLE) _Result = (_Operands.at(0)->getValue() <= _SOperand);
	} else {
		if(_Flags & EQ) _Result = (_Operands.at(0)->getValue() == _Operands.at(1)->getValue());
		else if(_Flags & NE) _Result = (_Operands.at(0)->getValue() != _Operands.at(1)->getValue());
		else if(_Flags & UGT) _Result = (_Operands.at(0)->getValue() > _Operands.at(1)->getValue());
		else if(_Flags & UGE) _Result = (_Operands.at(0)->getValue() >= _Operands.at(1)->getValue());
		else if(_Flags & ULT) _Result = (_Operands.at(0)->getValue() < _Operands.at(1)->getValue());
		else if(_Flags & ULE) _Result = (_Operands.at(0)->getValue() <= _Operands.at(1)->getValue());
		else if(_Flags & SGT) _Result = (_Operands.at(0)->getValue() > _Operands.at(1)->getValue());
		else if(_Flags & SGE) _Result = (_Operands.at(0)->getValue() >= _Operands.at(1)->getValue());
		else if(_Flags & SLT) _Result = (_Operands.at(0)->getValue() < _Operands.at(1)->getValue());
		else if(_Flags & SLE) _Result = (_Operands.at(0)->getValue() <= _Operands.at(1)->getValue());
	}
	// Store result in return register
	_ReturnRegister->setValue(&_Result);
	DPRINTF(LLVMOp, "Result: %d\n", _Result);
}
void
Phi::compute() {
	// <result> = phi <ty> [ <val0>, <label0>], ...
	DPRINTF(LLVMOp, "Performing %s Operation, Previous BB was #%s\n", _OpCode, _PrevBB);
	for(auto i = 0; i < _PhiVal.size(); ++i) {
		if (_PhiLabel.at(i) == _PrevBB) {
			if(_PhiVal.at(i) == "reg") _Result = _PhiReg.at(i)->getValue();
			if(_ReturnType[0] == 'i') _Result = stoi(_PhiVal.at(i));
		}
	}
	_ReturnRegister->setValue(&_Result);
	//DPRINTF(LLVMOp, "Storing %u in Register %s\n", _ReturnRegister->getValue(), _ReturnRegister->getName());
}

void
Select::compute() {
	// Currently only supports integer types but the framework for doubles and floats
	// exists within compute_node.cc and instruction.hh already
	DPRINTF(LLVMOp, "Performing %s Operation\n", _OpCode);
	if(_ReturnType[0] == 'i') {
		if(_Imm.at(0)) {
			if(_Imm.at(1)) {
				if(_Condition->getValue()) _Result = (_ImmValues.at(0));
				else _Result = (_ImmValues.at(1));
			} else {
				if(_Condition->getValue()) _Result = (_ImmValues.at(0));
				else _Result = (_RegValues.at(1)->getValue());
			}
		} else { 		
			if(_Imm.at(1)) {
				if(_Condition->getValue()) _Result = (_RegValues.at(0)->getValue());
				else _Result = (_ImmValues.at(1));
			} else {
				if(_Condition->getValue()) _Result = (_RegValues.at(0)->getValue());
				else _Result = (_RegValues.at(1)->getValue());
			}
		}
	}	
	_ReturnRegister->setValue(&_Result);		
}


bool 
InstructionBase::commit() {
	// If cycle count is = max cycle count, commit register value to memory
	if (_ReturnRegister != NULL) {
		_CurrCycle++;
		if (_CurrCycle >= _MaxCycle) {
			_ReturnRegister->commit();
			return true;
		} else DPRINTF(LLVMRegister, "Cycle Incomplete!\n\n");
	}
	return false;
}



bool 
InstructionBase::checkDependencies() {
    bool hot = false;
//	bool phiBranchDependent = false;
	if(_Dependencies.size() == 0) DPRINTF(LLVMRegister, "No Dependencies!\n");
	if(_OpCode == "phi"){
		/*
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
		*/
	} else {
		for (int i = 0; i < _Dependencies.size(); i++) {
			// Increment counter for register dependency check
			if (_Dependencies.at(i)->getStatus()) {
				hot = true;
			} 
		}
	}
	return hot;
}
