#include "compute_node.hh"

ComputeNode::ComputeNode(std::string line, RegisterList *list, std::string prev) {	

	// Components to be moved into the attributes struct in the future
	prevBB = prev;
    attributes.clock.finished = false;
	llvm_inst = line;
	std::string opcode;
	// ///////////////////////////////////////////////////////////////

	// Likely to be made obsolute with new parameter search implemenetation
    int returnChk = line.find(" = ");
    // ////////////////////////////////////////////////////////////////////

    //
    int leftDelimeter, rightDelimeter, lastInLine;
    std::vector<std::string> parameters;
    //


    // Find the return register. If it exists, it is always the first component of the line
    if (returnChk > 0) {
    	// Create new pointer to the return register
        attributes.params.returnRegister = new Register(line.substr((line.find("%")+1), returnChk));
        list->addRegister(attributes.params.returnRegister);
        // In all instances where a return register is the first component, the next component is
        // the opcode, which is parsed and removed from line
        line = line.substr(returnChk+3);
    	opCode = line.substr(0,line.find(' ')-1);
    	line = line.substr(line.find(' '));
    }
    // If no return register is found then the first component must instead be the opcode
    // as of LLVM 3.4 instruction types
    else {
    	// Search for first none empty space which is were the opcode must begin
    	// Then store the opcode and remove the parsed information from the line
    	for(int i = 0; i < line.length(); i++){
    		if(line[0] != ' ') {
    			opcode = line.substr(i, line.find(' ')-1);
    			line = line.substr(line.find(' '));
    			break;
    		}
    	}
    	// No return register needed
    	attributes.params.returnRegister = NULL;
    }

    //This chunk of code to be made obsolete by integrating the new parameter style below
    std::string temp = line.substr(returnChk+3);
	std::string buffer;
	std::stringstream buffer_stream(temp.substr(temp.find(" ") + 1));
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

    	for (int i = (parameters.size() - 1); i >= 0; i--) {

    		switch (s_opMap[opCode]) {

    		case IR_Ret: {
				// ret <type> <value>; Return a value from a non - void function
				// ret void; Return from void function
				if (parameters[i].find("void")) {
					// If void is found then it must not have a return value
					attributes.params.dataType = "void";
					attributes.params.returnValue = "void";
				}
				else {
					// If void is not found then the last parameters must be the return value,
					// and preceding it the return type
					attributes.params.returnValue = parameters[i];
					attributes.params.dataType = parameters[i - 1];
					i = RESET;
				}
				break;
			}
			case IR_Br: {
				// br i1 <cond>, label <iftrue>, label <iffalse>
				// br label <dest>          ; Unconditional branch
				if (parameters.size() == 2) {
					//Unconditional branch
					// Check if register already exists and create new one if not
					attributes.branch.uncond = parameters[i];
					if (list->findRegister(attributes.branch.uncond) == NULL) {
						attributes.branch.labelTrue = new Register(attributes.branch.uncond);
						list->addRegister(attributes.branch.labelTrue);
					}
					else {
						attributes.branch.labelTrue = list->findRegister(attributes.branch.uncond);
					}
					i = RESET;
				}
				else {
					// Conditional Branch
					// Check if register already exists and create new one if not
					attributes.branch.iffalse = parameters[i];
					if (list->findRegister(attributes.branch.iffalse) == NULL) {
						attributes.branch.labelFalse = new Register(attributes.branch.iffalse);
						list->addRegister(attributes.branch.labelFalse);
					}
					else {
						attributes.branch.labelFalse = list->findRegister(attributes.branch.iffalse);
					}
					// Check if register already exists and create new one if not
					attributes.branch.iftrue = parameters[i - 2];
					if (list->findRegister(attributes.branch.iftrue) == NULL) {
					attributes.branch.labelTrue = new Register(attributes.branch.iftrue);
					list->addRegister(attributes.branch.labelTrue);
					}
					else {
						attributes.branch.labelTrue = list->findRegister(attributes.branch.iftrue);
					}
					// Check if register already exists and create new one if not
					attributes.branch.cond = parameters[i - 4];
					if (list->findRegister(attributes.branch.cond) == NULL) {
						attributes.branch.conditionCheck = new Register(attributes.branch.cond);
						list->addRegister(attributes.branch.conditionCheck);
					}
					else {
						attributes.branch.conditionCheck = list->findRegister(attributes.branch.cond);
					}
					i = RESET;
				}
				break;
			}
			case IR_Switch: {
				// switch <intty> <value>, label <defaultdest> [ <intty> <val>, label <dest> ... ]
				attributes.params.dataType = parameters[0];
				attributes.switchStmt.argument = parameters[1];
				attributes.switchStmt.defaultCase = parameters[3];
				attributes.switchStmt.caseStatements = parameters[4];
				i = RESET;
				break;
			}
			case IR_IndirectBr: {
				// indirectbr <somety>* <address>, [ label <dest1>, label <dest2>, ... ]
				attributes.params.dataType = parameters[0];
				attributes.branch.addr = parameters[1];
				attributes.branch.destinationList = parameters[2];
				i = RESET;
				break;
			}
			case IR_Invoke: {
				// <result> = invoke [cconv] [ret attrs] <ptr to function ty> <function ptr val>(<function args>) [fn attrs]
				//   to label <normal label> unwind label <exception label>
				attributes.invoke.unwindlbl = parameters[i];
				attributes.invoke.normallbl = parameters[i - 3];
				if (parameters[2] == "label") {
					attributes.invoke.prototype = parameters[i - 6];
					attributes.params.dataType = parameters[i - 7];
					attributes.cconv.ccc = true;
				}
				else if (parameters[2][parameters[2].size() - 1] == ')') {
					if (parameters[0] == "ccc") attributes.cconv.ccc = true;
					if (parameters[0] == "fastcc") attributes.cconv.fastcc = true;
					if (parameters[0] == "coldcc") attributes.cconv.coldcc = true;
					if (parameters[0] == "cc10") attributes.cconv.cc10 = true;
					if (parameters[0] == "cc11") attributes.cconv.cc11 = true;
					attributes.params.dataType = parameters[1];
					attributes.invoke.prototype = parameters[i - 6];
					attributes.params.dataType = parameters[i - 7];
				}
				else {
					if (parameters[0] == "ccc") attributes.cconv.ccc = true;
					if (parameters[0] == "fastcc") attributes.cconv.fastcc = true;
					if (parameters[0] == "coldcc") attributes.cconv.coldcc = true;
					if (parameters[0] == "cc10") attributes.cconv.cc10 = true;
					if (parameters[0] == "cc11") attributes.cconv.cc11 = true;
					attributes.params.dataType = parameters[1];
					if (parameters[6] == "noreturn") attributes.fattrs.noreturn = true;
					if (parameters[6] == "nounwind") attributes.fattrs.nounwind = true;
					if (parameters[6] == "readonly") attributes.fattrs.readonly = true;
					if (parameters[6] == "readnone") attributes.fattrs.readnone = true;
					attributes.invoke.prototype = parameters[i - 7];
					attributes.params.dataType = parameters[i - 8];
				}
				i = RESET;
				break;
			}
			case IR_Resume: {
				// resume <type> <value>
				attributes.params.dataType = parameters[0];
				attributes.params.returnValue = parameters[1];
				break;
			}
			case IR_Unreachable: {
				// The \91unreachable\91 instruction has no defined semantics.
				break;
			}
			case IR_Add: {
				// <result> = add <ty> <op1>, <op2>          ; yields {ty}:result
				// <result> = add nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = add nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = add nuw nsw <ty> <op1>, <op2>; yields{ ty }:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "nsw") attributes.flags.nsw = true;
					if (parameters[i] == "nuw") attributes.flags.nuw = true;
				}
				break;
			}
			case IR_FAdd: {
				// <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters.at(i) == "nnan") attributes.flags.nnan = true;
					else if (parameters.at(i) == "ninf") attributes.flags.ninf = true;
					else if (parameters.at(i) == "nsz") attributes.flags.nsz = true;
					else if (parameters.at(i) == "arcp") attributes.flags.arcp = true;
					else if (parameters.at(i) == "contract") attributes.flags.contract = true;
					else if (parameters.at(i) == "afn") attributes.flags.afn = true;
					else if (parameters.at(i) == "reassoc") attributes.flags.reassoc = true;
					else if (parameters.at(i) == "fast") attributes.flags.fast = true;
				}
				break;
			}
			case IR_Sub: {
				// <result> = sub <ty> <op1>, <op2>          ; yields {ty}:result
				// <result> = sub nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = sub nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = sub nuw nsw <ty> <op1>, <op2>; yields{ ty }:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "nsw") attributes.flags.nsw = true;
					if (parameters[i] == "nuw") attributes.flags.nuw = true;
				}
				break;
			}
			case IR_FSub: {
				// <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters.at(i) == "nnan") attributes.flags.nnan = true;
					else if (parameters.at(i) == "ninf") attributes.flags.ninf = true;
					else if (parameters.at(i) == "nsz") attributes.flags.nsz = true;
					else if (parameters.at(i) == "arcp") attributes.flags.arcp = true;
					else if (parameters.at(i) == "contract") attributes.flags.contract = true;
					else if (parameters.at(i) == "afn") attributes.flags.afn = true;
					else if (parameters.at(i) == "reassoc") attributes.flags.reassoc = true;
					else if (parameters.at(i) == "fast") attributes.flags.fast = true;
				}
				break;
			}
			case IR_Mul: {
				// <result> = mul <ty> <op1>, <op2>          ; yields {ty}:result
				// <result> = mul nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = mul nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = mul nuw nsw <ty> <op1>, <op2>; yields{ ty }:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "nsw") attributes.flags.nsw = true;
					if (parameters[i] == "nuw") attributes.flags.nuw = true;
				}
				break;
			}
			case IR_FMul: {
				// <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters.at(i) == "nnan") attributes.flags.nnan = true;
					else if (parameters.at(i) == "ninf") attributes.flags.ninf = true;
					else if (parameters.at(i) == "nsz") attributes.flags.nsz = true;
					else if (parameters.at(i) == "arcp") attributes.flags.arcp = true;
					else if (parameters.at(i) == "contract") attributes.flags.contract = true;
					else if (parameters.at(i) == "afn") attributes.flags.afn = true;
					else if (parameters.at(i) == "reassoc") attributes.flags.reassoc = true;
					else if (parameters.at(i) == "fast") attributes.flags.fast = true;
				}
				break;
			}
			case IR_UDiv: {
				// <result> = udiv <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = udiv exact <ty> <op1>, <op2>; yields{ ty }:result
				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "exact") attributes.flags.exact = true;
				}
				break;
			}
			case IR_SDiv: {
				// <result> = sdiv <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = sdiv exact <ty> <op1>, <op2>; yields{ ty }:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "exact") attributes.flags.exact = true;
				}
				break;
			}
			case IR_FDiv: {
				// <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters.at(i) == "nnan") attributes.flags.nnan = true;
					else if (parameters.at(i) == "ninf") attributes.flags.ninf = true;
					else if (parameters.at(i) == "nsz") attributes.flags.nsz = true;
					else if (parameters.at(i) == "arcp") attributes.flags.arcp = true;
					else if (parameters.at(i) == "contract") attributes.flags.contract = true;
					else if (parameters.at(i) == "afn") attributes.flags.afn = true;
					else if (parameters.at(i) == "reassoc") attributes.flags.reassoc = true;
					else if (parameters.at(i) == "fast") attributes.flags.fast = true;
				}
				break;
			}
			case IR_URem: {
				// <result> = urem <ty> <op1>, <op2>   ; yields {ty}:result

					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i = RESET;

				break;
			}
			case IR_SRem: {
				// <result> = srem <ty> <op1>, <op2>   ; yields {ty}:result

				attributes.params.operand2 = parameters[i];
				attributes.params.operand1 = parameters[i - 1];
				attributes.params.dataType = parameters[i - 2];
				i = RESET;

			}
			case IR_FRem: {
				// <result> = frem [fast-math flags]* <ty> <op1>, <op2>   ; yields {ty}:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters.at(i) == "nnan") attributes.flags.nnan = true;
					else if (parameters.at(i) == "ninf") attributes.flags.ninf = true;
					else if (parameters.at(i) == "nsz") attributes.flags.nsz = true;
					else if (parameters.at(i) == "arcp") attributes.flags.arcp = true;
					else if (parameters.at(i) == "contract") attributes.flags.contract = true;
					else if (parameters.at(i) == "afn") attributes.flags.afn = true;
					else if (parameters.at(i) == "reassoc") attributes.flags.reassoc = true;
					else if (parameters.at(i) == "fast") attributes.flags.fast = true;
				}
				break;
			}
			case IR_Shl: {
				// <result> = shl <ty> <op1>, <op2>           ; yields {ty}:result
				// <result> = shl nuw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = shl nsw <ty> <op1>, <op2>; yields{ ty }:result
				// <result> = shl nuw nsw <ty> <op1>, <op2>; yields{ ty }:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "nsw") attributes.flags.nsw = true;
					if (parameters[i] == "nuw") attributes.flags.nuw = true;
				}
				break;
			}
			case IR_LShr: {
				// <result> = lshr <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = lshr exact <ty> <op1>, <op2>; yields{ ty }:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "exact") attributes.flags.exact = true;
				}
				break;
			}
			case IR_AShr: {
				// <result> = ashr <ty> <op1>, <op2>         ; yields {ty}:result
				// <result> = ashr exact <ty> <op1>, <op2>; yields{ ty }:result

				if (i == parameters.size() - 1) {
					attributes.params.operand2 = parameters[i];
					attributes.params.operand1 = parameters[i - 1];
					attributes.params.dataType = parameters[i - 2];
					i -= 2;
				}
				else {
					if (parameters[i] == "exact") attributes.flags.exact = true;
				}
				break;
			}
			case IR_And: {
				// <result> = and <ty> <op1>, <op2>   ; yields {ty}:result

				attributes.params.operand2 = parameters[i];
				attributes.params.operand1 = parameters[i - 1];
				attributes.params.dataType = parameters[i - 2];
				i = RESET;
				break;
			}
			case IR_Or: {
				// <result> = or <ty> <op1>, <op2>   ; yields {ty}:result

				attributes.params.operand2 = parameters[i];
				attributes.params.operand1 = parameters[i - 1];
				attributes.params.dataType = parameters[i - 2];
				i = RESET;
				break;
			}
			case IR_Xor: {
				// <result> = xor <ty> <op1>, <op2>   ; yields {ty}:result

				attributes.params.operand2 = parameters[i];
				attributes.params.operand1 = parameters[i - 1];
				attributes.params.dataType = parameters[i - 2];
				i = RESET;
				break;
			}

			case IR_Alloca: {
				// <result> = alloca <type>[, <ty> <NumElements>][, align <alignment>]     ; yields {type*}:result

				attributes.params.dataType = parameters[0];

				if (parameters[1] == "align") {
					attributes.params.align = parameters[2];
				}
				else if (parameters.size() == 5) {
					attributes.params.type2 = parameters[1];
					attributes.params.numElements = parameters[2];
					attributes.params.align = parameters[4];
				}
				else {
					attributes.params.type2 = parameters[1];
					attributes.params.numElements = parameters[2];
				}
				i = RESET;
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
			case IR_ICmp: {
				// <result> = icmp <cond> <ty> <op1>, <op2>   ; yields {i1} or {<N x i1>}:result

				attributes.params.operand2 = parameters[i];
				attributes.params.operand1 = parameters[i - 1];
				attributes.params.dataType = parameters[i - 2];
				attributes.condition.cond = parameters[i - 3];

				if (parameters[i - 3] == "eq") attributes.condition.eq = true;
				else if (parameters[i - 3] == "ne") attributes.condition.ne = true;
				else if (parameters[i - 3] == "ugt") attributes.condition.ugt = true;
				else if (parameters[i - 3] == "uge") attributes.condition.uge = true;
				else if (parameters[i - 3] == "ult") attributes.condition.ult = true;
				else if (parameters[i - 3] == "ule") attributes.condition.ule = true;
				else if (parameters[i - 3] == "sgt") attributes.condition.sgt = true;
				else if (parameters[i - 3] == "sge") attributes.condition.sge = true;
				else if (parameters[i - 3] == "slt") attributes.condition.slt = true;
				else if (parameters[i - 3] == "sle") attributes.condition.sle = true;

				break;
			}
			case IR_FCmp: {
				// <result> = fcmp <cond> <ty> <op1>, <op2>     ; yields {i1} or {<N x i1>}:result

				attributes.params.operand2 = parameters[i];
				attributes.params.operand1 = parameters[i - 1];
				attributes.params.dataType = parameters[i - 2];
				attributes.condition.cond = parameters[i - 3];


				if (parameters[i - 3] == "false") attributes.condition.condFalse = true;
				else if (parameters[i - 3] == "oeq") attributes.condition.oeq = true;
				else if (parameters[i - 3] == "ogt") attributes.condition.ogt = true;
				else if (parameters[i - 3] == "oge") attributes.condition.oge = true;
				else if (parameters[i - 3] == "olt") attributes.condition.olt = true;
				else if (parameters[i - 3] == "ole") attributes.condition.ole = true;
				else if (parameters[i - 3] == "one") attributes.condition.one = true;
				else if (parameters[i - 3] == "ord") attributes.condition.ord = true;
				else if (parameters[i - 3] == "ueq") attributes.condition.ueq = true;
				else if (parameters[i - 3] == "ugt") attributes.condition.ugt = true;
				else if (parameters[i - 3] == "uge") attributes.condition.uge = true;
				else if (parameters[i - 3] == "ult") attributes.condition.ult = true;
				else if (parameters[i - 3] == "ule") attributes.condition.ule = true;
				else if (parameters[i - 3] == "une") attributes.condition.une = true;
				else if (parameters[i - 3] == "uno") attributes.condition.uno = true;
				else if (parameters[i - 3] == "true") attributes.condition.condTrue = true;

				break;
			}
			case IR_PHI: {
				// <result> = phi <ty> [ <val0>, <label0>], ...

				//Improvement Opportunity - Dynamically allocated string to eliminate PHIPATHMAX
				attributes.params.dataType = parameters[0];
				for (int i = 1; i <= PHIPATHMAX; i++) {
					if (!(parameters[i].empty())) {
						attributes.phi.paths[i - 1] = parameters[i];
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


}

void
ComputeNode::compute() {

	switch (s_opMap[opCode]) {
	case IR_Move: {

		break;
	}
	case IR_Ret: {

		break;
	}
	case IR_Br: {

		break;
	}
	case IR_Switch: {

		break;
	}
	case IR_IndirectBr: {

		break;
	}
	case IR_Invoke: {

		break;
	}
	case IR_Resume: {

		break;
	}
	case IR_Unreachable: {

		break;
	}
	case IR_Add: {

		break;
	}
	case IR_FAdd: {

		break;
	}
	case IR_Sub: {

		break;
	}
	case IR_FSub: {

		break;
	}
	case IR_Mul: {

		break;
	}
	case IR_FMul: {

		break;
	}
	case IR_UDiv: {

		break;
	}
	case IR_SDiv: {

		break;
	}
	case IR_FDiv: {

		break;
	}
	case IR_URem: {

		break;
	}
	case IR_SRem: {

		break;
	}
	case IR_FRem: {

		break;
	}
	case IR_Shl: {

		break;
	}
	case IR_LShr: {

		break;
	}
	case IR_AShr: {

		break;
	}
	case IR_And: {

		break;
	}
	case IR_Or: {

		break;
	}
	case IR_Xor: {

		break;
	}

	case IR_Alloca: {

		break;
	}
	case IR_Load: {
		//		    uint8_t* getCurData() { return curData; }
		break;
	}
	case IR_Store: {
		//			comm_interface.hh
		//		    int prepRead(Addr src, size_t length);
		//		    int prepWrite(Addr dst, uint8_t* value, size_t length);

		break;
	}
	case IR_GetElementPtr: {

		break;
	}
	case IR_Fence: {

		break;
	}
	case IR_AtomicCmpXchg: {

		break;
	}
	case IR_AtomicRMW: {

		break;
	}
	case IR_Trunc: {

		break;
	}
	case IR_ZExt: {

		break;
	}
	case IR_SExt: {

		break;
	}
	case IR_FPToUI: {

		break;
	}
	case IR_FPToSI: {

		break;
	}
	case IR_UIToFP: {

		break;
	}
	case IR_SIToFP: {

		break;
	}
	case IR_FPTrunc: {

		break;
	}
	case IR_FPExt: {

		break;
	}
	case IR_PtrToInt: {

		break;
	}
	case IR_IntToPtr: {

		break;
	}
	case IR_BitCast: {

		break;
	}
	case IR_AddrSpaceCast: {

		break;
	}
	case IR_ICmp: {

		break;
	}
	case IR_FCmp: {

		break;
	}
	case IR_PHI: {

		break;
	}
	case IR_Call: {

		break;
	}
	case IR_Select: {

		break;
	}
	case IR_VAArg: {

		break;
	}
	case IR_ExtractElement: {

		break;
	}
	case IR_InsertElement: {

		break;
	}
	case IR_ShuffleVector: {

		break;
	}
	case IR_ExtractValue: {

		break;
	}
	case IR_InsertValue: {

		break;
	}
	case IR_LandingPad: {

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

	default: {

		break;
	}
	}


}

std::string
ComputeNode::computeBranch() {
    return "";
}

void
ComputeNode::checkState() {

}
