#include <iostream>
#include <fstream>
#include "hwacc/llvm_interface.hh"


LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    ComputeUnit(p),
    filename(p->in_file),
    scheduling_threshold(p->sched_threshold) {
    bbList = NULL;
    regList = NULL;
    currBB = NULL;
    prevBB = NULL;
    typeList = NULL;
    running = false;
    clock_period = comm->getProcessDelay(); //Clock period
    process_delay = 1; //Number of cycles a compute_node needs to complete
}

InstructionBase* createClone(const std::shared_ptr<InstructionBase>& b) {
    InstructionBase* clone = b->clone();
    return clone;
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

 New CNs are added to the reservation table whenever a new BB is encountered. This may occur
 during device init, or when a br op commits. For each CN in a BB we reset the CN, evaluate
 if it is a phi or uncond br, and add it to our reservation table otherwise.
*********************************************************************************************/

    DPRINTF(IOAcc, "\n%s\n%s %d\n%s\n",
            "********************************************************************************",
            "   Cycle", cycle,
            "********************************************************************************");
    cycle++;

    DPRINTF(IOAcc, "Queue In-Flight Status: Cmp:%d Rd:%d Wr:%d\n", computeQueue.size(), readQueue.size(), writeQueue.size());
    //Check our compute queue to see if any compute nodes are ready to commit
    DPRINTF(LLVMInterface, "Checking Compute Queue for Nodes Ready for Commit!\n");

    for(auto i = 0; i < computeQueue.size();) {
        DPRINTF(LLVMOp, "Checking if %s has finished\n", computeQueue.at(i)->_OpCode);
        if(computeQueue.at(i)->commit()) {
            auto it = computeQueue.erase(computeQueue.begin() + i);
            i = std::distance(computeQueue.begin(), it);
        } else i++;
    }


    bool scheduled = false;
    if (reservation.empty()) { // If no compute nodes in reservation queue, load next basic block
        DPRINTF(LLVMInterface, "Schedule Basic Block!\n");
        scheduleBB(currBB);
    }

    for (auto i = 0; i < reservation.size(); i++) {
        if (reservation.at(i)->_ReturnRegister == NULL)
            DPRINTF(RuntimeQueues, "%s\n", reservation.at(i)->_OpCode);
        else
            DPRINTF(RuntimeQueues, "%s %s\n", reservation.at(i)->_OpCode, reservation.at(i)->_ReturnRegister->getName());
    }

    for (auto i = 0; i < reservation.size();) {
      //  if (reservation.at(i)->_ReturnRegister == NULL)
            //DPRINTF(LLVMOp, "Checking if %s can launch\n", reservation.at(i)->_OpCode);
      //  else
            //DPRINTF(LLVMOp, "Checking if %s returning to %s can launch\n", reservation.at(i)->_OpCode, reservation.at(i)->_ReturnRegister->getName());
        if (reservation.at(i)->_ActiveParents == 0) {
            if(!(reservation.at(i)->_Terminator)) { 
                    if(reservation.at(i)->_OpCode == "load") {
                        readQueue.push_back(reservation.at(i));
                        reservation.at(i)->compute();
                    } else if(reservation.at(i)->_OpCode == "store") {
                        writeQueue.push_back(reservation.at(i));
                        reservation.at(i)->compute();
                    } else if(reservation.at(i)->_MaxCycle==0) {
                        reservation.at(i)->compute();
                        reservation.at(i)->commit();
                    } else {
                        computeQueue.push_back(reservation.at(i));
                        reservation.at(i)->compute();
                        reservation.at(i)->commit();
                    }
                    scheduled = true;
                    auto it = reservation.erase(reservation.begin()+i);
                    i = std::distance(reservation.begin(), it);
            } else if ((reservation.at(i)->_OpCode == "br")) {
                if (reservation.size() < scheduling_threshold) {
                    prevBB = currBB; // Store current BB as previous BB for use with Phi instructions
                    reservation.at(i)->compute(); // Send instruction to runtime computation simulator 
                    currBB = findBB(reservation.at(i)->_Dest); // Set pointer to next basic block
                    auto it = reservation.erase(reservation.begin()+i); // Remove instruction from reservation table
                    i = std::distance(reservation.begin(), it);
                    //currBB->prevBB(prevBB->getName());
                    scheduleBB(currBB);
                } else i++;
            }
        } else {
            if (reservation.at(i)->_OpCode == "ret"){
                if (i==0 && computeQueue.empty() && readQueue.empty() && writeQueue.empty()) {
                    running = false;
                    comm->finish();
                }
            }
            i++;
        }
    }

    if (!scheduled) stalls++; // No new compute node was scheduled this cycle
    if (running && !tickEvent.scheduled())
    {
        schedule(tickEvent, curTick() + clock_period * process_delay);
    }
}

