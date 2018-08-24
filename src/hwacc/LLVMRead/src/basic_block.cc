#include "basic_block.hh"
#include <memory>

// ////////////////////////////////////////////////////////////////////
// Compute Node Constructor 
// Inputs:	string line 		- line parsed from llvm file
//			RegisterList *list 	- pointer to linked list that contains all return registers created 
//			string prev 		- name of the previous basic block created 
//			CommInterface *co 	- pointer to communications interface to enque read and write requests
//			TypeList *typeList 	- linked list that contains all custom data types defines for function
//
// Outputs: none
// 
// This function is used to create a new compute node and initialize it based around the arguments 
// parsed from the passed LLVM instruction line.
// ////////////////////////////////////////////////////////////////////

void
BasicBlock::Parse(std::string line, RegisterList *list, std::string prev, CommInterface *co, TypeList *typeList) {
	// ////////////////////////////////////////////////////////////////////
	// Local Variables
	std::vector<std::string> parameters; // Used to store each each element of the passed in LLVM instruction line 
	std::vector<Register*> dependencies;
	int leftDelimeter, rightDelimeter, lastInLine; // Used for parsing elements from the instruction line
	int returnChk = line.find(" = "); // If a return register exists, this value is set greater than 0
	int last = 0; // Set as index to the last element in the parameters vector once initialized
	Register * ret_reg = NULL;
	std::string lineCpy = line;
	std::string opCode;
	std::string returnType;
	std::string instructionType;
	uint64_t maxCycles = 0;
	uint64_t computeFlags = 0;
	uint64_t attributeFlags = 0;

	// ////////////////////////////////////////////////////////////////////
	// Find the return register. If it exists, it is always the first component of the line
	if (returnChk > 0) {
		// Check if register already exists (It should not because SSA)
		std::string ret_name = line.substr((line.find("%") + 1), returnChk - 3); // Set return register name
		ret_reg = list->findRegister(ret_name); // Ensure the return register isnt already in the list 
		if(ret_reg == NULL) {
		    // Create new pointer to the return register
		    ret_reg = new Register(ret_name);
		    list->addRegister(ret_reg);
		    DPRINTF(LLVMRegister, "Creating Return Register: (%s)\n", ret_reg->getName());
		} else {
			DPRINTF(LLVMRegister, "Error: Trying to Create Duplicate Return Register!\n");
		}
		// In all instances where a return register is the first component, the next component is
		// the opcode, which is parsed and removed from line
		line = line.substr(returnChk + 3); // Skip over " = " 
		opCode = line.substr(0, line.find(' ')); // Store opcode in instruction struct
		line = line.substr(line.find(' ')); // Drop opcode and return register from instruction line
	} else {
		// If no return register is found then the first component must instead be the opcode
		// Search for first none empty space which is were the opcode must begin
		// Then store the opcode and remove the parsed information from the line
		for (int i = 0; i < line.length(); i++) {
			if (line[i] != ' ') {
				opCode = line.substr(i, line.find(' ', i) - 2); // Store opcode in instruction struct
				line = line.substr(line.find(' ') + 1); // Drop opcode from instruction line
				break;
			}
		}
		// No return register needed
		DPRINTF(LLVMRegister, "No Return Register Needed!\n");
	}
	// //////////////////////////////////////////////////////////////////////////////////
	DPRINTF(ComputeNode, "Opcode Found: (%s)\n", opCode);
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
				leftDelimeter = i + 1 +instruction.binary line.substr(i + 1).find('[');
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
					// Catches any additional elements in the instruction line seperated by commas
					rightDelimeter = 1 + line.substr(i + 1).find(",");
					parameters.push_back(line.substr(leftDelimeter, rightDelimeter));
					i += rightDelimeter;
				} else if (line.substr(i + 1).find(" ") < line.substr(i + 1).find(",")) {
					// Catches any additional elements in the instruction line seperated by whitespace
					rightDelimeter = 1 + line.substr(i + 1).find(" ");
					parameters.push_back(line.substr(leftDelimeter, rightDelimeter));
					i += rightDelimeter;
				} else if (line.substr(i + 1).find(" ") == -1) { // End of line
					parameters.push_back(line.substr(leftDelimeter));
					i = (line.length() - 1);
					// This will always be the last object on the line
				}MemoryRequest* _Req
			}
		}
	}
	last = (parameters.size() - 1); // Set last to index the final value in the parameters vector
	debugParams(parameters); // Prints all found parameters for the instruction line
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

	switch (s_opMap[opCode]) {
	// Terminator Instructions
	case IR_Ret: {
		// ret <type> <value>; Return a value from a non - void function
		// ret void; Return from void function
		// Set general instruction parameters
		maxCycles = CYCLECOUNTRET;
		instructionType = "Terminator";
		if (parameters[last].find("void")) {
			// If void is found then it must not have a return value
			returnType = "void";
		} else {
			// Implementation Removed for Returns of Non-Void type
		}
		auto ret = std::make_shared<Ret>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co);
		addNode(ret);
		break;	
	}
	case IR_Br: {
		// br i1 <cond>, label <iftrue>, label <iffalse>
		// br label <dest>          ; Unconditional branch
		instructionType = "Terminator";
		maxCycles = CYCLECOUNTBR;
		if (parameters.size() == 3) {
			//Unconditional branch
			returnType = "void";
			auto br = std::make_shared<Br>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											parameters[2].substr(1),
											true);
			addNode(br);
		} else {
			// Conditional Branch
			returnType = parameters[1];
			std::vector<std::string> branches;
			Register* condition;
			branches.push_back(parameters[4].substr(1)); // If True [0]
			branches.push_back(parameters[6].substr(1)); // If False [1]
			setRegister(parameters[2], condition, dependencies, list, parameters);
			condition->setSize(returnType);
			auto br = std::make_shared<Br>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											condition,
											branches,
											false);
			addNode(br);
		}
		break;
	}
	case IR_Switch: {
		// switch <intty> <value>, label <defaultdest> [ <intty> <val>, label <dest> ... ]
		instructionType = "Terminator";
		maxCycles = CYCLECOUNTSWITCH;
		returnType = parameters[1];
		std::vector<std::string> branches;
		std::vector<int>caseValues;
		Register* condition;
		branches.push_back(parameters[4].substr(1)); // Default Destination [0]
		caseValues.push_back(0); // Initialize first location of case values for Index Matching 
		// Derive case statements
		int location = 0;
		int length = 0;
		int statements = 0;
		int i = 1;
		std::string cases[MAXCASES][2];
		setRegister(parameters[2], condition, dependencies, list, parameters);
		// Determine the number of case statements 
		for(int k = 0; k < parameters[5].size(); k++) {
			if(parameters[5][k] == '%') statements++;
		}
		// Set the value and location of each case statement
		while(i <= statements) {
			location = parameters[5].find_first_of('i', location);
			location = parameters[5].find_first_of(' ', location) + 1;
			length = parameters[5].find_first_of(',', location) - location;
			caseValues.push_back(stoi(parameters[5].substr(location, length)));
			location = parameters[5].find_first_of('%', location)+1;
			length = parameters[5].find_first_of(' ', location) - location;
			branches.push_back(parameters[5].substr(location, length));
			DPRINTF(ComputeNode, "Value %d, Dest: %s\n", caseValues.at(i), branches.at(i));
			i++;
		}
		auto swtch = std::make_shared<Switch>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												condition,
												branches,
												caseValues);
		addNode(swtch);
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
		instructionType = "Terminator";
		maxCycles = CYCLECOUNTRESUME;
		returnType = parameters[0];
		Register* condition;
		setRegister(parameters[1], condition, dependencies, list, parameters);
		break;
		// Untested Implementation
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
		maxCycles = CYCLECOUNTADD;
		instructionType = "Binary";
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto add = std::make_shared<Add>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(add);
		break;
	}
	case IR_FAdd: {
		// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTFADD;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		double immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stof(convertImmediate(returnType, immOps.at(0));
		auto fadd = std::make_shared<FAdd>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(fadd);
		break;
	}
	case IR_Sub: {
		// <result> = sub <ty> <op1>, <op2>          ; yields {ty}:result
		// <result> = sub nuw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = sub nsw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = sub nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTSUB;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto sub = std::make_shared<Sub>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(sub);
		break;
	}
	case IR_FSub: {
		// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTFSUB;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		double immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stof(convertImmediate(returnType, immOps.at(0));
		auto fsub = std::make_shared<FSub>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(fsub);_Req = NULL;
                          _CurrCycle = 0; 
                          _Usage = 0;
		break;
	}
	case IR_Mul: {
		// <result> = mul <ty> <op1>, <op2>          ; yields {ty}:result
		// <result> = mul nuw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = mul nsw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = mul nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTMUL;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto mul = std::make_shared<Mul>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(mul);
		break;
	}
	case IR_FMul: {
		// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result.
		instructionType = "Binary";
		maxCycles = CYCLECOUNTFMUL;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		double immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stof(convertImmediate(returnType, immOps.at(0));
		auto fmul = std::make_shared<FMul>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(fmul);
		break;
	}
	case IR_UDiv: {
		// <result> = udiv <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = udiv exact <ty> <op1>, <op2>; yields{ ty }:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTUDIV;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto udiv = std::make_shared<UDiv>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(udiv);
		break;
	}
	case IR_SDiv: {
		// <result> = sdiv <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = sdiv exact <ty> <op1>, <op2>; yields{ ty }:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTSDIV;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto sdiv = std::make_shared<SDiv>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(sdiv);
		break;
	}
	case IR_FDiv: {
		// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTFDIV;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		double immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stof(convertImmediate(returnType, immOps.at(0));
		auto fdiv = std::make_shared<FDiv>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(fdiv);
		break;
	}
	case IR_URem: {
		// <result> = urem <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTUREM;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto urem = std::make_shared<URem>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(urem);
		break;
	}
	case IR_SRem: {
		// <result> = srem <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTSREM;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto srem = std::make_shared<SRem>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(srem);
		break;
	}
	case IR_FRem: {
		// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Binary";
		maxCycles = CYCLECOUNTFREM;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		double immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stof(convertImmediate(returnType, immOps.at(0));
		auto frem = std::make_shared<FRem>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(frem);
		break;
	}
	// Bitwise Operations
	case IR_Shl: {
		// <result> = shl <ty> <op1>, <op2>           ; yields {ty}:result
		// <result> = shl nuw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = shl nsw <ty> <op1>, <op2>; yields{ ty }:result
		// <result> = shl nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
		instructionType = "Bitwise";
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto shl = std::make_shared<Shl>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(shl);
		break;
	}
	case IR_LShr: {
		// <result> = lshr <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = lshr exact <ty> <op1>, <op2>; yields{ ty }:result
		instructionType = "Bitwise";
		maxCycles = CYCLECOUNTLSHR;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto lshr = std::make_shared<LShr>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(lshr);
		break;
	}
	case IR_AShr: {
		// <result> = ashr <ty> <op1>, <op2>         ; yields {ty}:result
		// <result> = ashr exact <ty> <op1>, <op2>; yields{ ty }:result.
		instructionType = "Bitwise";
		maxCycles = CYCLECOUNTASHR;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto ashr = std::make_shared<AShr>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(ashr);
		break;
	}
	case IR_And: {
		// <result> = and <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Bitwise";
		maxCycles = CYCLECOUNTAND;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto and = std::make_shared<And>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(and);
		break;
	}
	case IR_Or: {
		// <result> = or <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Bitwise";
		maxCycles = CYCLECOUNTOR;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto or = std::make_shared<Or>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(or);
		break;
	}
	case IR_Xor: {
		// <result> = xor <ty> <op1>, <op2>   ; yields {ty}:result
		instructionType = "Bitwise";
		maxCycles = CYCLECOUNTXOR;
		computeFlags = setFlags(parameters);
		initializeReturnRegister(parameters, ret_reg, returnType, instructionType);
		std::vector<Register*> regOps = setRegOperands(list, parameters, dependencies, instructionType);
		std::vector<std::string> immOps = setImmOperands(list, parameters, dependencies, instructionType);
		int64_t immOp = 0;
		if(!(immOps.at(0).empty())) immOp = stoi(immOps.at(0));
		auto xor = std::make_shared<Xor>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp );
		addNode(xor);
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
		instructionType = "Memory";
		maxCycles = CYCLECOUNTLOAD;
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
		instruction.general.returnType = parameters[index];
		
		// Set return register type and size
		instruction.general.returnRegister->setSize(instruction.memory.load.ty);
		int align = 0;
		while (parameters[align].compare("align") != 0)
		    align++;
		// Determine if register that contains load address exists
		if(isRegister(parameters[align - 1])) setRegister(parameters[align - 1], instruction.memory.load.pointer, dependencies, list, parameters);
		// Set value for alignment
		DPRINTF(ComputeNode, "Align: %s\n", parameters[align+1]);
	        instruction.memory.load.align = stoi(parameters[align + 1]);
		break;
	}
	case IR_Store: {
		// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void}
		// store atomic[volatile] <ty> <value>, <ty>* <pointer>[singlethread] <ordering>, align <alignment>; yields{ void }
		instructionType = "Memory";
		maxCycles = CYCLECOUNTSTORE;
		int index = 1;
		if (parameters[1] == "volatile") {
			index = 2;
			instruction.memory.store.volatileVar = true;
		}
		instruction.memory.store.ty = parameters[index];
		
		if(isRegister(parameters[index + 3])) setRegister(parameters[index + 3], instruction.memory.store.pointer, dependencies, list, parameters);
		else DPRINTF(ComputeNode, "Pointer is an immediate value, not implemented\n");
		
		if(isRegister(parameters[index + 1])) {
			setRegister(parameters[index + 1], instruction.memory.store.value, dependencies, list, parameters);
			instruction.memory.store.value->setSize(instruction.memory.store.ty);
		} else {
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
		instructionType = "Memory";
		maxCycles = CYCLECOUNTGETELEMENTPTR;
		instruction.general.returnRegister->setSize("pointer");
		instruction.general.integer = true;
		instruction.general.multiplier = true;
		if (parameters[0] == "inbounds") {
			instruction.memory.getptr.inbounds = true;
			instruction.memory.getptr.pty = parameters[1];
			instruction.general.returnType = parameters[1];
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
				setRegister(parameters[index+i+1], instruction.memory.getptr.idx[j], dependencies, list, parameters);
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
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTTRUNC;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto trnc = std::make_shared<Trunc>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(trnc);
		break;
	}
	case IR_ZExt: {
		// <result> = zext <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTZEXT;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto zext = std::make_shared<ZExt>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(zext);
		break;
	}
	case IR_SExt: {
		// <result> = sext <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTSEXT;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto sext = std::make_shared<SExt>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(sext);
		break;
	}
	case IR_FPToUI: {
		// <result> = fptoui <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTFPTOUI;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto fptoui = std::make_shared<FPToUI>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(fptoui);
		break;
	}
	case IR_FPToSI: {
		// <result> = fptosi <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTFPTOSI;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto fptosi = std::make_shared<FPToSI>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(fptosi);
		break;
	}
	case IR_UIToFP: {
		// <result> = uitofp <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTUITOFP;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto uitofp = std::make_shared<UIToFP>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(uitofp);
		break;
	}
	case IR_SIToFP: {
		// <result> = sitofp <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTSITOFP;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto sitofp = std::make_shared<SIToFP>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(sitofp);
		break;
	}
	case IR_FPTrunc: {
		// <result> = fptrunc <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTFPTRUNC;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto fptrunc = std::make_shared<FPTrunc>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(fptrunc);
		break;
	}
	case IR_FPExt: {
		// <result> = fpext <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTFPEXT;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto fpext = std::make_shared<FPExt>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(fpext);
		break;
	}
	case IR_PtrToInt: {
		// <result> = ptrtoint <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTPTRTOINT;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto ptrtoint = std::make_shared<PtrToInt>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(ptrtoint);
		break;
	}
	case IR_IntToPtr: {
		// <result> = inttoptr <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTINTTOPTR;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto inttoptr = std::make_shared<IntToPtr>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(inttoptr);
		break;
	}
	case IR_BitCast: {
		// <result> = bitcast <ty> <value> to <ty2>             ; yields ty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTBITCAST;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto bitcast = std::make_shared<BitCast>(	lineCpy, 
												opCode, 
												returnType, 
												instructionType, 
												ret_reg, 
												maxCycles, 
												dependencies, 
												co,
												originalType,
												operand);
		addNode(bitcast);
		break;
	}
	case IR_AddrSpaceCast: {
		// <result> = addrspacecast <pty> <ptrval> to <pty2>       ; yields pty2
		instructionType = "Conversion";
		maxCycles = CYCLECOUNTADDRSPACECAST;
		returnType = parameters[0];
		std::string originalType = parameters[3];
		Register* operand = NULL;
		if(isRegister(parameters[1])) setRegister(parameters[1], operand, dependencies, list, parameters);
		auto addrspacecast = std::make_shared<AddrSpaceCast>(	lineCpy, 
																opCode, 
																returnType, 
																instructionType, 
																ret_reg, 
																maxCycles, 
																dependencies, 
																co,
																originalType,
																operand);
		addNode(addrspacecast);
		break;
	}
	// Other Operations - Compare
	case IR_ICmp: {
		// <result> = icmp <cond> <ty> <op1>, <op2>   ; yields {i1} or {<N x i1>}:result
		instructionType = "Compare";
		std::string condition = parameters[0];
		maxCycles = CYCLECOUNTICMP;
		returnType = parameters[1];
		std::vector<Register*> regOps;
		int64_t immOp = 0;
		Register* op1;
		Register* op2;
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) {
			setRegister(parameters[last], op1, dependencies, list, parameters);	
			regOps.push_back(op1);
		} 
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) {
			setRegister(parameters[last-1], op2, dependencies, list, parameters);
			regOps.push_back(op2);
		} else immOp = stoi(parameters[last - 1]);
		if (condition == "eq") computeFlags = EQ;
		else if (condition == "ne") computeFlags = NE;
		else if (condition == "ugt") computeFlags = UGT;
		else if (condition == "uge") computeFlags = UGE;
		else if (condition == "ult") computeFlags = ULT;
		else if (condition == "ule") computeFlags = ULE;
		else if (condition == "sgt") computeFlags = SGT;
		else if (condition == "sge") computeFlags = SGE;
		else if (condition == "slt") computeFlags = SLT;
		else if (condition == "sle") computeFlags = SLE;
		auto icmp = std::make_shared<ICmp>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp);
		addNode(icmp);
		break;
	}
	case IR_FCmp: {
		// <result> = fcmp <cond> <ty> <op1>, <op2>     ; yields {i1} or {<N x i1>}:result
		instructionType = "Compare";
		std::string condition = parameters[0];
		maxCycles = CYCLECOUNTFCMP;
		returnType = parameters[1];
		std::vector<Register*> regOps;
		double immOp = 0;
		Register* op1;
		Register* op2;
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) {
			setRegister(parameters[last], op1, dependencies, list, parameters);	
			regOps.push_back(op1);
		} 
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) {
			setRegister(parameters[last-1], op2, dependencies, list, parameters);
			regOps.push_back(op2);
		} else immOp = stof(convertImmediate(returnType, parameters[last - 1]);
		
		if (condition == "false") computeFlags = CONDFALSE;
		else if (condition == "oeq") computeFlags = OEQ;
		else if (condition == "ogt") computeFlags = OGR;
		else if (condition == "oge") computeFlags = OGE;
		else if (condition == "olt") computeFlags = OLT;
		else if (condition == "ole") computeFlags = OLE;
		else if (condition == "one") computeFlags = ONE;
		else if (condition == "ord") computeFlags = ORD;
		else if (condition == "ueq") computeFlags = UEQ;
		else if (condition == "ugt") computeFlags = UGT;
		else if (condition == "uge") computeFlags = UGE;
		else if (condition == "ult") computeFlags = ULT;
		else if (condition == "ule") computeFlags = ULE;
		else if (condition == "une") computeFlags = UNE;
		else if (condition == "uno") computeFlags = UNO;
		else if (condition == "true") computeFlags = CONDTRUE;
		auto fcmp = std::make_shared<FCmp>(	lineCpy, 
											opCode, 
											returnType, 
											instructionType, 
											ret_reg, 
											maxCycles, 
											dependencies, 
											co,
											regOps,
											computeFlags,
											immOp);
		addNode(fcmp);
		break;
	}
	case IR_PHI: {
		// <result> = phi <ty> [ <val0>, <label0>], ...
		instructionType = "Other";
		instruction.general.phi = true;
		
		maxCycles = CYCLECOUNTPHI;
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
				setRegister(val[i-1], instruction.other.phi.val[i - 1], dependencies, list, parameters);
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
		instructionType = "Other";
		maxCycles = CYCLECOUNTSELECT;
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
		if(isRegister(parameters[1])) setRegister(parameters[1], instruction.other.select.cond, dependencies, list, parameters);
		else {
			instruction.other.select.icondFlag = true;
			if(parameters[1] == "true") instruction.other.select.icond = true;
		}
		if(isRegister(parameters[3])) setRegister(parameters[3], instruction.other.select.val1, dependencies, list, parameters);
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
		if(isRegister(parameters[5])) setRegister(parameters[5], instruction.other.select.val2, dependencies, list, parameters);
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


BasicBlock::BasicBlock(const std::string& Name, uint64_t BBID) {
    //cnList = new std::list<ComputeNode *>();
    _Name = Name;
    _BBID = BBID;
}

void
BasicBlock::addNode(InstructionBase* Node) {
    _Nodes->push_back(Node);
}

std::string 
BasicBlock::convertImmediate(std::string dataType, std::string immediateValue) {
	int arr1 = 0;
	int arr2 = 0;
	int integer = 0;
	double doub;
	float flt;
	std::string temp;
	char *array = &immediateValue[0];
	char *end;
	DPRINTF(IOAcc, "Type: %s, Value: %s\n",dataType, immediateValue);
	if(dataType.compare("double") == 0) {
		if(immediateValue[1] == 'x') {
			doub = strtol(array, &end, 16);
			uint64_t convert = (uint64_t) doub;
			doub = *((double*)&convert);
			temp = std::to_string(doub);
		} else temp = sciToDecimal(immediateValue);
	} else if(dataType.compare("float") == 0) {
		if(immediateValue[1] == 'x') {
			flt = strtol(array, &end, 16);
			uint64_t convert = (uint64_t) flt;
			doub = *((float*)&convert);
			temp = std::to_string(flt);
		} else temp = sciToDecimal(immediateValue);
	} else { // Integer Value
		if(immediateValue[1] == 'x') {
			integer = strtol(array, &end, 0);
			temp = std::to_string(integer);
		} else temp = sciToDecimal(immediateValue);
	}
	DPRINTF(IOAcc, "Value: %s, %d, %d, %d\n", temp, doub, arr1, arr2);
	return temp;
}

std::string 
BasicBlock::sciToDecimal(std::string immediateValue) {
	int decimalLocation = 0;
	int magnitudeLoc = 0;
	int magnitude = 0;

	for(int i = 0; i < immediateValue.length()-1; i++) {
		if(immediateValue[i] == '.') decimalLocation = i;
		if(immediateValue[i] == 'e') magnitudeLoc = i;
	}
	magnitude = stoi(immediateValue.substr(magnitudeLoc+2));
	for(int i = decimalLocation; i < decimalLocation+magnitude; i++) {
		immediateValue[i] = immediateValue[i+1];
	}
	immediateValue[decimalLocation+magnitude] = '.';
	immediateValue = immediateValue.substr(0,magnitudeLoc);
	
	return immediateValue;
}

void
BasicBlock::debugParams(std::vector<std::string> &parameters) { 
	DPRINTF(ComputeNode, "Creating (%s) Compute Node:\n", opCode);
	if(DEBUGPARAMS) for (int i = 0; i < parameters.size(); i++) DPRINTF(ComputeNode, "Parameter[%d]: (%s)\n", i, parameters[i]);
	DPRINTF(ComputeNode, "\n");
}

uint64_t
BasicBlock::setFlags(std::vector<std::string> &parameters) {
	/*
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
	*/
	return 0;
}

bool 
BasicBlock::isRegister(std::string data){
	if(data[0] == '%') return true;
	return false;
}

void
BasicBlock::setRegister(std::string data, Register *&reg, std::vector<Register*> &dependencies, RegisterList *list, std::vector<std::string> &parameters) {
	std::string name = data.substr(1);
	if (list->findRegister(name) == NULL) {
		reg = new Register(name);
		list->addRegister(reg);
		dependencies.push_back(reg);
		} else {
		reg = list->findRegister(name);
		dependencies.push_back(reg);
	}
}

void
BasicBlock::initializeReturnRegister(std::vector<std::string> &parameters, Register *&reg , std::string &returnType, const std::string &instructionType ){
	int last = parameters.size() - 1;
	if(!(instructionType.compare("Binary"))){
		// Set instruction return type <ty>
		returnType = parameters[last - 2];
		// Set size of return register to match instruction return type
		reg->setSize(returnType);	
	}
	else if(!(instructionType.compare("Bitwise"))){
		// Set instruction return type <ty>
		returnType = parameters[last - 2];
		// Set size of return register to match instruction return type
		reg->setSize(returnType);
	}
}

std::vector<Register*>
BasicBlock::setRegOperands(RegisterList *list, std::vector<std::string> &parameters, std::vector<Register*> &dependencies, const string& instructionType) {
	int last = parameters.size() - 1;
	Register* op;
	std::vector<Register*> operands;
	if(!(instructionType.compare("Binary"))){
		// Operand 2
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) setRegister(parameters[last], op, dependencies, list, parameters);
		// Operand 1
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) setRegister(parameters[last-1], op, dependencies, list, parameters);

	} else if(!(instructionType.compare("Bitwise"))) {
		// Operand 2
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) setRegister(parameters[last], op, dependencies, list, parameters);
		// Operand 1
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) setRegister(parameters[last-1], op, dependencies, list, parameters);
	}
}

