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
/*********************************************************************************************
 CN Scheduling

 Reservation Table:
 | CN* | ReturnReg | DepList | In-Flight (T/F) |

 As CNs are scheduled they are added to an in-flight queue depending on operation type.
 Loads and Stores are maintained in separate queues, and are committed by the comm_interface.
 Branch and phi instructions evaluate and commit immediately. All other CN types are added to
 an in-flight compute queue.

 Each tick we must first check our in-flight compute queue. Each node should have its cycle
 count incremented, and should commit if max cycle is reached.

 After queues are evaluated we must check our reservation table. If all dependencies of a CN
 are satisfied we mark it as in-flight, evaluate it, and add it to its respective queue if
 applicable. Upon commit the CN will be removed both from its queue and the reservation table.

 New CNs are added to the reservation table whenever a new BB is encountered. This may occur
 during device init, or when a br op commits. For each CN in a BB we reset the CN, evaluate
 if it is a phi or uncond br, and add it to our reservation table otherwise.
*********************************************************************************************/

    if (!tickEvent.scheduled())
    {
        schedule(tickEvent, curTick() + clock_period * process_delay);
    }
}

void
LLVMInterface::constructBBList() {
    bbList = new std::list<BasicBlock*>();
    //bbList = new BasicBlockList();
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
                bbList->push_back(currBB);
                //bbList->addBasicBlock(currBB);
            }
        } else {
            if (line.find("\n") > 0) {
                if (line.find("; <label>:") == 0) {
                    int labelEnd = line.find(" ", 10);
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(10,(labelEnd - 10)), bbnum);
                    bbnum++;
                    bbList->push_back(currBB);
                    //bbList->addBasicBlock(currBB);
                } else if (line.find(".") == 0) {
                    int labelEnd = line.find(" ");
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(1,(labelEnd - 1)), bbnum);
                    bbnum++;
                    bbList->push_back(currBB);
                    //bbList->addBasicBlock(currBB);
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
    if(!bbList)
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
