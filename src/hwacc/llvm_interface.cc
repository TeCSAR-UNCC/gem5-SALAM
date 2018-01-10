#include "hwacc/llvm_interface.hh"

LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    SimObject(p),
    acc(p->dummy_acc),
    tickEvent(this) {
    bbList = NULL;
    currBB = NULL;
    prevBB = NULL;
    currCompNode = NULL;
    running = false;
}

void
LLVMInterface::tick() {
    if (acc->isCompNeeded() && !running) {
        //Build BBList and start running
    } else if (running) {
        if (!acc->isRunning()) { //If acc isn't running we aren't in a memory op
            if (currCompNode != currBB->getEnd()) {
                currCompNode = currBB->step();
            } else {
            
            }
        }
    }
    if (!tickEvent.scheduled())
    {
        schedule(tickEvent, acc->nextCycle());
    }
}

LLVMInterface*
LLVMInterfaceParams::create() {
    return new LLVMInterface(this);
}
