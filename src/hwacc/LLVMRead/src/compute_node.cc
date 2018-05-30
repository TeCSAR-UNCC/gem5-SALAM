#include "compute_node.hh"

ComputeNode::ComputeNode(std::string line, RegisterList *list, std::string prev, CommInterface *co, TypeList *typeList) {
	std::vector<std::string> parameters;
	int leftDelimeter, rightDelimeter, lastInLine;
	int returnChk = line.find(" = ");
	int last = 0;
	instruction.general.llvm_Line = line;
	instruction.cycle.max = 1;
	comm = co;
	prevBB = prev;
	// ////////////////////////////////////////////////////////////////////
	
	// Find the return register. If it exists, it is always the first component of the line
	if (returnChk > 0) {
	    std::string ret_name = line.substr((line.find("%") + 1), returnChk - 3);
	    Register * ret_reg = list->findRegister(ret_name);
		//Check if register already exists
		if(ret_reg == NULL) {
		    // Create new pointer to the return register
		    instruction.general.returnRegister = new Register(ret_name);
		    list->addRegister(instruction.general.returnRegister);
		    DPRINTF(ComputeNode, "Creating Return Register: (%s)\n", instruction.general.returnRegister->getName());
		} else {
		    instruction.general.returnRegister = ret_reg;
		}
		// In all instances where a return register is the first component, the next component is
		// the opcode, which is parsed and removed from line
		line = line.substr(returnChk + 3);
		instruction.general.opCode = line.substr(0, line.find(' '));
		line = line.substr(line.find(' '));
		
	} else {
		// If no return register is found then the first component must instead be the opcode
		// as of LLVM 3.4 instruction types
		// Search for first none empty space which is were the opcode must begin
		// Then store the opcode and remove the parsed information from the line
		for (int i = 0; i < line.length(); i++) {
			if (line[i] != ' ') {
				instruction.general.opCode = line.substr(i, line.find(' ', i) - 2);
				line = line.substr(line.find(' ') + 1);
				i = line.length();
			}
		}
		// No return register needed
		DPRINTF(ComputeNode, "No Return Register Needed!\n");
		instruction.general.returnRegister = NULL;
	}
	// //////////////////////////////////////////////////////////////////////////////////

	DPRINTF(ComputeNode, "Opcode Found: (%s)\n", instruction.general.opCode);
	// Loop to break apart each component of the LLVM line
	// Any brackets or braces that contain data are stored as under the parent set
	// Components are pushed back into the parameter vector in order they are read
	// This vector is initialized after the return register and opcode are found
	// and the line at this point already has those components removed if they
	// exist.

	for (int i = 0; i < line.length(); i++) {
		// Looks until the loop finds a non-space character
		if (line[i] != ' ') {
			// Each of these functions preform the same action using a different
			// bracket or brace as the searchable character. Since the loop looks
			// through the line sequentially no type of brace or bracket takes
			// precendence over the others, it simply finds the first instance
			// and matches the component to the correct closing character and stores
			// the entire encapsulated component and its sub components if they exist
			// as one entry in the struct
			if (line[i] == '(') {
				lastInLine = 0; //Returned as -1 when no character found in string
				leftDelimeter = i + 1 + line.substr(i + 1).find('(');
				rightDelimeter = i + line.substr(i).find(')');
				lastInLine = rightDelimeter;
				while ((leftDelimeter < rightDelimeter) && (lastInLine >= 0)) {
					lastInLine = line.substr(leftDelimeter + 1).find('(');
					leftDelimeter = leftDelimeter + 1 + line.substr(leftDelimeter + 1).find('(');
					rightDelimeter = rightDelimeter + 1 + line.substr(rightDelimeter + 1).find(')');
				}
				parameters.push_back(line.substr(i, rightDelimeter - (i - 1)));
				i += (rightDelimeter - i);
			} else if (line[i] == '[') {
				lastInLine = 0;
				leftDelimeter = i + 1 + line.substr(i + 1).find('[');
				rightDelimeter = i + line.substr(i).find(']');
				lastInLine = rightDelimeter;
				while ((leftDelimeter < rightDelimeter) && (lastInLine >= 0)) {
					lastInLine = line.substr(leftDelimeter + 1).find('[');
					leftDelimeter = leftDelimeter + 1 + line.substr(leftDelimeter + 1).find('[');
					rightDelimeter = rightDelimeter + 1 + line.substr(rightDelimeter + 1).find(']');
				}
				parameters.push_back(line.substr(i, rightDelimeter - (i - 1)));
				i += (rightDelimeter - i);
			} else if (line[i] == '{') {
				lastInLine = 0;
				leftDelimeter = i + 1 + line.substr(i + 1).find('{');
				rightDelimeter = i + line.substr(i).find('}');
				lastInLine = rightDelimeter;
				while ((leftDelimeter < rightDelimeter) && (lastInLine >= 0)) {
					lastInLine = line.substr(leftDelimeter + 1).find('{');
					leftDelimeter = leftDelimeter + 1 + line.substr(leftDelimeter + 1).find('{');
					rightDelimeter = rightDelimeter + 1 + line.substr(rightDelimeter + 1).find('}');
				}
				parameters.push_back(line.substr(i, rightDelimeter - (i - 1)));
				i += (rightDelimeter - i);
			} else if (line[i] == '<') {
				lastInLine = 0;
				leftDelimeter = i + 1 + line.substr(i + 1).find('<');
				rightDelimeter = i + line.substr(i).find('>');
				lastInLine = rightDelimeter;
				while ((leftDelimeter < rightDelimeter) && (lastInLine >= 0)) {
					lastInLine = line.substr(leftDelimeter + 1).find('<');
					leftDelimeter = leftDelimeter + 1 + line.substr(leftDelimeter + 1).find('<');
					rightDelimeter = rightDelimeter + 1 + line.substr(rightDelimeter + 1).find('>');
				}
				parameters.push_back(line.substr(i, rightDelimeter - (i - 1)));
				i += (rightDelimeter - i);
				// End of functions that search for encapulated components
			} else {	
				// The else condition catches all other components of the function and stores them
				// in the order found as parameters
				leftDelimeter = i;
				if (line[i] == '=') {
					//Ignore
				} else if (line[i] == '*') {
					// Should only occur after finding a struct or vector pointer, append onto
					// previous parameter
					parameters[parameters.size()-1]+='*';
				} else if (line[i] == ',') {
					//Ignore
				} else if (line.substr(i + 1).find(" ") > line.substr(i + 1).find(",")) {
					rightDelimeter = 1 + line.substr(i + 1).find(",");
					parameters.push_back(line.substr(leftDelimeter, rightDelimeter));
					i += rightDelimeter;
				} else if (line.substr(i + 1).find(" ") < line.substr(i + 1).find(",")) {
					rightDelimeter = 1 + line.substr(i + 1).find(" ");
					parameters.push_back(line.substr(leftDelimeter, rightDelimeter));
					i += rightDelimeter;
				} else if (line.substr(i + 1).find(" ") == -1) {
					parameters.push_back(line.substr(leftDelimeter));
					i = (line.length() - 1);
					//This will always be the last object on the line
				}
			}
		}
	}

	instruction.general.fields = parameters.size();
	last = (parameters.size() - 1);
	debugParams(parameters);
	// Once all components have been found, navigate through and define each component
	// and initialize the attributes struct values to match the line
	//Instruction Type
	// Terminator
	// Binary Operations
	// Integer
	// Floating Point
	// Bitwise Binary Operation
	// Vector Operations
	// Aggregate Operations
	// Memory Access and Addressing Oprations
	// Converstion Operation
	// Other Operations
	// Custom Operations

	switch (s_opMap[instruction.general.opCode]) {
	// Terminator Instructions
	case IR_Ret: {
		// ret <type> <value>; Return a value from a non - void function
		// ret void; Return from void function
		// Set general instruction parameters
		instruction.general.terminator = true;
		instruction.general.flowControl = true;

		if (parameters[last].find("void")) {
			// If void is found then it must not have a return value
			instruction.terminator.type = "void";
		} else {
			// If void is not found then the last parameters must be the return value,
			// and preceding it the return type
			instruction.terminator.type = parameters[last - 1];
			if(isRegister(parameters[last])) {
				setRegister(parameters[last], instruction.terminator.value, instruction, list, parameters);
				instruction.terminator.ivalue = "void";
				instruction.terminator.intermediate = false;
				instruction.terminator.value->setSize(instruction.terminator.type);
			} else {
			instruction.terminator.intermediate = true;
			instruction.terminator.ivalue = parameters[last];
			}
		}
		break;
		
	}
	case IR_Br: {
		// br i1 <cond>, label <iftrue>, label <iffalse>
		// br label <dest>          ; Unconditional branch
		instruction.general.terminator = true;
		instruction.general.flowControl = true;
		if (parameters.size() == 3) {
			//Unconditional branch
			instruction.terminator.unconditional = true;
			instruction.terminator.type = "void";
			instruction.terminator.dest = parameters[2].substr(1);
		} else {
			// Conditional Branch
			instruction.terminator.type = parameters[1];
			instruction.terminator.iftrue = parameters[4].substr(1);
			instruction.terminator.iffalse = parameters[6].substr(1);
			if(isRegister(parameters[2])) setRegister(parameters[2], instruction.terminator.cond, instruction, list, parameters);
			instruction.terminator.cond->setSize(instruction.terminator.type);
		}
		break;
	}
	case IR_Switch: {
		// switch <intty> <value>, label <defaultdest> [ <intty> <val>, label <dest> ... ]
		instruction.general.terminator = true;
		instruction.general.flowControl = true;
		instruction.terminator.intty = parameters[1];
		if(isRegister(parameters[2])) setRegister(parameters[2], instruction.terminator.value, instruction, list, parameters);
		instruction.terminator.defaultdest = parameters[4].substr(1); 
		int location = 0;
		int length = 0;
		int statements = 0;
		int i = 0;
		std::string cases[MAXCASES][2];
		for(int k = 0; k < parameters[5].size(); k++) {
			if(parameters[5][k] == '%') statements++;
		}
		instruction.terminator.cases.statements = statements;
		while(i < statements) {
			location = parameters[5].find_first_of('i', location);
			location = parameters[5].find_first_of(' ', location) + 1;
			length = parameters[5].find_first_of(',', location) - location;
			instruction.terminator.cases.value[i] = stoi(parameters[5].substr(location, length));
			location = parameters[5].find_first_of('%', location)+1;
			length = parameters[5].find_first_of(' ', location) - location;
			instruction.terminator.cases.dest[i] = parameters[5].substr(location, length);
			DPRINTF(ComputeNode, "Value %d, Dest: %s\n", instruction.terminator.cases.value[i], instruction.terminator.cases.dest[i]);
			i++;
		}
		break;
	}
	case IR_IndirectBr: {
		// indirectbr <somety>* <address>, [ label <dest1>, label <dest2>, ... ]
		
		break;
	}
	case IR_Invoke: {
		// <result> = invoke [cconv] [ret attrs] <ptr to function ty> <function ptr val>(<function args>) [fn attrs]
		// to label <normal label> unwind label <exception label>
	}
	case IR_Resume: {
		// resume <type> <value>
		instruction.general.terminator = true;
		instruction.general.flowControl = true;
		
		instruction.terminator.type = parameters[0];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.terminator.value, instruction, list, parameters);
		break;
	}
	case IR_Unreachable: {
		// The unreachable instruction has no defined semantics.
		break;
	}
	// Binary Operations
	case IR_Add: {
		// <result> = add <ty> <op1>, <op2>          ; yields {ty}:result
		// <result> = add nuw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = add nsw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = add nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.binary = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_FAdd: {
		// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.binary = true;
		instruction.cycle.max = 5;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_Sub: {
		// <result> = sub <ty> <op1>, <op2>          ; yields {ty}:result
		// <result> = sub nuw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = sub nsw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = sub nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.binary = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_FSub: {
		// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.binary = true;
		instruction.cycle.max = 5;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_Mul: {
		// <result> = mul <ty> <op1>, <op2>          ; yields {ty}:result
		// <result> = mul nuw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = mul nsw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = mul nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.binary = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_FMul: {
		// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.binary = true;
		instruction.cycle.max = 5;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_UDiv: {
		// <result> = udiv <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = udiv exact <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.binary = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_SDiv: {
		// <result> = sdiv <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = sdiv exact <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.binary = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_FDiv: {
		// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.binary = true;
		instruction.cycle.max = 5;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_URem: {
		// <result> = urem <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.binary = true;
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_SRem: {
		// <result> = srem <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.binary = true;
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_FRem: {
		// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.binary = true;
		instruction.cycle.max = 5;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	// Bitwise Operations
	case IR_Shl: {
		// <result> = shl <ty> <op1>, <op2>           ; yields {ty}:result
		// <result> = shl nuw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = shl nsw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = shl nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.bitwise = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_LShr: {
		// <result> = lshr <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = lshr exact <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.bitwise = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_AShr: {
		// <result> = ashr <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = ashr exact <ty> <op1>, <op2>; yields{ ty }:result
		instruction.general.bitwise = true;
		setFlags(parameters, instruction);
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_And: {
		// <result> = and <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.bitwise = true;
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_Or: {
		// <result> = or <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.bitwise = true;
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	case IR_Xor: {
		// <result> = xor <ty> <op1>, <op2>   ; yields {ty}:result
		instruction.general.bitwise = true;
		initializeReturnRegister(parameters, instruction);
		setOperands(list, parameters, instruction);
		break;
	}
	// Memory Operations
	case IR_Alloca: {
		// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result
		break;
	}
	case IR_Load: {
		// <result> = load [volatile] <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>]
		// <result> = load atomic[volatile] <ty>* <pointer>[singlethread] <ordering>, align <alignment>
		//	!<index> = !{ i32 1 }
		// Set memory operation flag to true
		instruction.general.memory = true;
		// Index variable tracks keywords starting from the beginning of instruction
		int index = 0;
		// Check if instruction has volatilte keyword
		if (parameters[0] == "volatile") {
			// Increment index
			index++;
			// Set volatile flag 
			instruction.memory.load.volatileVar = true;
		}
		// Set return type
		instruction.memory.load.ty = parameters[index];
		// Set return register type and size
		instruction.general.returnRegister->setSize(instruction.memory.load.ty);
		int align = 0;
		while (parameters[align].compare("align") != 0)
		    align++;
		// Determine if register that contains load address exists
		if(isRegister(parameters[align - 1])) setRegister(parameters[align - 1], instruction.memory.load.pointer, instruction, list, parameters);
		// Set value for alignment
		DPRINTF(ComputeNode, "Align: %s\n", parameters[align+1]);
	        instruction.memory.load.align = stoi(parameters[align + 1]);
		break;
	}
	case IR_Store: {
		// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void}
		// store atomic[volatile] <ty> <value>, <ty>* <pointer>[singlethread] <ordering>, align <alignment>; yields{ void }
		instruction.general.memory = true;
		int index = 1;
		if (parameters[1] == "volatile") {
			index = 2;
			instruction.memory.store.volatileVar = true;
		}
		instruction.memory.store.ty = parameters[index];
		if(isRegister(parameters[index + 3])) setRegister(parameters[index + 3], instruction.memory.store.pointer, instruction, list, parameters);
		else DPRINTF(ComputeNode, "Pointer is an immediate value, not implemented\n");
		
		if(isRegister(parameters[index + 1])) setRegister(parameters[index + 1], instruction.memory.store.value, instruction, list, parameters);
		else {
			instruction.memory.store.immediate = true;
			if (instruction.memory.store.ty[0] == 'i') {
				instruction.memory.store.ival = stoi(parameters[2]);
			}
			else DPRINTF(ComputeNode, "Immediate value is of type other than integer, not implemented");
		}
		instruction.memory.store.align = std::stoi(parameters[index + 5]);
		break;
	}
	case IR_GetElementPtr: {
	// <result> = getelementptr Creating select<ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
	// <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
		int index = 0;
		int j = 0;
		std::string customDataType;
		instruction.general.memory = true;
		instruction.general.returnRegister->setSize("pointer");

		if (parameters[0] == "inbounds") {
			instruction.memory.getptr.inbounds = true;
			instruction.memory.getptr.pty = parameters[1];
			if (list->findRegister(parameters[3].substr(1)) == NULL) {
				instruction.memory.getptr.ptrval = new Register(parameters[3]);
				list->addRegister(instruction.memory.getptr.ptrval);
			} else instruction.memory.getptr.ptrval = list->findRegister(parameters[3].substr(1));
			if(parameters[1].back() != '*')
			    index = 3;
		    else
		        index = 2;
		} else {
			instruction.memory.getptr.pty = parameters[0];
			if (list->findRegister(parameters[2].substr(1)) == NULL) {
				instruction.memory.getptr.ptrval = new Register(parameters[3]);
				list->addRegister(instruction.memory.getptr.ptrval);
			} else instruction.memory.getptr.ptrval = list->findRegister(parameters[2].substr(1));
			
			if (parameters[0].back() != '*')
			    index = 2;
		    else
		        index = 1;
		}
		if(instruction.memory.getptr.pty[0] == '[') { // Return type is a struct
				int stringLength = (instruction.memory.getptr.pty.find_first_of(']') - instruction.memory.getptr.pty.find('%')-1);
				customDataType = instruction.memory.getptr.pty.substr(instruction.memory.getptr.pty.find('%')+1, stringLength);
				instruction.memory.getptr.llvmType = typeList->findType(customDataType);
			if(instruction.memory.getptr.llvmType != NULL) {	
				DPRINTF(LLVMGEP, "Custom Data Type = %s\n", customDataType);
			} else {
				customDataType = "none";
				DPRINTF(LLVMGEP, "No Custom Data Types Found!\n");
			}
		}
		if(instruction.memory.getptr.pty[0] == '%') { // Return type is a custom data type
				int stringLength = (instruction.memory.getptr.pty.find_first_of(' ') - instruction.memory.getptr.pty.find('%')-1);
				customDataType = instruction.memory.getptr.pty.substr(instruction.memory.getptr.pty.find('%')+1, stringLength);
				instruction.memory.getptr.llvmType = typeList->findType(customDataType);
			if(instruction.memory.getptr.llvmType != NULL) {	
				DPRINTF(LLVMGEP, "Custom Data Type = %s\n", customDataType);
			} else {
				customDataType = "none";
				DPRINTF(LLVMGEP, "No Custom Data Types Found!\n");
			}
		}
		for (int i = 1; i + index <= last; i+=2) {
			instruction.memory.getptr.ty[j] = parameters[index+i];
			//if(parameters[index+i+1][0] == '%') {
			if(isRegister(parameters[index+i+1])) {
				setRegister(parameters[index+i+1], instruction.memory.getptr.idx[j], instruction, list, parameters);
				instruction.memory.getptr.immediate[j] = false;
				instruction.memory.getptr.immdx[j] = 0;
				DPRINTF(LLVMGEP, "idx%d = %s\n", j, instruction.memory.getptr.idx[j]);
			}
			else {
				instruction.memory.getptr.immediate[j] = true;
				instruction.memory.getptr.immdx[j] = stoi(parameters[index+i+1]);
				DPRINTF(LLVMGEP, "idx%d = %d\n", j, instruction.memory.getptr.immdx[j]);
			}
			j++;
			instruction.memory.getptr.index = j;
		}
		break;
	}
	case IR_Fence: {
		// fence [singlethread] <ordering>                   ; yields {void}
		break;
	}
	case IR_AtomicCmpXchg: {
		// cmpxchg [volatile] <ty>* <pointer>, <ty> <cmp>, <ty> <new> [singlethread] <ordering>  ; yields {ty}
		break;
	}
	case IR_AtomicRMW: {
		// atomicrmw [volatile] <operation> <ty>* <pointer>, <ty> <value> [singlethread] <ordering>                   ; yields {ty}
		break;
	}
	// Conversion Operations
	case IR_Trunc: {
		// <result> = trunc <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_ZExt: {
		// <result> = zext <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_SExt: {
		// <result> = sext <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_FPToUI: {
		// <result> = fptoui <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_FPToSI: {
		// <result> = fptosi <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_UIToFP: {
		// <result> = uitofp <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_SIToFP: {
		// <result> = sitofp <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_FPTrunc: {
		// <result> = fptrunc <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_FPExt: {
		// <result> = fpext <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_PtrToInt: {
		// <result> = ptrtoint <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_IntToPtr: {
		// <result> = inttoptr <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_BitCast: {
		// <result> = bitcast <ty> <value> to <ty2>             ; yields ty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	case IR_AddrSpaceCast: {
		// <result> = addrspacecast <pty> <ptrval> to <pty2>       ; yields pty2
		instruction.general.conversion = true;
		instruction.conversion.ty = parameters[0];
		instruction.conversion.ty2 = parameters[3];
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.conversion.value, instruction, list, parameters);
		else {
			instruction.conversion.immVal = stoi(parameters[1]);
			instruction.conversion.immediate = true;
		}
		break;
	}
	// Other Operations - Compare
	case IR_ICmp: {
		// <result> = icmp <cond> <ty> <op1>, <op2>   ; yields {i1} or {<N x i1>}:result
		instruction.general.other = true;
		instruction.general.compare = true;
		instruction.other.compare.condition.cond = parameters[0];

		instruction.other.compare.ty = parameters[1];
		instruction.general.returnRegister->setSize(instruction.other.compare.ty);

		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) setRegister(parameters[last], instruction.other.compare.op2, instruction, list, parameters);	
		else {
			instruction.other.compare.immediate2 = true;
			instruction.other.compare.iop2 = parameters[last];
		}

		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) setRegister(parameters[last-1], instruction.other.compare.op1, instruction, list, parameters);
		else {
			instruction.other.compare.immediate1 = true;
			instruction.other.compare.iop1 = parameters[last - 1];
		}

		if (instruction.other.compare.condition.cond == "eq")
			instruction.other.compare.condition.eq = true;
		else if (instruction.other.compare.condition.cond == "ne")
			instruction.other.compare.condition.ne = true;
		else if (instruction.other.compare.condition.cond == "ugt")
			instruction.other.compare.condition.ugt = true;
		else if (instruction.other.compare.condition.cond == "uge")
			instruction.other.compare.condition.uge = true;
		else if (instruction.other.compare.condition.cond == "ult")
			instruction.other.compare.condition.ult = true;
		else if (instruction.other.compare.condition.cond == "ule")
			instruction.other.compare.condition.ule = true;
		else if (instruction.other.compare.condition.cond == "sgt")
			instruction.other.compare.condition.sgt = true;
		else if (instruction.other.compare.condition.cond == "sge")
			instruction.other.compare.condition.sge = true;
		else if (instruction.other.compare.condition.cond == "slt")
			instruction.other.compare.condition.slt = true;
		else if (instruction.other.compare.condition.cond == "sle")
			instruction.other.compare.condition.sle = true;
		break;
	}
	case IR_FCmp: {
		// <result> = fcmp <cond> <ty> <op1>, <op2>     ; yields {i1} or {<N x i1>}:result
		instruction.general.other = true;
		instruction.general.compare = true;
		instruction.other.compare.condition.cond = parameters[0];

		instruction.other.compare.ty = parameters[1];
		instruction.general.returnRegister->setSize(instruction.other.compare.ty);

		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) setRegister(parameters[last], instruction.other.compare.op2, instruction, list, parameters);
		else {
			instruction.other.compare.immediate2 = true;
			instruction.other.compare.iop2 = parameters[last];
		}

		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) setRegister(parameters[last-1], instruction.other.compare.op1, instruction, list, parameters);
		else {
			instruction.other.compare.immediate1 = true;
			instruction.other.compare.iop1 = parameters[last - 1];
		}
		
		if (instruction.other.compare.condition.cond == "false")
			instruction.other.compare.condition.condFalse = true;
		else if (instruction.other.compare.condition.cond == "oeq")
			instruction.other.compare.condition.oeq = true;
		else if (instruction.other.compare.condition.cond == "ogt")
			instruction.other.compare.condition.ogt = true;
		else if (instruction.other.compare.condition.cond == "oge")
			instruction.other.compare.condition.oge = true;
		else if (instruction.other.compare.condition.cond == "olt")
			instruction.other.compare.condition.olt = true;
		else if (instruction.other.compare.condition.cond == "ole")
			instruction.other.compare.condition.ole = true;
		else if (instruction.other.compare.condition.cond == "one")
			instruction.other.compare.condition.one = true;
		else if (instruction.other.compare.condition.cond == "ord")
			instruction.other.compare.condition.ord = true;
		else if (instruction.other.compare.condition.cond == "ueq")
			instruction.other.compare.condition.ueq = true;
		else if (instruction.other.compare.condition.cond == "ugt")
			instruction.other.compare.condition.ugt = true;
		else if (instruction.other.compare.condition.cond == "uge")
			instruction.other.compare.condition.uge = true;
		else if (instruction.other.compare.condition.cond == "ult")
			instruction.other.compare.condition.ult = true;
		else if (instruction.other.compare.condition.cond == "ule")
			instruction.other.compare.condition.ule = true;
		else if (instruction.other.compare.condition.cond == "une")
			instruction.other.compare.condition.une = true;
		else if (instruction.other.compare.condition.cond == "uno")
			instruction.other.compare.condition.uno = true;
		else if (instruction.other.compare.condition.cond == "true")
			instruction.other.compare.condition.condTrue = true;

		break;
	}
	case IR_PHI: {
		// <result> = phi <ty> [ <val0>, <label0>], ...
		instruction.general.other = true;
		instruction.general.phi = true;
		instruction.general.flowControl = true;
		instruction.other.phi.ty = parameters[0];
		instruction.general.returnRegister->setSize(instruction.other.phi.ty);
		std::string *val = new std::string[last];
		std::string *label = new std::string[last];
		int labelLength = 0;
		for (int i = 1; i <= last; i++) {
			instruction.other.phi.ival[i - 1] = parameters[i];
			labelLength = instruction.other.phi.ival[i - 1].find(',');
			labelLength = labelLength - instruction.other.phi.ival[i - 1].find('[');
			val[i - 1] = instruction.other.phi.ival[i - 1].substr(2, (instruction.other.phi.ival[i - 1].find(',')) - 2);
			labelLength = instruction.other.phi.ival[i - 1].find(',');
			labelLength = instruction.other.phi.ival[i - 1].find(']') - labelLength;
			label[i - 1] = instruction.other.phi.ival[i - 1].substr((instruction.other.phi.ival[i - 1].find(',')) + 3, labelLength - 4);
			if(isRegister(val[i-1])) {
				setRegister(val[i-1], instruction.other.phi.val[i - 1], instruction, list, parameters);
				instruction.other.phi.ival[i - 1].clear();
				instruction.other.phi.immVal[i - 1] = false;
				instruction.other.phi.label[i - 1] = label[i - 1];
				DPRINTF(ComputeNode, "Loading value stored in %s if called from BB %s. \n", val[i - 1], label[i - 1]);
			} else {
				if (val[i-1] == "true") {
					instruction.other.phi.ival[i - 1] = '1';
				} else if (val[i-1] == "false") {
					instruction.other.phi.ival[i - 1] = '0';
				} else {
					instruction.other.phi.ival[i - 1] = val[i - 1];
				}
				instruction.other.phi.immVal[i - 1] = true;
				instruction.other.phi.label[i - 1] = label[i - 1];
				DPRINTF(ComputeNode, "Loading immediate value %s if called from BB %s. \n", val[i - 1], label[i - 1]);
			}
		}
		break;
	}
	case IR_Call: {
		// <result> = [tail] call[cconv][ret attrs] <ty>[<fnty>*] <fnptrval>(<function args>)[fn attrs]
		break;
	}
	case IR_Select: {
		// <result> = select selty <cond>, <ty> <val1>, <ty> <val2>             ; yields ty
		// selty is either i1 or {<N x i1>}
		instruction.general.other = true;
		instruction.other.select.ty = parameters[2];
		instruction.general.returnRegister->setSize(instruction.other.select.ty);

		instruction.other.select.immediate[0] = false;
		instruction.other.select.immediate[1] = false;
		
		if(parameters[2][0] == 'i') {
			instruction.other.select.intTy = true;
			DPRINTF(ComputeNode, "Integer Type\n");
		} else if(parameters[2] == "float") {
			instruction.other.select.floatTy = true;
		} else if(parameters[2] == "double") {
			instruction.other.select.doubleTy = true;
		} else { }
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.other.select.cond, instruction, list, parameters);
		else {
			instruction.other.select.icondFlag = true;
			if(parameters[1] == "true") instruction.other.select.icond = true;
		}
		if(isRegister(parameters[3])) setRegister(parameters[3], instruction.other.select.val1, instruction, list, parameters);
		else {
			///////////////////////////////////
			instruction.other.select.immediate[0] = true;
			instruction.other.select.immVal[0] = stoi(parameters[3]);
			///////////////////////////////////
			if(parameters[2][0] == 'i') {
			} else if(parameters[2] == "float") {
			} else if(parameters[2] == "double") {
			} else { }
		}
		if(isRegister(parameters[5])) setRegister(parameters[5], instruction.other.select.val2, instruction, list, parameters);
		else {
			////////////////////////////////
			instruction.other.select.immediate[1] = true;
			instruction.other.select.immVal[1] = stoi(parameters[5]);
			////////////////////////////
			if(parameters[4][0] == 'i') {
			} else if(parameters[2] == "float") {
			} else if(parameters[2] == "double") {
			} else { }
		}
		break;
	}
	case IR_VAArg: {
		// <resultval> = va_arg <va_list*> <arglist>, <argty>
		break;
	}
	case IR_ExtractElement: {
		// <result> = extractelement <n x <ty>> <val>, i32 <idx>    ; yields <ty>
		break;
	}
	case IR_InsertElement: {
		// <result> = insertelement <n x <ty>> <val>, <ty> <elt>, i32 <idx>    ; yields <n x <ty>>
		break;
	}
	case IR_ShuffleVector: {
		// <result> = shufflevector <n x <ty>> <v1>, <n x <ty>> <v2>, <m x i32> <mask>    ; yields <m x <ty>>
		break;
	}
	case IR_ExtractValue: {
		// <result> = extractvalue <aggregate type> <val>, <idx>{, <idx>}*
		break;
	}
	case IR_InsertValue: {
		// <result> = insertvalue <aggregate type> <val>, <ty> <elt>, <idx>{ , <idx> }*; yields <aggregate type>
		break;
	}
	case IR_LandingPad: {
		// <resultval> = landingpad <resultty> personality <type> <pers_fn> <clause>+
		// <resultval> = landingpad <resultty> personality <type> <pers_fn> cleanup <clause>*
		// <clause> : = catch <type> <value>
		// <clause> : = filter <array constant type> <array constant>
		break;
	}
	case IR_DMAFence: { break; }
	case IR_DMAStore: { break; }
	case IR_DMALoad: { break; }
	case IR_IndexAdd: { break; }
	case IR_SilentStore: { break; }
	case IR_Sine: { break; }
	case IR_Cosine: { break; }
	case IR_Move: { break; }
	default: { break; }
	}
	dependencyList(parameters, dependencies);
}

void
ComputeNode::dependencyList(std::vector<std::string> &parameters, int dependencies) {
	DPRINTF(ComputeNode, "\n");
	DPRINTF(ComputeNode, "Dependencies List: \n");
	if(dependencies == 0) {
		DPRINTF(ComputeNode, "No Dependencies!\n\n");
	} else {
		for (int i = 0; i < dependencies; i++) {
			DPRINTF(ComputeNode, "#%d = Register (%s)\n", i + 1, instruction.dependencies.registers[i]->getName());
		}
		DPRINTF(ComputeNode, "\n");
	}
}

void 
ComputeNode::compute() {
	switch (s_opMap[instruction.general.opCode]) {
	case IR_Move: { break; }
	case IR_Ret:{ Operations::llvm_ret(instruction); break; }
	case IR_Br: { Operations::llvm_br(instruction); break; }
	case IR_Switch: { Operations::llvm_switch(instruction); break; }
	case IR_IndirectBr: { Operations::llvm_indirectbr(instruction); break; }
	case IR_Invoke: { Operations::llvm_invoke(instruction); break; }
	case IR_Resume: { Operations::llvm_resume(instruction); break; }
	case IR_Unreachable: { Operations::llvm_unreachable(instruction); break; }
	case IR_Add: { Operations::llvm_add(instruction); break; }
	case IR_FAdd: { Operations::llvm_fadd(instruction); break; }
	case IR_Sub: { Operations::llvm_sub(instruction); break; }
	case IR_FSub: { Operations::llvm_fsub(instruction); break; }
	case IR_Mul: { Operations::llvm_mul(instruction); break; }
	case IR_FMul: { Operations::llvm_fmul(instruction); break; }
	case IR_UDiv: { Operations::llvm_udiv(instruction); break; }
	case IR_SDiv: { Operations::llvm_sdiv(instruction); break; }
	case IR_FDiv: { Operations::llvm_fdiv(instruction); break; }
	case IR_URem: { Operations::llvm_urem(instruction); break; }
	case IR_SRem: { Operations::llvm_srem(instruction); break; }
	case IR_FRem: { Operations::llvm_frem(instruction); break; }
	case IR_Shl: { Operations::llvm_shl(instruction); break; }
	case IR_LShr: { Operations::llvm_lshr(instruction); break; }
	case IR_AShr: { Operations::llvm_ashr(instruction); break; }
	case IR_And: { Operations::llvm_and(instruction); break; }
	case IR_Or: { Operations::llvm_or(instruction); break; }
	case IR_Xor: { Operations::llvm_xor(instruction); break; }
	case IR_Alloca: { Operations::llvm_alloca(instruction); break; }
	case IR_Load: {
        uint64_t src = instruction.memory.load.pointer->value;
		req = new MemoryRequest((Addr)src, instruction.general.returnRegister->size);
		comm->enqueueRead(req);
	    break;
    }
	case IR_Store: {
		uint64_t data;
	    uint64_t dst = instruction.memory.store.pointer->getValue();
		if(instruction.memory.store.immediate) {
			DPRINTF(ComputeNode, "Immediate value store. \n");
			data = (uint64_t) instruction.memory.store.ival;
			req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), 1);
		}
        else {
			data = instruction.memory.store.value->getValue();
        	req = new MemoryRequest((Addr)dst, (uint8_t *)(&data), instruction.memory.store.value->size);
		}
		comm->enqueueWrite(req);
		break;
	}
	case IR_GetElementPtr: { Operations::llvm_getelementptr(instruction); break; }
	case IR_Fence: { Operations::llvm_fence(instruction); break; }
	case IR_AtomicCmpXchg: { Operations::llvm_cmpxchg(instruction); break; }
	case IR_AtomicRMW: { Operations::llvm_atomicrmw(instruction); break; }
	case IR_Trunc: { Operations::llvm_trunc(instruction); break; }
	case IR_ZExt: { Operations::llvm_zext(instruction); break; }
	case IR_SExt: { Operations::llvm_sext(instruction); break; }
	case IR_FPToUI: { Operations::llvm_fptoui(instruction); break; }
	case IR_FPToSI: { Operations::llvm_fptosi(instruction); break; }
	case IR_UIToFP: { Operations::llvm_uitofp(instruction); break; }
	case IR_SIToFP: { Operations::llvm_sitofp(instruction); break; }
	case IR_FPTrunc: { Operations::llvm_fptrunc(instruction); break; }
	case IR_FPExt: { Operations::llvm_fpext(instruction); break; }
	case IR_PtrToInt: { Operations::llvm_ptrtoint(instruction); break; }
	case IR_IntToPtr: { Operations::llvm_inttoptr(instruction); break; }
	case IR_BitCast: { Operations::llvm_bitcast(instruction); break; } 
	case IR_AddrSpaceCast: { Operations::llvm_addrspacecast(instruction); break; }
	case IR_ICmp: { Operations::llvm_icmp(instruction); break; }
	case IR_FCmp: { Operations::llvm_fcmp(instruction); break; }
	case IR_PHI: { Operations::llvm_phi(instruction, prevBB); break; }
	case IR_Call: { Operations::llvm_call(instruction); break; }
	case IR_Select: { Operations::llvm_select(instruction); break; }
	case IR_VAArg: { Operations::llvm_vaarg(instruction); break; }
	case IR_ExtractElement: { Operations::llvm_extractelement(instruction); break; }
	case IR_InsertElement: 	{ Operations::llvm_insertelement(instruction); break; }
	case IR_ShuffleVector: { Operations::llvm_shufflevector(instruction); break; }
	case IR_ExtractValue: { Operations::llvm_extractvalue(instruction); break; }
	case IR_InsertValue: { Operations::llvm_insertvalue(instruction); break; }
	case IR_LandingPad: { Operations::llvm_landingpad(instruction); break; }
	case IR_DMAFence: { Operations::llvm_dmafence(instruction); break; }
	case IR_DMAStore: { Operations::llvm_dmastore(instruction); break; }
	case IR_DMALoad: { Operations::llvm_dmaload(instruction); break; }
	case IR_IndexAdd: { Operations::llvm_indexadd(instruction); break; }
	case IR_SilentStore: { Operations::llvm_silentstore(instruction); break; }
	case IR_Sine: { Operations::llvm_sine(instruction); break; }
	case IR_Cosine: { Operations::llvm_cosine(instruction); break; }
	default: { break; }
	}
}

bool 
ComputeNode::commit() {
	// If cycle count is = max cycle count, commit register value to memory
	DPRINTF(LLVMRegister, "Committing (%s) Compute Node:\n", instruction.general.opCode);
	if (instruction.general.returnRegister != NULL) {
		DPRINTF(LLVMRegister, "Attempting to Commit Register (%s)\n", instruction.general.returnRegister->getName());
		instruction.cycle.current++;
		DPRINTF(LLVMRegister, "Cycle: Current = (%d) || Max = (%d) || Remaining = (%d)\n", instruction.cycle.current, instruction.cycle.max, instruction.cycle.max - instruction.cycle.current);
		if (instruction.cycle.current >= instruction.cycle.max) {
			instruction.general.returnRegister->commit();
			DPRINTF(LLVMRegister, "Cycle Complete! Register (%s) = (%.16x)\n\n", instruction.general.returnRegister->getName(), instruction.general.returnRegister->getValue());
			return true;
		} else DPRINTF(LLVMRegister, "Cycle Incomplete!\n\n");
	}
	return false;
}

bool 
ComputeNode::checkDependency() {
	bool hot = false;
	bool phiBranchDependent = false;
	DPRINTF(LLVMRegister, "Checking Dependencies for (%s) Compute Node!\n", instruction.general.opCode);
	if(dependencies == 0) DPRINTF(LLVMRegister, "No Dependencies!\n");
	if(instruction.general.opCode == "phi"){
		DPRINTF(LLVMRegister,"Phi Instruction Detected: Previous BB = (%s)\n", prevBB);
		for (int i = 0; i < MAXPHI; i++) {
			if (prevBB == instruction.other.phi.label[i]) {
				if(!instruction.other.phi.immVal[i]) {
					phiBranchDependent = true;
					if(instruction.other.phi.val[i]->getStatus()){
						DPRINTF(LLVMRegister, "Register (%s) is Hot:\n", instruction.other.phi.val[i]->getName());
						hot = true;
					} else DPRINTF(LLVMRegister, "Register (%s) is Ready:\n", instruction.other.phi.val[i]->getName());
				} else {
					DPRINTF(LLVMRegister, "Immediate Value (%d) Loaded!\n", instruction.other.phi.val[i]);
				}
			}
		}
		if(!phiBranchDependent) DPRINTF(LLVMRegister, "No Dependencies!\n");
	} else {
		for (int i = 0; i < dependencies; i++) {
			DPRINTF(LLVMRegister, "Checking Dependency #%d:\n", i+1);
			if (instruction.dependencies.registers[i]->getStatus()) {
				DPRINTF(LLVMRegister, "Register (%s) is Hot:\n", instruction.dependencies.registers[i]->getName());
				hot = true;
			} else {
				DPRINTF(LLVMRegister, "Register (%s) is Ready:\n", instruction.dependencies.registers[i]->getName());
			}
		}
	}
	DPRINTF(LLVMRegister, "Checking Dependencies: Finished!\n\n");

	return hot;
}

void 
ComputeNode::reset() {
	if (instruction.general.returnRegister != NULL) {
		instruction.general.returnRegister->reset();
		instruction.cycle.current = 0;
	}
}

void
ComputeNode::debugParams(std::vector<std::string> &parameters) { 
	DPRINTF(ComputeNode, "Creating (%s) Compute Node:\n", instruction.general.opCode);
	if(DEBUGPARAMS) for (int i = 0; i < parameters.size(); i++) DPRINTF(ComputeNode, "Parameter[%d]: (%s)\n", i, parameters[i]);
	DPRINTF(ComputeNode, "\n");
}

void
ComputeNode::setFlags(std::vector<std::string> &parameters, Instruction &instruction) {
	for(int i = 0; i < parameters.size(); i++){
		if (parameters[i] == "nuw") instruction.flags.nuw = true;
		else if (parameters[i] == "nsw") instruction.flags.nsw = true;
		else if (parameters[i] == "nnan") instruction.flags.nnan = true;
		else if (parameters[i] == "ninf") instruction.flags.ninf = true;
		else if (parameters[i] == "nsz") instruction.flags.nsz = true;
		else if (parameters[i] == "arcp") instruction.flags.arcp = true;
		else if (parameters[i] == "contract") instruction.flags.contract = true;
		else if (parameters[i] == "afn") instruction.flags.afn = true;
		else if (parameters[i] == "reassoc") instruction.flags.reassoc = true;
		else if (parameters[i] == "fast") instruction.flags.fast = true;
		else if (parameters[i] == "exact") instruction.flags.exact = true;
	}
}

bool 
ComputeNode::isRegister(std::string data){
	if(data[0] == '%') return true;
	return false;
}

void
ComputeNode::setRegister(std::string data, Register *&reg, Instruction &instruction, RegisterList *list, std::vector<std::string> &parameters) {
	std::string name = data.substr(1);
	if (list->findRegister(name) == NULL) {
		reg = new Register(name);
		list->addRegister(reg);
		instruction.dependencies.registers[dependencies] = reg;
		dependencies++;
		} else {
		reg = list->findRegister(name);
		instruction.dependencies.registers[dependencies] = reg;
		dependencies++;
	}
}

void
ComputeNode::initializeReturnRegister(std::vector<std::string> &parameters, Instruction &instruction){
	int last = parameters.size() - 1;
	if(instruction.general.binary){
		// Set instruction return type <ty>
		instruction.binary.ty = parameters[last - 2];
		// Set size of return register to match instruction return type
		instruction.general.returnRegister->setSize(instruction.binary.ty);	
	}
	else if(instruction.general.bitwise){
		// Set instruction return type <ty>
		instruction.bitwise.ty = parameters[last - 2];
		// Set size of return register to match instruction return type
		instruction.general.returnRegister->setSize(instruction.bitwise.ty);
	}
}

void
ComputeNode::setOperands(RegisterList *list, std::vector<std::string> &parameters, Instruction &instruction) {
	int last = parameters.size() - 1;
	if(instruction.general.binary){
		// Operand 2
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) setRegister(parameters[last], instruction.binary.op2, instruction, list, parameters);
		else {
			// Operation uses immediate value
			// Set immediate flag true
			instruction.binary.immediate2 = true;
			// Load string representation of immediate value
			instruction.binary.iop2 = parameters[last];
		}
		// Operand 1
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) setRegister(parameters[last-1], instruction.binary.op1, instruction, list, parameters);
		else {
			// Operation uses immediate value
			// Set immediate flag true
			instruction.binary.immediate1 = true;
			// Load string representation of immediate value
			instruction.binary.iop1 = parameters[last - 1];
		}
	} else if(instruction.general.bitwise) {
		// Operand 2
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) setRegister(parameters[last], instruction.bitwise.op2, instruction, list, parameters);
		else {
			// Operation uses immediate value
			// Set immediate flag true
			instruction.bitwise.immediate2 = true;
			// Load string representation of immediate value
			instruction.bitwise.iop2 = parameters[last];
		}
		// Operand 1
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) setRegister(parameters[last-1], instruction.bitwise.op1, instruction, list, parameters);
		else {
			// Operation uses immediate value
			// Set immediate flag true
			instruction.bitwise.immediate1 = true;
			// Load string representation of immediate value
			instruction.bitwise.iop1 = parameters[last - 1];
		}
	}
}
