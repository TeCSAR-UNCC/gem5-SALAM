#include "compute_node.hh"
#include "operations.hh"

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
    std::string temp = line.substr(returnChk+3);
    op = temp.substr(0,temp.find(" "));
    prevBB = prev;
    params = temp.substr(temp.find(" ")+1);
}

void
ComputeNode::compute() {

}

std::string
ComputeNode::computeBranch() {
    return Operations::llvm_br(params);
}

void
ComputeNode::checkState() {

}
