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

    //Check our compute queue to see if any compute nodes are ready to commit
    for (auto it=computeQueue->begin(); it!=computeQueue->end(); ++it) {
        while ((*it)->commit()) {
            computeQueue->erase(it);
        }
    }

    //
    if (reservation->empty()) {
        scheduleBB(currBB);
    }

    for (auto it=reservation->begin(); it!=reservation->end(); ++it) {
        Instruction instr = (*it)->getInstruction();
        while (!(instr.general.terminator) && ((*it)->checkDependency())) {
            instr = (*it)->getInstruction();
            if (instr.general.opCode.compare("load") == 0) {
                readQueue->push(*it);
                (*it)->compute();
            } else if (instr.general.opCode.compare("store") == 0) {
                writeQueue->push(*it);
                (*it)->compute();
            } else if (instr.general.opCode.compare("phi") == 0) {
                (*it)->compute();
                (*it)->commit();
            } else {
                computeQueue->push_back(*it);
                (*it)->compute();
            }
            reservation->erase(it);
        }
        if ((instr.general.opCode.compare("br") == 0) && ((*it)->checkDependency())) {
            if(readQueue->empty() && writeQueue->empty() && computeQueue->empty()) {
                //currBB <- Calculate branch
                prevBB = currBB;
                (*it)->compute();
                currBB = findBB(instr.terminator.dest);
                reservation->erase(it);
                scheduleBB(currBB);
            }
        }
        if (instr.general.opCode.compare("ret") == 0) {
            if(readQueue->empty() && writeQueue->empty() && computeQueue->empty()) {
                running = false;
                //We are done!!!!
                comm->finish();
            }
        }
    }

    if (running && !tickEvent.scheduled())
    {
        schedule(tickEvent, curTick() + clock_period * process_delay);
    }
}

void
LLVMInterface::scheduleBB(BasicBlock * bb) {
    for(auto it=bb->cnList->begin(); it!=bb->cnList->end(); ++it) {
        Instruction instr = (*it)->getInstruction();
        //if it is a phi and we don't have an unmet dependency -> commit immediately
        if(instr.general.phi) {
            (*it)->setPrevBB(prevBB->name);
            if((*it)->checkDependency()) {
                (*it)->compute();
                (*it)->commit();
            } else {
                (*it)->reset();
                reservation->push_back(*it);
            }
        //else if it is an unconditional branch -> evaluate immediately
        } else if (instr.general.terminator && instr.terminator.unconditional) {
            //currBB <- Calculate branch
            prevBB = currBB;
            (*it)->compute();
            currBB = findBB(instr.terminator.dest);
            scheduleBB(currBB);
        } else {
            (*it)->reset();
            reservation->push_back(*it);
        }
    }
}

void
LLVMInterface::constructBBList() {
    bbList = new std::list<BasicBlock*>();
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
            }
        } else {
            if (line.find("\n") > 0) {
                if (line.find("; <label>:") == 0) {
                    int labelEnd = line.find(" ", 10);
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(10,(labelEnd - 10)), bbnum);
                    bbnum++;
                    bbList->push_back(currBB);
                } else if (line.find(".") == 0) {
                    int labelEnd = line.find(" ");
                    prevBB = currBB;
                    currBB = new BasicBlock(line.substr(1,(labelEnd - 1)), bbnum);
                    bbnum++;
                    bbList->push_back(currBB);
                } else if (line.find("}") == 0) {
                    inFunction = false;
                } else {
                    currBB->addNode(new ComputeNode(line, regList, prevBB->getName()));
                }
            }
        }
    }
}

BasicBlock*
LLVMInterface::findBB(std::string bbname) {
    for (auto it = bbList->begin(); it != bbList->end(); ++it) {
        if ((*it)->name.compare(bbname) == 0)
            return (*it);
    }
    return NULL;
}

void
LLVMInterface::readCommit(uint8_t * data) {
    Instruction instr = readQueue->front()->getInstruction();
    instr.general.returnRegister->setValue(data);
    instr.general.returnRegister->commit();
    readQueue->pop();
}

void
LLVMInterface::initialize() {
    running = true;
    if (!bbList)
        constructBBList();
    if(!reservation)
        reservation = new std::list<ComputeNode*>();
    if (!readQueue)
        readQueue = new std::queue<ComputeNode*>();
    if (!writeQueue)
        writeQueue = new std::queue<ComputeNode*>();
    if (!computeQueue)
        computeQueue = new std::list<ComputeNode*>();
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
