#include "compute_node.hh"

ComputeNode::ComputeNode(std::string line, RegisterList *list, std::string prev) {	

	std::vector<std::string> parameters;
	int leftDelimeter, rightDelimeter, lastInLine;
    int returnChk = line.find(" = ");
	int last = 0;
	instruction.general.llvm_Line = line;
	int n = 1; 
	int dependencies = 0;
	prevBB = prev;
    // ////////////////////////////////////////////////////////////////////
   

    // Find the return register. If it exists, it is always the first component of the line
    if (returnChk > 0) {
    	// Create new pointer to the return register
        instruction.general.returnRegister = new Register(line.substr((line.find("%")+1), returnChk));
        list->addRegister(instruction.general.returnRegister);
        // In all instances where a return register is the first component, the next component is
        // the opcode, which is parsed and removed from line
        line = line.substr(returnChk+3);
    	instruction.general.opCode = line.substr(0,line.find(' ')-1);
    	line = line.substr(line.find(' '));
    }
    // If no return register is found then the first component must instead be the opcode
    // as of LLVM 3.4 instruction types
    else {
    	// Search for first none empty space which is were the opcode must begin
    	// Then store the opcode and remove the parsed information from the line
    	for(int i = 0; i < line.length(); i++){
    		if(line[0] != ' ') {
				instruction.general.opCode = line.substr(i, line.find(' ')-1);
    			line = line.substr(line.find(' '));
    			break;
    		}
    	}
    	// No return register needed
    	instruction.general.returnRegister = NULL;
    }
    // //////////////////////////////////////////////////////////////////////////////////


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
    			}

    			else if (line[i] == '[') {
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
    			}

    			else if (line[i] == '{') {
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
    			}
    			else if (line[i] == '<') {
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
    			}
    			// End of functions that search for encapulated components

    			// The else condition catches all other components of the function and stores them
    			// in the order found as parameters

    			else {
    				leftDelimeter = i;
    				if (line[i] == '=') {
    					//Ignore
    				}
    				else if (line[i] == ',') {
    					//Ignore
    				}
    				else if (line.substr(i + 1).find(" ") > line.substr(i + 1).find(",")) {
    					rightDelimeter = 1 + line.substr(i + 1).find(",");
    					parameters.push_back(line.substr(leftDelimeter, rightDelimeter));
    					i += rightDelimeter;

    				}
    				else if (line.substr(i + 1).find(" ") < line.substr(i + 1).find(",")) {
    					rightDelimeter = 1 + line.substr(i + 1).find(" ");
    					parameters.push_back(line.substr(leftDelimeter, rightDelimeter));
    					i += rightDelimeter;

    				}
    				else if (line.substr(i + 1).find(" ") == -1) {
    					parameters.push_back(line.substr(leftDelimeter));
    					i = (line.length()-1);
    					//This will always be the last object on the line

    				}
    			}
    		}
    	}
		

		instruction.general.fields = parameters.size();
		last = (parameters.size() - 1);
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
				instruction.general.terminator = true;
				instruction.general.flowControl = true;
				if (parameters[last].find("void")) {
					// If void is found then it must not have a return value
					instruction.terminator.type = "void";
				}
				else {
					// If void is not found then the last parameters must be the return value,
					// and preceding it the return type
					instruction.terminator.type = parameters[last - 1];
					if (parameters[last][0] == '%') {
						// Find if return value references a register
						instruction.terminator.ivalue = "void";
						instruction.terminator.intermediate = false;
						if (list->findRegister(parameters[last]) == NULL) {
							instruction.terminator.value = new Register(parameters[last]);
							list->addRegister(instruction.terminator.value);
							instruction.terminator.value->setSize(instruction.terminator.type);
							instruction.dependencies.registers[0] = instruction.terminator.value;
						}
						else {
							instruction.terminator.value = list->findRegister(parameters[last]);
							instruction.terminator.value->setSize(instruction.terminator.type);
							instruction.dependencies.registers[0] = instruction.terminator.value;
						}
					}
					else {
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
				if (parameters.size() == 2) {
					//Unconditional branch
					instruction.terminator.unconditional = true;
					instruction.terminator.type = "void";
					// Check if register already exists and create new one if not
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.terminator.dest = new Register(parameters[last]);
						list->addRegister(instruction.terminator.dest);
						instruction.terminator.dest->setSize(instruction.terminator.type);
						instruction.dependencies.registers[0] = instruction.terminator.dest;
					}
					else {
						instruction.terminator.dest = list->findRegister(parameters[last]);
						instruction.terminator.dest->setSize(instruction.terminator.type);
						instruction.dependencies.registers[0] = instruction.terminator.dest;
					}
					instruction.general.returnRegister = instruction.terminator.dest;
				}
				else {
					// Conditional Branch
					// Check if register already exists and create new one if not
					instruction.terminator.type = parameters[0];
					if (list->findRegister(parameters[last-2]) == NULL) {
						instruction.terminator.iftrue = new Register(parameters[last-2]);
						list->addRegister(instruction.terminator.iftrue);
						instruction.terminator.iftrue->setSize("label");
						instruction.dependencies.registers[1] = instruction.terminator.iftrue;
					}
					else {
						instruction.terminator.iftrue = list->findRegister(parameters[last - 2]);
						instruction.dependencies.registers[1] = instruction.terminator.iftrue;
						instruction.terminator.iftrue->setSize("label");
					}
					// Check if register already exists and create new one if not
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.terminator.iffalse = new Register(parameters[last]);
						list->addRegister(instruction.terminator.iffalse);
						instruction.terminator.iffalse->setSize("label");
						instruction.dependencies.registers[2] = instruction.terminator.iffalse;
					}
					else {
						instruction.terminator.iffalse = list->findRegister(parameters[last]);
						instruction.dependencies.registers[2] = instruction.terminator.iffalse;
						instruction.terminator.iffalse->setSize("label");
					}
					// Check if register already exists and create new one if not
					if (list->findRegister(parameters[1]) == NULL) {
						instruction.terminator.cond = new Register(parameters[1]);
						list->addRegister(instruction.terminator.cond);
						instruction.terminator.cond->setSize(instruction.terminator.type);
						instruction.dependencies.registers[0] = instruction.terminator.cond;
					}
					else {
						instruction.terminator.cond = list->findRegister(parameters[1]);
						instruction.dependencies.registers[0] = instruction.terminator.cond;
						instruction.terminator.cond->setSize(instruction.terminator.type);
					}
				}
				break;
			}

			case IR_Switch: {
				// Not up to date with setting register datatypes

				// switch <intty> <value>, label <defaultdest> [ <intty> <val>, label <dest> ... ]
				// When using a switch statement the default case is within instruction.terminator
				// while each case statement exists within instruction.terminator.cases
				instruction.general.terminator = true;
				instruction.general.flowControl = true;
				instruction.terminator.intty = parameters[0];
				if (parameters.size() <= 5) {
					if (list->findRegister(parameters[1]) == NULL) {
						instruction.terminator.value = new Register(parameters[1]);
						list->addRegister(instruction.terminator.value);
						instruction.dependencies.registers[0] = instruction.terminator.value;
					}
					else {
						instruction.terminator.value = list->findRegister(parameters[1]);
						instruction.dependencies.registers[0] = instruction.terminator.value;
					}

					if (list->findRegister(parameters[3]) == NULL) {
						instruction.terminator.defaultdest = new Register(parameters[3]);
						list->addRegister(instruction.terminator.defaultdest);
						instruction.dependencies.registers[1] = instruction.terminator.defaultdest;
					}
					else {
						instruction.terminator.defaultdest = list->findRegister(parameters[3]);
						instruction.dependencies.registers[1] = instruction.terminator.defaultdest;
					}
				}
				
				else {
					// Since the switch statement will always be n*4 elements where n is the number of case statements (min 1 = default only)

					// NOTE *** BUG BUG *** NOTE //
					// Currently the parser will see the entire struct as a single element, that will need to be broken apart here
					// to make this section of the code functionable. 
					n = parameters.size() / 4;
					instruction.terminator.cases.statements = n;
					while (n >= 1) {
						instruction.terminator.cases.intty[n - 1] = parameters[((n - 1) * 4)];
						if (list->findRegister(parameters[((n - 1) * 4) + 1]) == NULL) {
							instruction.terminator.cases.value[n - 1] = new Register(parameters[((n - 1) * 4) + 1]);
							list->addRegister(instruction.terminator.cases.value[n - 1]);
							instruction.dependencies.registers[(n * 2) - 2] = instruction.terminator.cases.value[n - 1];
						}
						else {
							instruction.terminator.cases.value[n - 1] = list->findRegister(parameters[((n - 1) * 4) + 1]);
							instruction.dependencies.registers[(n * 2) - 2] = instruction.terminator.cases.value[n - 1];
						}

						if (list->findRegister(parameters[((n - 1) * 4) + 3]) == NULL) {
							instruction.terminator.cases.dest[n - 1] = new Register(parameters[((n - 1) * 4) + 3]);
							list->addRegister(instruction.terminator.cases.dest[n - 1]);
							instruction.dependencies.registers[((n * 2) - 1)] = instruction.terminator.cases.dest[n - 1];
						}
						else {
							instruction.terminator.cases.dest[n - 1] = list->findRegister(parameters[((n - 1) * 4) + 3]);
							instruction.dependencies.registers[(n * 2) - 1] = instruction.terminator.cases.dest[n - 1];
						}
						n--;
					}
				}			
				break;
			}

			case IR_IndirectBr: {
				// Not up to date with setting register datatypes

				// indirectbr <somety>* <address>, [ label <dest1>, label <dest2>, ... ]
				instruction.general.terminator = true;
				instruction.general.flowControl = true;
				if (parameters.size() <= 3) {
					instruction.terminator.somety = parameters[0];
					if (list->findRegister(parameters[1]) == NULL) {
						instruction.terminator.Addr = new Register(parameters[1]);
						list->addRegister(instruction.terminator.Addr);
						instruction.dependencies.registers[0] = instruction.terminator.Addr;
					}
					else {
						instruction.terminator.Addr = list->findRegister(parameters[1]);
						instruction.dependencies.registers[0] = instruction.terminator.Addr;
					}
				}
				else {
					// NOTE *** BUG BUG *** NOTE //
					// Currently the parser will see the entire struct as a single element, that will need to be broken apart here
					// to make this section of the code functionable. 

					n = parameters.size() / 2;
					instruction.terminator.cases.statements = n;

					while (n >= 1) {
						if (list->findRegister(parameters[((n - 1) * 2) + 1]) == NULL) {
							instruction.terminator.cases.dest[n - 1] = new Register(parameters[((n - 1) * 2) + 1]);
							list->addRegister(instruction.terminator.cases.dest[n - 1]);
							instruction.dependencies.registers[n - 1] = instruction.terminator.cases.dest[n - 1];
						}
						else {
							instruction.terminator.cases.dest[n - 1] = list->findRegister(parameters[((n - 1) * 2) + 1]);
							instruction.dependencies.registers[n - 1] = instruction.terminator.cases.dest[n - 1];
						}
						n--;
					}
				}

				break;
			}

			case IR_Invoke: {
				// Not up to date with setting register datatypes

				// <result> = invoke [cconv] [ret attrs] <ptr to function ty> <function ptr val>(<function args>) [fn attrs]
				//   to label <normal label> unwind label <exception label>
				instruction.general.terminator = true;
				instruction.general.flowControl = true;

				if (list->findRegister(parameters[last]) == NULL) {
					instruction.terminator.exception_label = new Register(parameters[last]);
					list->addRegister(instruction.terminator.exception_label);
					instruction.dependencies.registers[0] = instruction.terminator.exception_label;
				}
				else {
					instruction.terminator.exception_label = list->findRegister(parameters[last]);
					instruction.dependencies.registers[0] = instruction.terminator.exception_label;
				}

				if (list->findRegister(parameters[last-3]) == NULL) {
					instruction.terminator.normal_label = new Register(parameters[last-3]);
					list->addRegister(instruction.terminator.normal_label);
					instruction.dependencies.registers[1] = instruction.terminator.normal_label;
				}
				else {
					instruction.terminator.normal_label = list->findRegister(parameters[last-3]);
					instruction.dependencies.registers[1] = instruction.terminator.normal_label;
				}

				// Determine the calling convention, default to c if none specified
					if (parameters[0] == "ccc") instruction.attributes.cconv.ccc = true;
					else if (parameters[0] == "fastcc") instruction.attributes.cconv.fastcc = true;
					else if (parameters[0] == "coldcc") instruction.attributes.cconv.coldcc = true;
					else if (parameters[0] == "cc10") instruction.attributes.cconv.cc10 = true;
					else if (parameters[0] == "cc11") instruction.attributes.cconv.cc11 = true;
					else instruction.attributes.cconv.ccc = true;

				// Implementation incomplete

				break;
			}

			case IR_Resume: {
				// Not up to date with setting register datatypes

				// resume <type> <value>
				instruction.general.terminator = true;
				instruction.general.flowControl = true;

				instruction.terminator.type = parameters[0];

				if (list->findRegister(parameters[1]) == NULL) {
					instruction.terminator.value = new Register(parameters[1]);
					list->addRegister(instruction.terminator.value);
					instruction.dependencies.registers[0] = instruction.terminator.value;
				}
				else {
					instruction.terminator.value = list->findRegister(parameters[1]);
					instruction.dependencies.registers[0] = instruction.terminator.value;
				}
				break;

			}

			case IR_Unreachable: {
				// Not up to date with setting register datatypes

				// The unreachable instruction has no defined semantics.
				break;
			}

			// Binary Operations

			case IR_Add: {
				// <result> = add <ty> <op1>, <op2>          ; yields {ty}:result
				// <result> = add nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = add nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = add nuw nsw <ty> <op1>, <op2>; yields{ ty }:result

				dependencies = 0;

				for (int i = 0; i <= 1; i++) {
					if (parameters[i] == "nuw") instruction.flags.nuw = true;
					if (parameters[i] == "nsw") instruction.flags.nsw = true;
				}

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last-1][0] == '%') {
					if (list->findRegister(parameters[last-1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last-1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last-1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last-1];
				}



				break;
			}

			case IR_FAdd: {
				// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result

					for(int i = 0; i < parameters.size() - 3; i++){
					if (parameters.at(i) == "nnan") instruction.flags.nnan = true;
					else if (parameters.at(i) == "ninf") instruction.flags.ninf = true;
					else if (parameters.at(i) == "nsz") instruction.flags.nsz = true;
					else if (parameters.at(i) == "arcp") instruction.flags.arcp = true;
					else if (parameters.at(i) == "contract") instruction.flags.contract = true;
					else if (parameters.at(i) == "afn") instruction.flags.afn = true;
					else if (parameters.at(i) == "reassoc") instruction.flags.reassoc = true;
					else if (parameters.at(i) == "fast") instruction.flags.fast = true;
				}

				dependencies = 0;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);


				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}

				break;
			}

			case IR_Sub: {
				// <result> = sub <ty> <op1>, <op2>          ; yields {ty}:result
				// <result> = sub nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = sub nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = sub nuw nsw <ty> <op1>, <op2>; yields{ ty }:result

				dependencies = 0;

				for (int i = 0; i <= 1; i++) {
					if (parameters[i] == "nuw") instruction.flags.nuw = true;
					if (parameters[i] == "nsw") instruction.flags.nsw = true;
				}

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

					// Check if adding from register or immediate value
					if (parameters[last][0] == '%') {
						if (list->findRegister(parameters[last]) == NULL) {
							instruction.binary.op2 = new Register(parameters[last]);
							list->addRegister(instruction.binary.op2);
							instruction.dependencies.registers[dependencies] = instruction.binary.op2;
							dependencies++;
						}
						else {
							instruction.binary.op2 = list->findRegister(parameters[last]);
							instruction.dependencies.registers[dependencies] = instruction.binary.op2;
							dependencies++;
						}
					}
					else {
						instruction.binary.immediate2 = true;
						instruction.binary.iop2 = parameters[last];
					}

					// Check if value is from register or immediate value
					if (parameters[last - 1][0] == '%') {
						if (list->findRegister(parameters[last - 1]) == NULL) {
							instruction.binary.op1 = new Register(parameters[last - 1]);
							list->addRegister(instruction.binary.op1);
							instruction.dependencies.registers[dependencies] = instruction.binary.op1;
							dependencies++;
						}
						else {
							instruction.binary.op1 = list->findRegister(parameters[last - 1]);
							instruction.dependencies.registers[dependencies] = instruction.binary.op1;
							dependencies++;
						}
					}
					else {
						instruction.binary.immediate1 = true;
						instruction.binary.iop1 = parameters[last - 1];
					}




				break;
			}

			case IR_FSub: {
				// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result

				for (int i = 0; i < parameters.size() - 3; i++) {
					if (parameters.at(i) == "nnan") instruction.flags.nnan = true;
					else if (parameters.at(i) == "ninf") instruction.flags.ninf = true;
					else if (parameters.at(i) == "nsz") instruction.flags.nsz = true;
					else if (parameters.at(i) == "arcp") instruction.flags.arcp = true;
					else if (parameters.at(i) == "contract") instruction.flags.contract = true;
					else if (parameters.at(i) == "afn") instruction.flags.afn = true;
					else if (parameters.at(i) == "reassoc") instruction.flags.reassoc = true;
					else if (parameters.at(i) == "fast") instruction.flags.fast = true;
				}

				dependencies = 0;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}

				break;
			}

			case IR_Mul: {
				// <result> = mul <ty> <op1>, <op2>          ; yields {ty}:result
				// <result> = mul nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = mul nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = mul nuw nsw <ty> <op1>, <op2>; yields{ ty }:result

				dependencies = 0;

				for (int i = 0; i <= 1; i++) {
					if (parameters[i] == "nuw") instruction.flags.nuw = true;
					if (parameters[i] == "nsw") instruction.flags.nsw = true;
				}

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}


				break;
			}

			case IR_FMul: {
				// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result


				for (int i = 0; i < parameters.size() - 3; i++) {
					if (parameters.at(i) == "nnan") instruction.flags.nnan = true;
					else if (parameters.at(i) == "ninf") instruction.flags.ninf = true;
					else if (parameters.at(i) == "nsz") instruction.flags.nsz = true;
					else if (parameters.at(i) == "arcp") instruction.flags.arcp = true;
					else if (parameters.at(i) == "contract") instruction.flags.contract = true;
					else if (parameters.at(i) == "afn") instruction.flags.afn = true;
					else if (parameters.at(i) == "reassoc") instruction.flags.reassoc = true;
					else if (parameters.at(i) == "fast") instruction.flags.fast = true;
				}

				dependencies = 0;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}

				break;
			}

			case IR_UDiv: {
				// <result> = udiv <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = udiv exact <ty> <op1>, <op2>; yields{ ty }:result
				dependencies = 0;
			
				if (parameters[0] == "exact") instruction.flags.exact = true;				

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}


				break;
			}

			case IR_SDiv: {
				// <result> = sdiv <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = sdiv exact <ty> <op1>, <op2>; yields{ ty }:result

				dependencies = 0;

				if (parameters[0] == "exact") instruction.flags.exact = true;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}

				break;
			}

			case IR_FDiv: {
				// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result


				for (int i = 0; i < parameters.size() - 3; i++) {
					if (parameters.at(i) == "nnan") instruction.flags.nnan = true;
					else if (parameters.at(i) == "ninf") instruction.flags.ninf = true;
					else if (parameters.at(i) == "nsz") instruction.flags.nsz = true;
					else if (parameters.at(i) == "arcp") instruction.flags.arcp = true;
					else if (parameters.at(i) == "contract") instruction.flags.contract = true;
					else if (parameters.at(i) == "afn") instruction.flags.afn = true;
					else if (parameters.at(i) == "reassoc") instruction.flags.reassoc = true;
					else if (parameters.at(i) == "fast") instruction.flags.fast = true;
				}

				dependencies = 0;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}

				break;
			}

			case IR_URem: {
				// <result> = urem <ty> <op1>, <op2>   ; yields {ty}:result

				dependencies = 0;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}


				break;
			}

			case IR_SRem: {
				// <result> = srem <ty> <op1>, <op2>   ; yields {ty}:result

				dependencies = 0;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}
			}

			case IR_FRem: {
				// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result


				for (int i = 0; i < parameters.size() - 3; i++) {
					if (parameters.at(i) == "nnan") instruction.flags.nnan = true;
					else if (parameters.at(i) == "ninf") instruction.flags.ninf = true;
					else if (parameters.at(i) == "nsz") instruction.flags.nsz = true;
					else if (parameters.at(i) == "arcp") instruction.flags.arcp = true;
					else if (parameters.at(i) == "contract") instruction.flags.contract = true;
					else if (parameters.at(i) == "afn") instruction.flags.afn = true;
					else if (parameters.at(i) == "reassoc") instruction.flags.reassoc = true;
					else if (parameters.at(i) == "fast") instruction.flags.fast = true;
				}

				dependencies = 0;

				instruction.binary.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.binary.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.binary.op2 = new Register(parameters[last]);
						list->addRegister(instruction.binary.op2);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
					else {
						instruction.binary.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op2;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate2 = true;
					instruction.binary.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.binary.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.binary.op1);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
					else {
						instruction.binary.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.binary.op1;
						dependencies++;
					}
				}
				else {
					instruction.binary.immediate1 = true;
					instruction.binary.iop1 = parameters[last - 1];
				}

				break;
			}

			// Bitwise Operations

			case IR_Shl: {
				// <result> = shl <ty> <op1>, <op2>           ; yields {ty}:result
				// <result> = shl nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = shl nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = shl nuw nsw <ty> <op1>, <op2>; yields{ ty }:result
				dependencies = 0;

				for (int i = 0; i <= 1; i++) {
					if (parameters[i] == "nuw") instruction.flags.nuw = true;
					if (parameters[i] == "nsw") instruction.flags.nsw = true;
				}

				instruction.bitwise.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.bitwise.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.bitwise.op2 = new Register(parameters[last]);
						list->addRegister(instruction.bitwise.op2);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
					else {
						instruction.bitwise.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate2 = true;
					instruction.bitwise.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.bitwise.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.bitwise.op1);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
					else {
						instruction.bitwise.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate1 = true;
					instruction.bitwise.iop1 = parameters[last - 1];
				}

				break;
			}

			case IR_LShr: {
				// <result> = lshr <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = lshr exact <ty> <op1>, <op2>; yields{ ty }:result

				dependencies = 0;

				if (parameters[0] == "exact") instruction.flags.exact = true;

				instruction.bitwise.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.bitwise.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.bitwise.op2 = new Register(parameters[last]);
						list->addRegister(instruction.bitwise.op2);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
					else {
						instruction.bitwise.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate2 = true;
					instruction.bitwise.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.bitwise.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.bitwise.op1);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
					else {
						instruction.bitwise.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate1 = true;
					instruction.bitwise.iop1 = parameters[last - 1];
				}

				break;
			}

			case IR_AShr: {
				// <result> = ashr <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = ashr exact <ty> <op1>, <op2>; yields{ ty }:result

				dependencies = 0;

				if (parameters[0] == "exact") instruction.flags.exact = true;

				instruction.bitwise.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.bitwise.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.bitwise.op2 = new Register(parameters[last]);
						list->addRegister(instruction.bitwise.op2);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
					else {
						instruction.bitwise.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate2 = true;
					instruction.bitwise.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.bitwise.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.bitwise.op1);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
					else {
						instruction.bitwise.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate1 = true;
					instruction.bitwise.iop1 = parameters[last - 1];
				}
				break;
			}

			case IR_And: {
				// <result> = and <ty> <op1>, <op2>   ; yields {ty}:result
				dependencies = 0;

				instruction.bitwise.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.bitwise.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.bitwise.op2 = new Register(parameters[last]);
						list->addRegister(instruction.bitwise.op2);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
					else {
						instruction.bitwise.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate2 = true;
					instruction.bitwise.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.bitwise.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.bitwise.op1);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
					else {
						instruction.bitwise.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate1 = true;
					instruction.bitwise.iop1 = parameters[last - 1];
				}
				break;
			}

			case IR_Or: {
				// <result> = or <ty> <op1>, <op2>   ; yields {ty}:result

				dependencies = 0;

				instruction.bitwise.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.bitwise.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.bitwise.op2 = new Register(parameters[last]);
						list->addRegister(instruction.bitwise.op2);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
					else {
						instruction.bitwise.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate2 = true;
					instruction.bitwise.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.bitwise.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.bitwise.op1);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
					else {
						instruction.bitwise.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate1 = true;
					instruction.bitwise.iop1 = parameters[last - 1];
				}
				break;
			}

			case IR_Xor: {
				// <result> = xor <ty> <op1>, <op2>   ; yields {ty}:result

				dependencies = 0;

				instruction.bitwise.ty = parameters[last - 2];
				instruction.general.returnRegister->setSize(instruction.bitwise.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.bitwise.op2 = new Register(parameters[last]);
						list->addRegister(instruction.bitwise.op2);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
					else {
						instruction.bitwise.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op2;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate2 = true;
					instruction.bitwise.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last - 1][0] == '%') {
					if (list->findRegister(parameters[last - 1]) == NULL) {
						instruction.bitwise.op1 = new Register(parameters[last - 1]);
						list->addRegister(instruction.bitwise.op1);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
					else {
						instruction.bitwise.op1 = list->findRegister(parameters[last - 1]);
						instruction.dependencies.registers[dependencies] = instruction.bitwise.op1;
						dependencies++;
					}
				}
				else {
					instruction.bitwise.immediate1 = true;
					instruction.bitwise.iop1 = parameters[last - 1];
				}
				break;
			}

			// Memory Operations 

			case IR_Alloca: {
				// Not up to date with setting register datatypes

				// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result

				// attributes.params.dataType = parameters[0];

				// if (parameters[1] == "align") {
				// 	attributes.params.align = parameters[2];
				// }
				// else if (parameters.size() == 5) {
				// 	attributes.params.type2 = parameters[1];
				// 	attributes.params.numElements = parameters[2];
				// 	attributes.params.align = parameters[4];
				// }
				// else {
				// 	attributes.params.type2 = parameters[1];
				// 	attributes.params.numElements = parameters[2];
				// }
				break;
			}
			case IR_Load: {
				//		    uint8_t* getCurData() { return curData; }
				// <result> = load [volatile] <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>]
				// <result> = load atomic[volatile] <ty>* <pointer>[singlethread] <ordering>, align <alignment>
				//	!<index> = !{ i32 1 }
				break;
			}
			case IR_Store: {
				//			comm_interface.hh
				//		    int prepRead(Addr src, size_t length);
				//		    int prepWrite(Addr dst, uint8_t* value, size_t length);
				// store [volatile] <ty> <value>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>]        ; yields {void}
				// store atomic[volatile] <ty> <value>, <ty>* <pointer>[singlethread] <ordering>, align <alignment>; yields{ void }

				break;
			}
			case IR_GetElementPtr: {
				// <result> = getelementptr <pty>* <ptrval>{, <ty> <idx>}*
				// <result> = getelementptr inbounds <pty>* <ptrval>{, <ty> <idx>}*
				// <result> = getelementptr <ptr vector> ptrval, <vector index type> idx
				int index = 0;
				instruction.general.memory = true;
				instruction.general.returnRegister->setSize("pointer");

				if(parameters[0] == "inbounds"){
				instruction.memory.getptr.inbounds = true;
				instruction.memory.getptr.pty = parameters[1];
				instruction.memory.getptr.ptrval = parameters[2];
				index = 3;
				}
				else{
				instruction.memory.getptr.pty = parameters[0];
				instruction.memory.getptr.ptrval = parameters[1];
				index = 2;
				}

				for(int i = index; i <= last; i+=2){
					instruction.memory.getptr.ty[i] = parameters[index+i];
					instruction.memory.getptr.idx[i+1] = parameters[index+i+1];
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

				break;
			}
			case IR_ZExt: {
				// <result> = zext <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_SExt: {
				// <result> = sext <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_FPToUI: {
				// <result> = fptoui <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_FPToSI: {
				// <result> = fptosi <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_UIToFP: {
				// <result> = uitofp <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_SIToFP: {
				// <result> = sitofp <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_FPTrunc: {
				// <result> = fptrunc <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_FPExt: {
				// <result> = fpext <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_PtrToInt: {
				// <result> = ptrtoint <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_IntToPtr: {
				// <result> = inttoptr <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_BitCast: {
				// <result> = bitcast <ty> <value> to <ty2>             ; yields ty2

				break;
			}
			case IR_AddrSpaceCast: {
				// <result> = addrspacecast <pty> <ptrval> to <pty2>       ; yields pty2

				break;
			}

			// Other Operations - Compare
			case IR_ICmp: {
				// <result> = icmp <cond> <ty> <op1>, <op2>   ; yields {i1} or {<N x i1>}:result
				dependencies = 0;
				instruction.general.other = true;
				instruction.general.compare = true;
				instruction.other.compare.condition.cond = parameters[0];
				instruction.other.compare.ty = parameters[1];
				instruction.general.returnRegister->setSize(instruction.other.compare.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.other.compare.op2 = new Register(parameters[last]);
						list->addRegister(instruction.other.compare.op2);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op2;
						dependencies++;
					}
					else {
						instruction.other.compare.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op2;
						dependencies++;
					}
				}
				else {
					instruction.other.compare.immediate2 = true;
					instruction.other.compare.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last-1][0] == '%') {
					if (list->findRegister(parameters[last-1]) == NULL) {
						instruction.other.compare.op1 = new Register(parameters[last-1]);
						list->addRegister(instruction.other.compare.op1);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op1;
						dependencies++;
					}
					else {
						instruction.other.compare.op1 = list->findRegister(parameters[last-1]);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op1;
						dependencies++;
					}
				}
				else {
					instruction.other.compare.immediate1 = true;
					instruction.other.compare.iop1 = parameters[last-1];
				}


				if (instruction.other.compare.condition.cond == "eq") instruction.other.compare.condition.eq = true;
				else if (instruction.other.compare.condition.cond == "ne") instruction.other.compare.condition.ne = true;
				else if (instruction.other.compare.condition.cond == "ugt") instruction.other.compare.condition.ugt = true;
				else if (instruction.other.compare.condition.cond == "uge") instruction.other.compare.condition.uge = true;
				else if (instruction.other.compare.condition.cond == "ult") instruction.other.compare.condition.ult = true;
				else if (instruction.other.compare.condition.cond == "ule") instruction.other.compare.condition.ule = true;
				else if (instruction.other.compare.condition.cond == "sgt") instruction.other.compare.condition.sgt = true;
				else if (instruction.other.compare.condition.cond == "sge") instruction.other.compare.condition.sge = true;
				else if (instruction.other.compare.condition.cond == "slt") instruction.other.compare.condition.slt = true;
				else if (instruction.other.compare.condition.cond == "sle") instruction.other.compare.condition.sle = true;

				break;
			}
			case IR_FCmp: {
				// <result> = fcmp <cond> <ty> <op1>, <op2>     ; yields {i1} or {<N x i1>}:result
				dependencies = 0;
				instruction.general.other = true;
				instruction.general.compare = true;
				instruction.other.compare.condition.cond = parameters[0];
				instruction.other.compare.ty = parameters[1];
				instruction.general.returnRegister->setSize(instruction.other.compare.ty);

				// Check if adding from register or immediate value
				if (parameters[last][0] == '%') {
					if (list->findRegister(parameters[last]) == NULL) {
						instruction.other.compare.op2 = new Register(parameters[last]);
						list->addRegister(instruction.other.compare.op2);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op2;
						dependencies++;
					}
					else {
						instruction.other.compare.op2 = list->findRegister(parameters[last]);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op2;
						dependencies++;
					}
				}
				else {
					instruction.other.compare.immediate2 = true;
					instruction.other.compare.iop2 = parameters[last];
				}

				// Check if value is from register or immediate value
				if (parameters[last-1][0] == '%') {
					if (list->findRegister(parameters[last-1]) == NULL) {
						instruction.other.compare.op1 = new Register(parameters[last-1]);
						list->addRegister(instruction.other.compare.op1);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op1;
						dependencies++;
					}
					else {
						instruction.other.compare.op1 = list->findRegister(parameters[last-1]);
						instruction.dependencies.registers[dependencies] = instruction.other.compare.op1;
						dependencies++;
					}
				}
				else {
					instruction.other.compare.immediate1 = true;
					instruction.other.compare.iop1 = parameters[last-1];
				}



				if (instruction.other.compare.condition.cond  == "false")  instruction.other.compare.condition.condFalse = true;
				else if (instruction.other.compare.condition.cond  == "oeq")  instruction.other.compare.condition.oeq = true;
				else if (instruction.other.compare.condition.cond  == "ogt")  instruction.other.compare.condition.ogt = true;
				else if (instruction.other.compare.condition.cond  == "oge")  instruction.other.compare.condition.oge = true;
				else if (instruction.other.compare.condition.cond  == "olt")  instruction.other.compare.condition.olt = true;
				else if (instruction.other.compare.condition.cond  == "ole")  instruction.other.compare.condition.ole = true;
				else if (instruction.other.compare.condition.cond  == "one")  instruction.other.compare.condition.one = true;
				else if (instruction.other.compare.condition.cond  == "ord")  instruction.other.compare.condition.ord = true;
				else if (instruction.other.compare.condition.cond  == "ueq")  instruction.other.compare.condition.ueq = true;
				else if (instruction.other.compare.condition.cond  == "ugt")  instruction.other.compare.condition.ugt = true;
				else if (instruction.other.compare.condition.cond  == "uge")  instruction.other.compare.condition.uge = true;
				else if (instruction.other.compare.condition.cond  == "ult")  instruction.other.compare.condition.ult = true;
				else if (instruction.other.compare.condition.cond  == "ule")  instruction.other.compare.condition.ule = true;
				else if (instruction.other.compare.condition.cond  == "une")  instruction.other.compare.condition.une = true;
				else if (instruction.other.compare.condition.cond  == "uno")  instruction.other.compare.condition.uno = true;
				else if (instruction.other.compare.condition.cond  == "true")  instruction.other.compare.condition.condTrue = true;

				break;
			}
			case IR_PHI: {
				// <result> = phi <ty> [ <val0>, <label0>], ...
				
				instruction.general.other = true;
				instruction.general.phi = true;
				instruction.general.flowControl = true;
				instruction.other.phi.ty = parameters[0];
				instruction.general.returnRegister->setSize(instruction.other.phi.ty);

				std::string val[last-1], label[last-1];

				// Phi instructions grouped in brackets first listed as pairs in ival array
				for(int i = 1; i <= last; i++){
					instruction.other.phi.ival[i-1] = parameters[i];
					val[i-1] = instruction.other.phi.ival[i-1].substr(1, (instruction.other.phi.ival[i-1].find(',')-1));
					label[i-1] = instruction.other.phi.ival[i-1].substr((instruction.other.phi.ival[i-1].find(',')+2), (instruction.other.phi.ival[i-1].length()-1));
					if(val[i-1][0] == '%'){
						instruction.other.phi.ival[i-1].clear();
						instruction.other.phi.immVal[i-1] = false;
						if (list->findRegister(val[i-1]) == NULL) {
						instruction.other.phi.val[i-1] = new Register(val[i-1]);
						list->addRegister(instruction.other.phi.val[i-1]);
						instruction.dependencies.registers[dependencies] = instruction.other.phi.val[i-1];
						dependencies++;
					}
					else {
						instruction.other.phi.val[i-1] = list->findRegister(val[i-1]);
						instruction.dependencies.registers[dependencies] = instruction.other.phi.val[i-1];
						dependencies++;
					}
					}
					else{
						instruction.other.phi.ival[i-1] = val[i-1];
						instruction.other.phi.immVal[i-1] = true;
					}

					if(label[i-1][0] == '%'){
						instruction.other.phi.ilabel[i-1].clear();
						instruction.other.phi.immLabel[i-1] = false;
						if (list->findRegister(label[i-1]) == NULL) {
						instruction.other.phi.label[i-1] = new Register(label[i-1]);
						list->addRegister(instruction.other.phi.label[i-1]);
						instruction.dependencies.registers[dependencies] = instruction.other.phi.label[i-1];
						dependencies++;
					}
					else {
						instruction.other.phi.label[i-1] = list->findRegister(label[i-1]);
						instruction.dependencies.registers[dependencies] = instruction.other.phi.label[i-1];
						dependencies++;
					}
					}
					else{
						instruction.other.phi.ilabel[i-1] = label[i-1];
						instruction.other.phi.immLabel[i-1] = true;
					}

				}

				// //Improvement Opportunity - Dynamically allocated string to eliminate PHIPATHMAX
				// attributes.params.dataType = parameters[0];
				// for (int i = 1; i <= PHIPATHMAX; i++) {
				// 	if (!(parameters[i].empty())) {
				// 		attributes.phi.paths[i - 1] = parameters[i];
				// 	}
				// }
				break;
			}
			case IR_Call: {
				// <result> = [tail] call[cconv][ret attrs] <ty>[<fnty>*] <fnptrval>(<function args>)[fn attrs]

				break;
			}
			case IR_Select: {
				// <result> = select selty <cond>, <ty> <val1>, <ty> <val2>             ; yields ty
				// selty is either i1 or {<N x i1>}


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
			case IR_DMAFence: {

				break;
			}
			case IR_DMAStore: {

				break;
			}
			case IR_DMALoad: {

				break;
			}
			case IR_IndexAdd: {

				break;
			}
			case IR_SilentStore: {

				break;
			}
			case IR_Sine: {

				break;
			}
			case IR_Cosine: {

				break;
			}
			case IR_Move: {

				break;
			}
    		default: {

    			break;
    		}
    		}
    	}


