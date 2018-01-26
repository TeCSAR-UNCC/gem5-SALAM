#include "compute_node.hh"

ComputeNode::ComputeNode(std::string line, RegisterList *list, std::string prev) {	
    llvm_inst = line;
    int returnChk = line.find(" = ");

    if (returnChk > 0) {
        returnRegister = new Register(line.substr((line.find("%")+1), returnChk));
        list->addRegister(returnRegister);
    } else {
        returnRegister = NULL;
    }
    finished = 0x0;
    prevBB = prev;
    std::string temp = line.substr(returnChk+3);
	std::string buffer;
	std::stringstream buffer_stream(temp.substr(temp.find(" ") + 1));
    opCode = temp.substr(0,temp.find(" "));
    
    //Logic to look for arrays and/or non-conforming instructions to skip the
    //while loop below
    
	while (buffer_stream >> buffer) {
		if (buffer[(buffer.length() - 2)] == ',') buffer = buffer.substr(0, (buffer.length() - 3));
		decode.push_back(buffer);
	}

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
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nuw") attributes.keywords.nuw = true;
				else if (decode.at(i) == "nsw") attributes.keywords.nuw = true;
			}
			break;
		}
		case IR_FAdd: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nnan") attributes.flags.nnan = true;
				else if (decode.at(i) == "ninf") attributes.flags.ninf = true;
				else if (decode.at(i) == "nsz") attributes.flags.nsz = true;
				else if (decode.at(i) == "arcp") attributes.flags.arcp = true;
				else if (decode.at(i) == "contract") attributes.flags.contract = true;
				else if (decode.at(i) == "afn") attributes.flags.afn = true;
				else if (decode.at(i) == "reassoc") attributes.flags.reassoc = true;
				else if (decode.at(i) == "fast") attributes.flags.fast = true;
			}
			break;
		}
		case IR_Sub: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nuw") attributes.keywords.nuw = true;
				else if (decode.at(i) == "nsw") attributes.keywords.nuw = true;
			}
			break;
		}
		case IR_FSub: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nnan") attributes.flags.nnan = true;
				else if (decode.at(i) == "ninf") attributes.flags.ninf = true;
				else if (decode.at(i) == "nsz") attributes.flags.nsz = true;
				else if (decode.at(i) == "arcp") attributes.flags.arcp = true;
				else if (decode.at(i) == "contract") attributes.flags.contract = true;
				else if (decode.at(i) == "afn") attributes.flags.afn = true;
				else if (decode.at(i) == "reassoc") attributes.flags.reassoc = true;
				else if (decode.at(i) == "fast") attributes.flags.fast = true;
			}
			break;
		}
		case IR_Mul: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nuw") attributes.keywords.nuw = true;
				else if (decode.at(i) == "nsw") attributes.keywords.nuw = true;
			}
			break;
		}
		case IR_FMul: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nnan") attributes.flags.nnan = true;
				else if (decode.at(i) == "ninf") attributes.flags.ninf = true;
				else if (decode.at(i) == "nsz") attributes.flags.nsz = true;
				else if (decode.at(i) == "arcp") attributes.flags.arcp = true;
				else if (decode.at(i) == "contract") attributes.flags.contract = true;
				else if (decode.at(i) == "afn") attributes.flags.afn = true;
				else if (decode.at(i) == "reassoc") attributes.flags.reassoc = true;
				else if (decode.at(i) == "fast") attributes.flags.fast = true;
			}
			break;
		}
		case IR_UDiv: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "exact") attributes.keywords.exact = true;
			}
			break;
		}
		case IR_SDiv: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "exact") attributes.keywords.exact = true;
			}
			break;
		}
		case IR_FDiv: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nnan") attributes.flags.nnan = true;
				else if (decode.at(i) == "ninf") attributes.flags.ninf = true;
				else if (decode.at(i) == "nsz") attributes.flags.nsz = true;
				else if (decode.at(i) == "arcp") attributes.flags.arcp = true;
				else if (decode.at(i) == "contract") attributes.flags.contract = true;
				else if (decode.at(i) == "afn") attributes.flags.afn = true;
				else if (decode.at(i) == "reassoc") attributes.flags.reassoc = true;
				else if (decode.at(i) == "fast") attributes.flags.fast = true;
			}
			break;
		}
		case IR_URem: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			break;
		}
		case IR_SRem: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			break;
		}
		case IR_FRem: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nnan") attributes.flags.nnan = true;
				else if (decode.at(i) == "ninf") attributes.flags.ninf = true;
				else if (decode.at(i) == "nsz") attributes.flags.nsz = true;
				else if (decode.at(i) == "arcp") attributes.flags.arcp = true;
				else if (decode.at(i) == "contract") attributes.flags.contract = true;
				else if (decode.at(i) == "afn") attributes.flags.afn = true;
				else if (decode.at(i) == "reassoc") attributes.flags.reassoc = true;
				else if (decode.at(i) == "fast") attributes.flags.fast = true;
			}
			break;
		}
		case IR_Shl: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nuw") attributes.keywords.nuw = true;
				else if (decode.at(i) == "nsw") attributes.keywords.nuw = true;
			}
			break;
		}
		case IR_LShr: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nuw") attributes.keywords.exact = true;
			}
			break;
		}
		case IR_AShr: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			for (int i = 0; i < decode.size() - 3; i++) {
				if (decode.at(i) == "nuw") attributes.keywords.exact = true;
			}
			break;
		}
		case IR_And: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			break;
		}
		case IR_Or: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			break;
		}
		case IR_Xor: {
			dataType = decode.at(decode.size() - 3);
			operand1 = decode.at(decode.size() - 2);
			operand2 = decode.at(decode.size() - 1);
			break;
		}
		case IR_Alloca: {

			break;
		}
		case IR_Load: {

			break;
		}
		case IR_Store: {

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

void
ComputeNode::compute() {

}

std::string
ComputeNode::computeBranch() {
    return "";
}

void
ComputeNode::checkState() {

}