void
LLVMInterface::scheduleBB(BasicBlock* bb) {
/*********************************************************************************************
 BB Scheduling

 As BBs are scheduled they are added to the reservation table. Once the BB is scheduled check
 if the the BB includes Phi instructions, and if so commit them immediately. Also checks if
 the only remaining instructions are unconditional branches such as pre-headers and crit-edges
 and immediately compute and commit.
*********************************************************************************************/
    DPRINTF(LLVMInterface, "Adding BB: (%s) to Reservation Table!\n", bb->getName());
    for (auto i = 0; i < bb->_Nodes.size(); i++) {
        DPRINTF(LLVMOp, "Adding %s to reservation table\n", bb->_Nodes.at(i)->_OpCode);
        reservation.push_back(createClone(bb->_Nodes.at(i)));
        if (reservation.back()->_ReturnRegister) { //Search for other instances of the same instruction
            InstructionBase * parent = findParent(reservation.back()->_LLVMLine);
            if (parent) {
                DPRINTF(LLVMOp, "Previous instance found\n");
                reservation.back()->registerParent(parent);
                parent->registerChild(reservation.back());
            } else {
                DPRINTF(LLVMOp, "No previous instance found\n");
            }
        }
        if (reservation.back()->_OpCode == "getelementptr") {
            InstructionBase * parent = findParent(dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal);
            if (parent) {
                DPRINTF(LLVMOp, "Parent returning to register:%s found\n", dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal->getName());
                reservation.back()->registerParent(parent);
                parent->registerChild(reservation.back());
            } else {
                DPRINTF(LLVMOp, "No parent returning to register:%s found\n", dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal->getName());
                dynamic_cast<GetElementPtr*>(reservation.back())->_ActivePtr = dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal->getValue();
            }
        }
        std::vector<Register*> depList = reservation.back()->runtimeDependencies(prevBB->getName());

        if (depList.size() > 0) {
            if(!((depList.size() == 1) && (depList.at(0) == NULL))) { // Special case for GEP with global and Imm only values
                for (auto j = 0; j < depList.size(); j++) { //Search for parent nodes in scheduling and in-flight queues
                    InstructionBase * parent = findParent(depList.at(j));
                    if (parent) {
                        DPRINTF(LLVMOp, "Parent returning to register:%s found\n", depList.at(j)->getName());
                        reservation.back()->registerParent(parent);
                        parent->registerChild(reservation.back());
                    } else {
                        DPRINTF(LLVMOp, "No parent returning to register:%s found\n", depList.at(j)->getName());
                        reservation.back()->fetchDependency(j);
                    }
                }
            }
        }
    }
    DPRINTF(LLVMInterface, "Adding BB: Complete!\n");
}

InstructionBase *
LLVMInterface::findParent(Register* reg) {
    //Search queues with return registers for last instance of a node with the same return register as our target reg
    //Check the reservation queue first to ensure we get the last instance if multiple instances exist in queues
    for(int i = reservation.size()-2; i >= 0; i--) { //Start with the second to last node to avoid linking a node to itself
        if (reservation.at(i)->_ReturnRegister == reg) return reservation.at(i);
    }
    for(int i = computeQueue.size()-1; i >= 0; i--) {
        if (computeQueue.at(i)->_ReturnRegister == reg) return computeQueue.at(i);
    }
    for(int i = readQueue.size()-1; i >= 0; i--) {
        if (readQueue.at(i)->_ReturnRegister == reg) return readQueue.at(i);
    }
    return NULL;
}

InstructionBase *
LLVMInterface::findParent(std::string line) {
    //Search queues with return registers for last instance of a node with the same instruction
    //Check the reservation queue first to ensure we get the last instance if multiple instances exist in queues
    for(int i = reservation.size()-2; i >= 0; i--) { //Start with the second to last node to avoid linking a node to itself
        if (reservation.at(i)->_LLVMLine == line) return reservation.at(i);
    }
    for(int i = computeQueue.size()-1; i >= 0; i--) {
        if (computeQueue.at(i)->_LLVMLine == line) return computeQueue.at(i);
    }
    for(int i = readQueue.size()-1; i >= 0; i--) {
        if (readQueue.at(i)->_LLVMLine == line) return readQueue.at(i);
    }
    return NULL;
}

