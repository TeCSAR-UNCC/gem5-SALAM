#include <iostream>
#include <fstream>
#include "hwacc/llvm_interface.hh"
#include "debug/LLVMInterface.hh"
#include "debug/LLVMParse.hh"
#include "debug/LLVMRegister.hh"

LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    ComputeUnit(p),
    filename(p->in_file),
    numPE(p->proc_elem) {
    bbList = NULL;
    regList = NULL;
    currBB = NULL;
    prevBB = NULL;
    typeList = NULL;
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

//    DPRINTF(LLVMInterface, "*************************************\n");
//    DPRINTF(LLVMInterface, "                Cycle %d             \n", cycle);
//    DPRINTF(LLVMInterface, "*************************************\n");
    DPRINTF(LLVMInterface, "\n%s\n%s %d\n%s\n",
            "*******************************************************************************",
            "   Cycle", cycle,
            "*******************************************************************************");
    cycle++;
    //Check our compute queue to see if any compute nodes are ready to commit
    DPRINTF(LLVMInterface, "Checking Compute Queue for Nodes Ready for Commit\n");
    for (auto it = computeQueue->begin(); it != computeQueue->end(); ) {
        if ((*it)->commit()) {
            Instruction instr = (*it)->getInstruction();
            it = computeQueue->erase(it);
        } else {
            ++it;
        }
    }

    //
    if (reservation->empty()) {
        DPRINTF(LLVMInterface, "Schedule Basic Block\n");
        scheduleBB(currBB);
    }

    for (auto it=reservation->begin(); it!=reservation->end(); ) {
        Instruction instr = (*it)->getInstruction();
        DPRINTF(LLVMInterface, "Next:%s\n", instr.general.llvm_Line);

        if (!(instr.general.terminator) && !((*it)->checkDependency())) {
            DPRINTF(LLVMInterface, "Non-Terminator Instruction Operation \n");
            if (instr.general.opCode.find("load") == 0) {
                DPRINTF(LLVMInterface, "Queueing Load\n");
                readQueue->push_back(*it);
                (*it)->compute();
            } else if (instr.general.opCode.find("store") == 0) {
                DPRINTF(LLVMInterface, "Queueing Store\n");
                writeQueue->push_back(*it);
                (*it)->compute();
            } else if (instr.general.opCode.find("phi") == 0) {
                DPRINTF(LLVMInterface, "Queueing PHI\n");
                (*it)->compute();
                (*it)->commit();
            } else {
                DPRINTF(LLVMInterface, "Queueing Compute\n");
                computeQueue->push_back(*it);
                (*it)->compute();
            }
            it = reservation->erase(it);
        } else if ((instr.general.opCode.compare("br") == 0) && !((*it)->checkDependency())) {
            if(readQueue->empty() && writeQueue->empty() && computeQueue->empty()) {
                DPRINTF(LLVMInterface, "Branch Operation In Progress\n");
                //currBB <- Calculate branch
                prevBB = currBB;
                (*it)->compute();
                instr = (*it)->getInstruction();
                DPRINTF(LLVMInterface, "Branching to Basic Block %s\n", instr.terminator.dest);
                currBB = findBB(instr.terminator.dest);
                it = reservation->erase(it);
                scheduleBB(currBB);
            } else {
                ++it;
            }
        } else if (instr.general.opCode.compare("ret") == 0) {
            if(readQueue->empty() && writeQueue->empty() && computeQueue->empty()) {
                running = false;
                //We are done!!!!
                comm->finish();
                break;
            }
        } else {
            ++it;
        }
    }

    if (running && !tickEvent.scheduled())
    {
        schedule(tickEvent, curTick() + clock_period * process_delay);
    }
}

void
LLVMInterface::scheduleBB(BasicBlock * bb) {
    DPRINTF(LLVMInterface, "Adding BB:%s to reservation table\n", bb->getName());
    for(auto it=bb->cnList->begin(); it!=bb->cnList->end(); ++it) {
        Instruction instr = (*it)->getInstruction();
        //if it is a phi and we don't have an unmet dependency -> commit immediately
        if(instr.general.phi) {
            (*it)->setPrevBB(prevBB->name);
            if (!(*it)->checkDependency()) {
                //DPRINTF(LLVMInterface, "Dependency Found\n");
                DPRINTF(LLVMInterface, "Compute\n");
                (*it)->compute();
                DPRINTF(LLVMInterface, "Commit\n");
                (*it)->commit();
            } else {
                //DPRINTF(LLVMInterface, "No Dependency Found\n");
                DPRINTF(LLVMInterface, "Reset\n");
                (*it)->reset();
                DPRINTF(LLVMInterface, "Reserve\n");
                reservation->push_back(*it);
            } 
        DPRINTF(LLVMInterface, "Phi Completed\n");
        //else if it is an unconditional branch -> evaluate immediately
        } else if (instr.general.terminator && instr.terminator.unconditional) {
            //currBB <- Calculate branch
            prevBB = currBB;
            (*it)->compute();
            instr = (*it)->getInstruction();
            currBB = findBB(instr.terminator.dest);
            assert(currBB);
            scheduleBB(currBB);
        } else {
            (*it)->reset();
            reservation->push_back(*it);
        }
    }
}

