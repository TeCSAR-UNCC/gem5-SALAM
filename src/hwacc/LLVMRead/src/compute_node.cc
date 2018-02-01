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
    	for(int i = 0; i < parameters.size(); i++){
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

    	}




	switch (s_opMap[opCode]) {

	// Terminator Instructions
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
	// End Terminator Instructions

	// Binary Operations - Integer
		case IR_Add: {

			break;
		}

		case IR_Sub: {

			break;
		}

		case IR_Mul: {

			break;
		}

		case IR_UDiv: {

			break;
		}
		case IR_SDiv: {

			break;
		}
		case IR_URem: {

			break;
		}
		case IR_SRem: {

			break;
		}
	// End Binary Operations - Integer
	// Binary Operations - Floating Point
		case IR_FAdd: {

			break;
		}
		case IR_FSub: {

			break;
		}
		case IR_FMul: {

			break;
		}
		case IR_FDiv: {

			break;
		}
		case IR_FRem: {

			break;
		}
	// End Binary Operations - Floating Point
	// Bitwise Binary Operations
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
	// End Bitwise Binary Operations
	// Memory Access and Addressing Operations
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
	// End Memory Access and Addressing Operations
	// Conversion Operations
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
	// End Converstion Operations
	// Other Operations
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
		case IR_LandingPad: {

			break;
		}
	// End Other Operations
	// Vector Operations
		case IR_ExtractElement: {

			break;
		}
		case IR_InsertElement: {

			break;
		}
		case IR_ShuffleVector: {

			break;
		}
	// End Vector Operations
	// Aggregate Operations
		case IR_ExtractValue: {

			break;
		}
		case IR_InsertValue: {

			break;
		}
	// End Aggregate Operations
	// Custom Operations
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
	// End Custom Operations
	// Default
		default: {

			break;
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