void
LLVMInterface::constructBBList() {
/*********************************************************************************************
 Constructing Basic Block List 

 Parses LLVM file and creates the CDFG passed to our runtime simulation engine. 
*********************************************************************************************/
    DPRINTF(LLVMInterface, "Constructing Dependency Graph!\n");
    bbList = new std::list<BasicBlock*>(); // Create New Basic Block List
    regList = new RegisterList(); // Create New Register List
    typeList = new TypeList(); // Create New User Defined Types List
    Register* alwaysTrue = new Register("alwaysTrue", ((uint64_t) 1));
    Register* alwaysFalse = new Register("alwaysFalse", ((uint64_t) 0));
    regList->addRegister(alwaysTrue);
    regList->addRegister(alwaysFalse);
    //pwrUtil = new Utilization(clock_period/1000);
    std::ifstream llvmFile(filename, std::ifstream::in); // Open LLVM File
    std::string line; // Stores Single Line of File
    bool inFunction = false; // Parse Variable
    unsigned bbnum = 0; // Start of Basic Block Numbering
    DPRINTF(LLVMInterface, "Parsing: (%s)\n", filename);
    if(llvmFile.is_open()) {
        while (getline(llvmFile, line)) { // Read until end of LLVM file
            DPRINTF(LLVMParse, "Line: (%s)\n", line); 
            if (!inFunction) { // Looks for data before the main function is defined
                if (!line.find("%struct")) { // Found custom data type.
                    int pos = line.find('='); 
                    int size = 1; // Size indicated number of elements in custom data type
                    std::string name = line.substr(1,pos-2); // Store name as called within function 
                    for(int i = pos; i < line.size(); i++) {
                        if(line[i] == ',') size++; // Elements delimeted by commas, counts number of elements
                    }
                    typeList->addType(new LLVMType(size, name)); // Add custom data type to typeList
                } else if (!line.find("define")) { //Found a function. Need to parse its header
                    DPRINTF(LLVMParse, "Found ACC Function, Parsing Global Variables!\n");
                    inFunction = true;
                    unsigned paramNum = 0;
                    unsigned linePos = 0;
                    int percPos = line.find("%"); //All registers preceeded by a % in LLVM
                    int commaPos;
                    while (percPos > -1) {
                        // Parse all global variables within the function definition 
                        if (line.find("%struct", linePos) != percPos) { //Ensure we didn't just find a struct type
                            percPos++;
                            commaPos = line.find(",", percPos);
                            if (commaPos < 0) commaPos = line.find(")");
                            std::string regName = line.substr(percPos, (commaPos-percPos)); // Determine register name for global variable
                            DPRINTF(LLVMParse, "Creating register for: (%s)\n", regName); 
                            regList->addRegister(new Register(regName, comm->getGlobalVar(paramNum))); // Create register for global variable
                            DPRINTF(LLVMParse, "Initial Value: (%X)\n", (regList->findRegister(regName))->getValue());
                            paramNum++;
                        }
                        linePos = percPos + 1;
                        percPos = line.find("%", linePos); // Check if another register exists within the function definition
                    }
                    currBB = new BasicBlock("0", bbnum); // First basic block is always defined as BB 0
                    DPRINTF(LLVMParse, "Found Basic Block: (%s)\n", currBB->_Name);
                    bbnum++; // Increment BB count
                    bbList->push_back(currBB); // Add BB to BB list
                }
            } else { // Already within a function, begin defining basic blocks and compute nodes
                if (line.find("\n") > 0) { // Skip blank lines
                    if (line.find("; <label>:") == 0) { // Found new basic block
                        int labelEnd = line.find(" ", 10);
                        prevBB = currBB; // Set previous basic block
                       // currBB->printNodes();  ////////////////////////////////////////////////////////////////////////////////////////////
                        currBB = new BasicBlock(line.substr(10,(labelEnd - 10)), bbnum); // Create new basic block
                        DPRINTF(LLVMParse, "Found Basic Block: (%s)\n", currBB->_Name);
                        bbnum++; // Increment BB count
                        bbList->push_back(currBB); // Add BB to BB list
                    } else if (line.find(".") == 0) { // Found new basic block (edge)
                        int labelEnd = line.find(" "); 
                        prevBB = currBB; // Set previous basic block
                        currBB = new BasicBlock(line.substr(0,(labelEnd-1)), bbnum); // Create new basic block
                        DPRINTF(LLVMParse, "Found Basic Block: (%s)\n", currBB->_Name); 
                        bbnum++; // Increment BB count
                        bbList->push_back(currBB); // Add BB to BB list
                    } else if (line.find("}") == 0) { // Found end of function definition
                        inFunction = false;
                        DPRINTF(LLVMParse, "Finished File Parsing!\n");
                        break;
                    } else if (!(line.find_first_not_of(' ') != std::string::npos)){ // Skip empty Line
                    } else { // Found instruction, create new compute node within the current BB
                        DPRINTF(LLVMParse, "Registering Compute Node for: (%s)\n", line);
                        if ((line.find("switch") != -1)) {
                            // If instruction is switch statement, convert it to be defined in a single line
                            DPRINTF(LLVMParse, "Found Switch Statement, Converting to Inline!\n");
                            std::string concatLine;
                            int cases = 1;
                            while(line.find(" ]") == -1) { 
                                // Continue reading through lines until the end of switch statement
                                // Concatinate all instruction lines into a single string
                                DPRINTF(LLVMParse, "Case %d: (%s)\n", cases, line);
                                concatLine += line;
                                getline(llvmFile, line);
                                cases++;
                            }
                        concatLine+= " ]"; // Close case statements arguement
                        line = concatLine;
                        DPRINTF(LLVMParse, "New Switch Instruction Line: (%s)\n", line);
                        }
                        if(prevBB) { // Add instruction line to compute node list in current BB
                            currBB->parse(line, regList, prevBB->getName(), comm, typeList);
                        } else { // Add instruction line to compute node list in current BB (Fist BB Only)
                            currBB->parse(line, regList, "NULL", comm, typeList);
                        }
                    }
                }
            }
        }
        currBB = findBB("0"); //
        prevBB = currBB; 
    } else { // Could not find LLVM file
        panic("Unable to open LLVM file!\n");
    }
    //regList->printRegNames();
}