void
LLVMInterface::constructBBList() {
    DPRINTF(LLVMInterface, "Constructing Dependency Graph\n");
    bbList = new std::list<BasicBlock*>();
    regList = new RegisterList();
    typeList = new TypeList();
    std::ifstream llvmFile(filename, std::ifstream::in);
    std::string line;
    bool inFunction = false;
    unsigned bbnum = 0;
    DPRINTF(LLVMInterface, "Parsing %s\n", filename);
    if(llvmFile.is_open()) {
        while (getline(llvmFile, line)) {
            DPRINTF(LLVMParse, "Line: %s\n", line);
            if (!inFunction) {
                if (!line.find("%struct")) { // Found defined data type.
                    int pos = line.find('=');
                    int size = 1;
                    std::string name = line.substr(1,pos-2);
                    for(int i = pos; i < line.size(); i++) {
                        if(line[i] == ',') size++;
                    }
                    typeList->addType(new LLVMType(size, name));
                   // DPRINTF(LLVMParse, "Found new data type. %s of size %i\n", name, size);
                } else if (!line.find("define")) { //Found a function. Need to parse its header
                    DPRINTF(LLVMParse, "Found acc function. Parsing Global Vars\n");
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
                            DPRINTF(LLVMParse, "Creating register for: %s\n", regName);
                            regList->addRegister(new Register(regName, comm->getGlobalVar(paramNum)));
                            DPRINTF(LLVMParse, "Initial Value: %X\n", (regList->findRegister(regName))->getValue());
                            paramNum++;
                        }
                        linePos = percPos + 1;
                        percPos = line.find("%", linePos);
                    }
                    currBB = new BasicBlock("0", bbnum);
                    DPRINTF(LLVMParse, "Found Basic Block: %s\n", currBB->name);
                    bbnum++;
                    bbList->push_back(currBB);
                }
            } else {
                if (line.find("\n") > 0) {
                    if (line.find("; <label>:") == 0) {
                        int labelEnd = line.find(" ", 10);
                        prevBB = currBB;
                        currBB = new BasicBlock(line.substr(10,(labelEnd - 10)), bbnum);
                        DPRINTF(LLVMParse, "Found Basic Block: %s\n", currBB->name);
                        bbnum++;
                        bbList->push_back(currBB);
                    } else if (line.find(".") == 0) {
                        int labelEnd = line.find(" ");
                        prevBB = currBB;
                        currBB = new BasicBlock(line.substr(0,(labelEnd-1)), bbnum);
                        DPRINTF(LLVMParse, "Found Basic Block: %s\n", currBB->name);
                        bbnum++;
                        bbList->push_back(currBB);
                    } else if (line.find("}") == 0) {
                        inFunction = false;
                        DPRINTF(LLVMParse, "Finished File Parsing\n");
                        break;
                    } else if (!(line.find_first_not_of(' ') != std::string::npos)){
                        DPRINTF(LLVMParse, "Empty Line\n");
                    } else {
                        DPRINTF(LLVMParse, "Registering Compute Node for: %s\n", line);
                        /////////////////////////////////////////////////
                        if ((line.find("switch") != -1)) {
                            DPRINTF(LLVMParse, "Found switch statement, converting to inline\n");
                            std::string concatLine;
                            while(line.find(" ]") == -1) {
                                DPRINTF(LLVMParse, " %s +\n", line);
                                concatLine += line;
                                getline(llvmFile, line);
                            }
                        concatLine+= ']';
                        line = concatLine;
                        DPRINTF(LLVMParse, "New Line: %s\n", line);
                        }
                        /////////////////////////////////////////////////
                        if(prevBB) {
                            currBB->addNode(new ComputeNode(line, regList, prevBB->getName(), comm, typeList));
                        } else {
                            currBB->addNode(new ComputeNode(line, regList, "NULL", comm, typeList));
                        }
                    }
                }
            }
        }
        currBB = findBB("0");
    } else {
        panic("Unable to open LLVM file\n");
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
LLVMInterface::readCommit(MemoryRequest * req) {
    for (auto it=readQueue->begin(); it!=readQueue->end(); ++it) {
        if ((*it)->getReq() == req) {
            Instruction instr = (*it)->getInstruction();
            instr.general.returnRegister->setValue(req->buffer);
            instr.general.returnRegister->commit();
            DPRINTF(LLVMInterface, "Read Operation Complete\n%s\n", instr.general.llvm_Line);
            DPRINTF(LLVMRegister, "Commit %s\n", instr.general.returnRegister->getName());
            it = readQueue->erase(it);
        }
    }
}

void
LLVMInterface::writeCommit(MemoryRequest * req) {
    for (auto it=writeQueue->begin(); it!=writeQueue->end(); ++it) {
        if ((*it)->getReq() == req) {
            Instruction instr = (*it)->getInstruction();
            DPRINTF(LLVMInterface, "Store Operation Complete\n%s\n", instr.general.llvm_Line);
            it = writeQueue->erase(it);
        }
    }
}

void
LLVMInterface::initialize() {
    DPRINTF(LLVMInterface, "Initializing LLVM Runtime Engine\n");
    running = true;
    if (!bbList)
        constructBBList();
    //if(!reservation) {
        DPRINTF(LLVMInterface, "Initializing reservation list\n");
        reservation = new std::list<ComputeNode*>();
    //}
    //if (!readQueue) {
        DPRINTF(LLVMInterface, "Initializing readQueue queue\n");
        readQueue = new std::list<ComputeNode*>();
    //}
    //if (!writeQueue) {
        DPRINTF(LLVMInterface, "Initializing writeQueue queue\n");
        writeQueue = new std::list<ComputeNode*>();
    //}
    //if (!computeQueue) {
        DPRINTF(LLVMInterface, "Initializing computeQueue list\n");
        computeQueue = new std::list<ComputeNode*>();
    //}
//    DPRINTF(LLVMInterface, "*************************************\n");
//    DPRINTF(LLVMInterface, "*         Beginning Compute         *\n");
//    DPRINTF(LLVMInterface, "*************************************\n");
    DPRINTF(LLVMInterface, "\n%s\n%s\n%s\n",
            "*******************************************************************************",
            "*                                 Begin Compute                               *",
            "*******************************************************************************");
    cycle = 0;
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