std::vector<std::string>
BasicBlock::setImmOperands(RegisterList *list, std::vector<std::string> &parameters, std::vector<Register*> &dependencies, const string& instructionType) {
	int last = parameters.size() - 1;
	std::vector<std::string> operands;
	if(!(instructionType.compare("Binary"))){
		// Operand 2
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) 
		else {
			// Operation uses immediate value
			// Load string representation of immediate value
			operands.push_back([last]);
		}
		// Operand 1
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) 
		else {
			// Operation uses immediate value
			// Load string representation of immediate value
			operands.push_back(parameters[last - 1]);
		}
	} else if(!(instructionType.compare("Bitwise"))) {
		// Operand 2
		// Check if adding from register or immediate value
		if(isRegister(parameters[last])) 
		else {
			// Operation uses immediate value
			// Load string representation of immediate value
			operands.push_back(parameters[last]);
		}
		// Operand 1
		// Check if value is from register or immediate value
		if(isRegister(parameters[last-1])) 
		else {
			// Operation uses immediate value
			// Load string representation of immediate value
			operands.push_back(parameters[last - 1]);
		}
	}
}

int
BasicBlock::setSize(std::string dataType) {
    // Code here will infer size based around the stored dataType string
    std::string temp = dataType;
	int size = 8;
    // Pointers
    if (temp.compare("pointer") == 0) size = SystemSize/8;
    else if (temp.back() =='*') size = SystemSize/8;
    // Boolean and integer data types
    // Set size if dataType is integer
    else if (temp.front() == 'i') {
        temp = temp.substr(1);
        size = ((std::stoi(temp)-1)/8)+1;
    }
    // Floating point data types    
    // Set size if dataType is float
    else if (temp.compare("float")  != -1) size = SystemSize/16;
    // Set size if dataType is double
    else if (temp.find("double") > -1) size = SystemSize/8;
    // Set size if dataType is void
    else if (temp.find("void") > -1) size = 0;
    // Aggregate data types
    // Array dataType
    else if (temp[0] == '[') { }
    // Struct dataType
    else if (temp.find("{") > -1) { }
    // Unspecified dataType
    // Label
    // Treat size equivalent to a pointer
    else if (temp.find("label") > -1) size = SystemSize/8;
    // Unknown dataType
    else { }
	return size;
}