BasicBlock*
LLVMInterface::findBB(std::string bbname) {
/*********************************************************************************************
 Find Basic Block 

 Iterates through list of known basic block and returns a pointer to passed BB name if found
 and NULL if the BB does not exist in the BB list.
*********************************************************************************************/
    for (auto it = bbList->begin(); it != bbList->end(); ++it) {
        if ((*it)->_Name.compare(bbname) == 0)
            return (*it);
    }
    return NULL;
}

void
LLVMInterface::readCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Read Request 
*********************************************************************************************/
    for (auto i = 0; i < readQueue.size(); i++ ) {
        if(readQueue.at(i)->getReq() == req) {
            readQueue.at(i)->setResult(req->buffer);
            readQueue.at(i)->commit();
            readQueue.erase(readQueue.begin() + i);
        }
    }
}

void
LLVMInterface::writeCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Write Request 
*********************************************************************************************/    
   for (auto i = 0; i < writeQueue.size(); i++ ) {
        if(writeQueue.at(i)->getReq() == req) {
            writeQueue.at(i)->commit();
            writeQueue.erase(writeQueue.begin() + i);
        }
    }
}

void
LLVMInterface::initialize() {
/*********************************regPwr.************************************************************
 Initialize the Runtime Engine

 Calls function that constructs the basic block list, initializes the reservation table and
 read, write, and compute queues. Set all data collection variables to zero.
*********************************************************************************************/     
    DPRINTF(LLVMInterface, "Initializing LLVM Runtime Engine!\n");
    running = true;
    constructBBList();
    DPRINTF(LLVMInterface, "Initializing Reservation Table!\n");
    DPRINTF(LLVMInterface, "Initializing readQueue Queue!\n");
    DPRINTF(LLVMInterface, "Initializing writeQueue Queue!\n");
    DPRINTF(LLVMInterface, "Initializing computeQueue List!\n");
    DPRINTF(LLVMInterface, "\n%s\n%s\n%s\n",
            "*******************************************************************************",
            "*                 Begin Runtime Simulation Computation Engine                 *",
            "*******************************************************************************");
    cycle = 0;
    stalls = 0;
    execnodes = 0;
    tick();
}

void
LLVMInterface::startup() {
/*********************************************************************************************
 Initialize communications between gem5 interface and simulator
*********************************************************************************************/ 
    comm->registerCompUnit(this);
}

LLVMInterface*
LLVMInterfaceParams::create() {
/*********************************************************************************************
 Create new interface between the llvm IR and our simulation engine
*********************************************************************************************/     
    return new LLVMInterface(this);
}