void
ComputeNode::compute() {
	
	switch (s_opMap[instruction.general.opCode]) {
	case IR_Move: {
	
		break;
	}
	case IR_Ret: {
		Operations::llvm_ret(instruction);
		break;
	}
	case IR_Br: {
		Operations::llvm_br(instruction);
		break;
	}
	case IR_Switch: {
		Operations::llvm_switch(instruction);
		break;
	}
	case IR_IndirectBr: {
		Operations::llvm_indirectbr(instruction);
		break;
	}
	case IR_Invoke: {
		Operations::llvm_invoke(instruction);
		break;
	}
	case IR_Resume: {
		Operations::llvm_resume(instruction);
		break;
	}
	case IR_Unreachable: {
		Operations::llvm_unreachable(instruction);
		break;
	}
	case IR_Add: {
		Operations::llvm_add(instruction);
		break;
	}
	case IR_FAdd: {
		Operations::llvm_fadd(instruction);
		break;
	}
	case IR_Sub: {
		Operations::llvm_sub(instruction);
		break;
	}
	case IR_FSub: {
		Operations::llvm_fsub(instruction);
		break;
	}
	case IR_Mul: {
		Operations::llvm_mul(instruction);
		break;
	}
	case IR_FMul: {
		Operations::llvm_fmul(instruction);
		break;
	}
	case IR_UDiv: {
		Operations::llvm_udiv(instruction);
		break;
	}
	case IR_SDiv: {
		Operations::llvm_sdiv(instruction);
		break;
	}
	case IR_FDiv: {
		Operations::llvm_fdiv(instruction);
		break;
	}
	case IR_URem: {
		Operations::llvm_urem(instruction);
		break;
	}
	case IR_SRem: {
		Operations::llvm_srem(instruction);
		break;
	}
	case IR_FRem: {
		Operations::llvm_frem(instruction);
		break;
	}
	case IR_Shl: {
		Operations::llvm_shl(instruction);
		break;
	}
	case IR_LShr: {
		Operations::llvm_lshr(instruction);
		break;
	}
	case IR_AShr: {
		Operations::llvm_ashr(instruction);
		break;
	}
	case IR_And: {
		Operations::llvm_and(instruction);
		break;
	}
	case IR_Or: {
		Operations::llvm_or(instruction);
		break;
	}
	case IR_Xor: {
		Operations::llvm_xor(instruction);
		break;
	}

	case IR_Alloca: {
		Operations::llvm_alloca(instruction);
		break;
	}
	case IR_Load: {
		Operations::llvm_load(instruction);
		break;
	}
	case IR_Store: {
		Operations::llvm_store(instruction);
		break;
	}
	case IR_GetElementPtr: {
		Operations::llvm_getelementptr(instruction);
		break;
	}
	case IR_Fence: {
		Operations::llvm_fence(instruction);
		break;
	}
	case IR_AtomicCmpXchg: {
		Operations::llvm_cmpxchg(instruction);
		break;
	}
	case IR_AtomicRMW: {
		Operations::llvm_atomicrmw(instruction);
		break;
	}
	case IR_Trunc: {
		Operations::llvm_trunc(instruction);
		break;
	}
	case IR_ZExt: {
		Operations::llvm_zext(instruction);
		break;
	}
	case IR_SExt: {
		Operations::llvm_sext(instruction);
		break;
	}
	case IR_FPToUI: {
		Operations::llvm_fptoui(instruction);
		break;
	}
	case IR_FPToSI: {
		Operations::llvm_fptosi(instruction);
		break;
	}
	case IR_UIToFP: {
		Operations::llvm_uitofp(instruction);
		break;
	}
	case IR_SIToFP: {
		Operations::llvm_sitofp(instruction);
		break;
	}
	case IR_FPTrunc: {
		Operations::llvm_fptrunc(instruction);
		break;
	}
	case IR_FPExt: {
		Operations::llvm_fpext(instruction);
		break;
	}
	case IR_PtrToInt: {
		Operations::llvm_ptrtoint(instruction);
		break;
	}
	case IR_IntToPtr: {
		Operations::llvm_inttoptr(instruction);
		break;
	}
	case IR_BitCast: {
		Operations::llvm_bitcast(instruction);
		break;
	}
	case IR_AddrSpaceCast: {
		Operations::llvm_addrspacecast(instruction);
		break;
	}
	case IR_ICmp: {
		Operations::llvm_icmp(instruction);
		break;
	}
	case IR_FCmp: {
		Operations::llvm_fcmp(instruction);
		break;
	}
	case IR_PHI: {
		Operations::llvm_phi(instruction, prevBB);
		break;
	}
	case IR_Call: {
		Operations::llvm_call(instruction);
		break;
	}
	case IR_Select: {
		Operations::llvm_select(instruction);
		break;
	}
	case IR_VAArg: {
		Operations::llvm_vaarg(instruction);
		break;
	}
	case IR_ExtractElement: {
		Operations::llvm_extractelement(instruction);
		break;
	}
	case IR_InsertElement: {
		Operations::llvm_insertelement(instruction);
		break;
	}
	case IR_ShuffleVector: {
		Operations::llvm_shufflevector (instruction);
		break;
	}
	case IR_ExtractValue: {
		Operations::llvm_extractvalue(instruction);
		break;
	}
	case IR_InsertValue: {
		Operations::llvm_insertvalue(instruction);
		break;
	}
	case IR_LandingPad: {
		Operations::llvm_landingpad(instruction);
		break;
	}
	case IR_DMAFence: {
		Operations::llvm_dmafence(instruction);
		break;
	}
	case IR_DMAStore: {
		Operations::llvm_dmastore(instruction);
		break;
	}
	case IR_DMALoad: {
		Operations::llvm_dmaload(instruction);
		break;
	}
	case IR_IndexAdd: {
		Operations::llvm_indexadd(instruction);
		break;
	}
	case IR_SilentStore: {
		Operations::llvm_silentstore(instruction);
		break;
	}
	case IR_Sine: {
		Operations::llvm_sine(instruction);
		break;
	}
	case IR_Cosine: {
		Operations::llvm_cosine(instruction);
		break;
	}

	default: {

		break;
	}
	}


}

bool ComputeNode::commit(){
	if(instruction.general.returnRegister != NULL){
		instruction.general.returnRegister->commit();
		instruction.cycle.current++;
		if(instruction.cycle.current >= instruction.cycle.max){
			instruction.general.returnRegister->commit();
			return true;
		}
	}
	return false;
}

bool ComputeNode::checkDependency(){
	bool hot = true;
	for(int i = 0; i < MAXDEPENDENTS; i++){
		if(instruction.dependencies.registers[i] != NULL){
			if(instruction.dependencies.registers[i]->getStatus() == false) hot = false;
			else hot = true;
		}
	}
	return hot;
}

void ComputeNode::reset(){
	if(instruction.general.returnRegister != NULL){
		instruction.general.returnRegister->reset();
		instruction.cycle.current = 0;
	}
}