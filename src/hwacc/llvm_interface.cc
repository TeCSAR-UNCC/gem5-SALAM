#include "hwacc/llvm_interface.hh"

LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    SimObject(p),
    acc(p->dummy_acc) {
    bbList = NULL;
    currBB = NULL;
    currCompNode = NULL;
}

void
tick() {

}

LLVMInterface*
LLVMInterfaceParams::create() {
    return new LLVMInterface(this);
}