void
LLVMInterface::statistics() {
/*********************************************************************************************
 Prints usage statistics of how many times each instruction was accessed during runtime
*********************************************************************************************/ 
    /*
    for(auto it = opCount.begin(); it != opCount.end(); ++it)  {
        DPRINTF(IOAcc, "Instruction (Count): %s  (%d)\n", it->first, it->second);
    }
    */
   /*
    DPRINTF(Hardware, "%s %s %s",
    "\n*******************************************************************************",
    "\n******************************* Instruction Usage *****************************",
    "\n*******************************************************************************\n");
    DPRINTF(Hardware, "Floating Point Instructions:\n");
    for(auto it = pwrUtil->floats.begin(); it != pwrUtil->floats.end(); ++it)  {
        DPRINTF(Hardware, "[%s]-(%d)\n", it->first, it->second);
    }
    DPRINTF(Hardware, "Integer Instructions:\n");
    for(auto it = pwrUtil->integer.begin(); it != pwrUtil->integer.end(); ++it)  {
        DPRINTF(Hardware, "[%s]-(%d)\n", it->first, it->second);
    }
    DPRINTF(Hardware, "Custom Instructions:\n");
    for(auto it = pwrUtil->others.begin(); it != pwrUtil->others.end(); ++it)  {
        DPRINTF(Hardware, "[%s]-(%d)\n", it->first, it->second);
    }
    DPRINTF(Hardware, "Bitwise Instructions:\n");
    for(auto it = pwrUtil->bitCount.begin(); it != pwrUtil->bitCount.end(); ++it)  {
        DPRINTF(Hardware, "[%s]-(%d)\n", it->first, it->second);
    }
    DPRINTF(Hardware, "Shift Instructions:\n");
    for(auto it = pwrUtil->shiftCount.begin(); it != pwrUtil->shiftCount.end(); ++it)  {
        DPRINTF(Hardware, "[%s]-(%d)\n", it->first, it->second);
    }
    DPRINTF(Hardware, "Other Instructions:\n");
    for(auto it = pwrUtil->opCount.begin(); it != pwrUtil->opCount.end(); ++it)  {
        DPRINTF(Hardware, "[%s]-(%d)\n", it->first, it->second);
    }
    DPRINTF(Hardware, "%s %s %s",
    "\n*******************************************************************************",
    "\n*******************************  Register Usage   *****************************",
    "\n*******************************************************************************\n");

    for (auto it=regList->beginit(); it!=regList->endit(); ++it) {
        DPRINTF(Hardware, "[%s] Reads-(%d) Writes-(%d)\n", (*it)->getName(),(*it)->getRead(), (*it)->getWrite());
    }
    DPRINTF(Hardware, "%s %s %s",
    "\n*******************************************************************************",
    "\n*******************************   Final Results   *****************************",
    "\n*******************************************************************************\n");

    double divisor = cycle * (clock_period / 1000);

    DPRINTF(IOAcc,"%s %s %d %s %f %s %d %s %d  %s %f %s %s %f %s %s %f %s %s %f %s %s %f %s %s %f %s %f %s %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s",
    "\n*******************************************************************************",
    "\n   Runtime (Cycles): ", cycle,
    "\n   Runtime (Seconds):", (cycle*10*(1e-12)),
    "\n   Stalls  (Cycles): ", stalls,
    "\n   Executed Nodes: ", execnodes,
    "\n   Average Power: ", (pwrUtil->getLeakage()+ pwrUtil->getDynEnergy()/divisor+pwrUtil->getRegLeak()+pwrUtil->getRegDyn()/divisor), "mW",
    "\n   FU Leakage Power: ", pwrUtil->getLeakage(), "mW",
    "\n   FU Dynamic Power: ", pwrUtil->getDynEnergy()/divisor, "mW",
    "\n   Register Leakage Power: ", pwrUtil->getRegLeak(), "mW",
    "\n   Register Dynamic Power: ", pwrUtil->getRegDyn()/divisor, "mW",
    "\n   FU Area = ",pwrUtil->getArea()," um^2, (", pwrUtil->getArea()/1000000, " mm^2)",
    "\n   Number of Double Precision FP Multipliers: ", pwrUtil->fpMaxMul(),
    "\n   Number of Double Precision FP Adders: ", pwrUtil->fpMaxAdd(),
    "\n   Number of Bit-Wise Operator: ", pwrUtil->maxBit(),
    "\n   Number of Shifters: ", pwrUtil->maxShift(),
    "\n   Number of Integer Adders: ", pwrUtil->intMaxAdd(),
    "\n   Number of Integer Multipliers: ", pwrUtil->intMaxMul(),
    "\n   Number of Registers: ", regList->size(),
    "\n*******************************************************************************\n");
    */
}

