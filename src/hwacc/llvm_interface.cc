#include <iostream>
#include <fstream>
#include "hwacc/llvm_interface.hh"
#include "debug/LLVMInterface.hh"

LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    ComputeUnit(p),
    filename(p->in_file),
    numPE(p->proc_elem) {
    bbList = NULL;
    regList = NULL;
    currBB = NULL;
    prevBB = NULL;
    currCompNode = NULL;
    running = false;
    clock_period = comm->getProcessDelay(); //Clock period
    process_delay = 1; //Number of cycles a compute_node needs to complete
}

void
LLVMInterface::tick() {
    if (comm->isCompNeeded() && !running) {
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
        if (!tickEvent.scheduled())
        {
            schedule(tickEvent, curTick() + clock_period * process_delay);
        }
    } else if (running) {
        if (!comm->isRunning()) { //If comm isn't running we aren't in a memory op
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
                //There is no need to schedule another event
            }
        }
        if (!tickEvent.scheduled())
        {
            schedule(tickEvent, curTick() + clock_period * process_delay);
        }
    }
    if (!tickEvent.scheduled())
    {
        schedule(tickEvent, comm->nextCycle());
    }
}

void
LLVMInterface::constructBBList() {
    bbList = new BasicBlockList();
    regList = new RegisterList();
    std::ifstream llvmFile(filename, std::ifstream::in);
    std::string line;
    bool inFunction = false;
    unsigned bbnum = 0;

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
                        regList->addRegister(new Register(regName, comm->getGlobalVar(paramNum)));
                        paramNum++;
                    }
                    linePos = percPos + 1;
                    percPos = line.find("%", linePos);
                }
                currBB = new BasicBlock("0", bbnum);
                bbnum++;
                bbList->addBasicBlock(currBB);
            }
        } else {
            if (line.find("\n") > 0) {
                if (line.find("; <label>:") == 0) {
                    int labelEnd = line.find(" ", 10);
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(10,(labelEnd - 10)), bbnum);
                    bbnum++;
                    bbList->addBasicBlock(currBB);
                } else if (line.find(".") == 0) {
                    int labelEnd = line.find(" ");
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(1,(labelEnd - 1)), bbnum);
                    bbnum++;
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
LLVMInterface::copyToBuffer(uint8_t *data, unsigned size) {
    assert(size > 0);
    dataBuffer = new uint8_t[size];
    for (int i = 0; i < size; i++) {
        dataBuffer[i] = *(data + i);
    }
}

void
LLVMInterface::initialize() {
    constructBBList();
    tick();
}

void
LLVMInterface::startup() {
    comm->registerCompUnit(this);
}

LLVMInterface*
LLVMInterfaceParams::create() {
    return new LLVMInterface(this);
}
