#include <iostream>
#include <fstream>
#include "hwacc/llvm_interface.hh"

LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    SimObject(p),
    acc(p->dummy_acc),
    filename(p->in_file),
    tickEvent(this) {
    bbList = NULL;
    regList = NULL;
    currBB = NULL;
    prevBB = NULL;
    currCompNode = NULL;
    running = false;
    clock_delay = acc->getProcessDelay(); //Clock period
    process_delay = 1; //Number of cycles a compute_node needs to complete
}

void
LLVMInterface::tick() {
    if (acc->isCompNeeded() && !running) {
        LLVMInterface::constructBBList();
        currBB = bbList->findBasicBlock("0");
        prevBB = NULL;
        currCompNode = currBB->getStart();
        if(currCompNode->isBranch()) {
            prevBB = currBB;
            BasicBlock *branchTarget = bbList->findBasicBlock(currCompNode->computeBranch());
            assert(branchTarget);
            currBB = branchTarget;
            currCompNode = currBB->getStart();
        }
    } else if (running) {
        if (!acc->isRunning()) { //If acc isn't running we aren't in a memory op
            if(currCompNode->isBranch()) {
                prevBB = currBB;
                BasicBlock *branchTarget = bbList->findBasicBlock(currCompNode->computeBranch());
                assert(branchTarget);
                currBB = branchTarget;
                currCompNode = currBB->getStart();
            }else if (currCompNode != currBB->getEnd()) {
                currCompNode->compute();
                currCompNode = currBB->step();
            } else {
                currCompNode->compute();
                //We are finished execution. Raise interrupts!
            }
        }
    }
    if (!tickEvent.scheduled())
    {
        schedule(tickEvent, curTick() + clock_delay * process_delay);
    }
}

void
LLVMInterface::constructBBList() {
    bbList = new BasicBlockList();
    regList = new RegisterList();
    std::ifstream llvmFile(filename, std::ifstream::in);
    std::string line;
    bool inFunction = false;

    while (llvmFile) {
        getline(llvmFile, line);
        if (!inFunction) {
            if (line.find("define")) { //Found a function. Need to parse its header
                inFunction = true;
                unsigned paramNum = 0;
                unsigned linePos = 0;
                int percPos = line.find("%"); //All registers preceeded by a % in LLVM
                int commaPos;
                while (percPos > -1) {
                    if (line.find("%struct", linePos) != percPos) { //Ensure we didn't just find a struct type
                        percPos++;
                        commaPos = line.find(",", percPos);
                        if (commaPos < 0) commaPos = line.find(")");
                        std::string regName = line.substr(percPos, (commaPos-percPos));
                        regList->addRegister(new Register(regName, acc->getMMRData(paramNum)));
                        paramNum++;
                    }
                    linePos = percPos + 1;
                    percPos = line.find("%", linePos);
                }
                currBB = new BasicBlock("0");
                bbList->addBasicBlock(currBB);
            }
        } else {
            if (line.find("\n") > 0) {
                if (line.find("; <label>:") == 0) {
                    int labelEnd = line.find(" ", 10);
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(10,(labelEnd - 10)));
                    bbList->addBasicBlock(currBB);
                } else if (line.find(".") == 0) {
                    int labelEnd = line.find(" ");
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(1,(labelEnd - 1)));
                    bbList->addBasicBlock(currBB);
                } else if (line.find("}") == 0) {
                    inFunction = false;
                } else {
                    currBB->addNode(new ComputeNode(line, regList, prevBB->getName()));
                }
            }
        }
    }

}

void
LLVMInterface::startup() {
    schedule(tickEvent, acc->nextCycle());
}

LLVMInterface*
LLVMInterfaceParams::create() {
    return new LLVMInterface(this);
}
