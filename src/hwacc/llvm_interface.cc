#include <iostream>
#include <fstream>
#include "hwacc/llvm_interface.hh"

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

    DPRINTF(IOAcc, "\n%s\n%s %d\n%s\n",
            "********************************************************************************",
            "   Cycle", cycle,
            "********************************************************************************");
    cycle++;
    DPRINTF(IOAcc, "Queue In-Flight Status: Cmp:%d Rd:%d Wr:%d\n", computeQueue->size(), readQueue->size(), writeQueue->size());
    //Check our compute queue to see if any compute nodes are ready to commit
    DPRINTF(LLVMInterface, "Checking Compute Queue for Nodes Ready for Commit!\n");

    int intMultiplierCount = 0;
    int intAdderCount = 0;
    int fpMultiplierCount = 0;
    int fpAdderCount = 0;
    int bitCount = 0;
    int shiftCount = 0;
    pwrUtil->clearAll();
    for (auto it = computeQueue->begin(); it != computeQueue->end(); ) {
        if ((*it)->commit()) {
            // If compute node is ready to commit, fetch the updated instruction struct
            Instruction instr = (*it)->getInstruction(); // Update instruction data struct
            pwrUtil->update(instr);
            // opCount[instr.general.opCode]++; // Increment intruction use counter
            it = computeQueue->erase(it); // Remove the compute node from the queue
            // Determine maximum number of hardware units needed
            if(instr.general.multiplier) {
                if(instr.general.integer) {
                    intMultiplierCount++;
                    pwrUtil->currUnits(intMultiplierCount, MULUNIT, instr.general.floatingPoint);
                } else if(instr.general.floatingPoint) {
                    fpMultiplierCount++;
                    pwrUtil->currUnits(fpMultiplierCount, MULUNIT, instr.general.floatingPoint);
                }
            } else if(instr.general.adder) {
                if(instr.general.integer) {
                    intAdderCount++;
                    pwrUtil->currUnits(intAdderCount, ADDUNIT, instr.general.floatingPoint);
                } else if(instr.general.floatingPoint) {
                    fpAdderCount++;
                    pwrUtil->currUnits(fpAdderCount, ADDUNIT, instr.general.floatingPoint);
                }
            } else if(instr.general.bit) {
                bitCount++;
                pwrUtil->currUnits(bitCount, BITUNIT, instr.general.floatingPoint);
            } else if(instr.general.shifter) {
                shiftCount++;
                pwrUtil->currUnits(shiftCount, SHIFTUNIT, instr.general.floatingPoint);
            }
            // ///////////////////////////////////////////////////////////
        } else {
            ++it; // Compute node is not ready, check next node
        }
    }
    pwrUtil->calculateDynamicPowerUsage();
    DPRINTF(Hardware, "Integer Units: (%d) Add | (%d) Mul | (%d) Bit | (%d) Shift\n",pwrUtil->currintAdd(), pwrUtil->currintMul(), pwrUtil->currBit(), pwrUtil->currShift());
    DPRINTF(Hardware, "FP Units: (%d) Add | (%d) Mul\n",pwrUtil->currfpAdd(), pwrUtil->currfpMul());
   // pwrUtil->clearAll();

    if (reservation->empty()) { // If no compute nodes in reservation queue, load next basic block
        DPRINTF(LLVMInterface, "Schedule Basic Block!\n");
        scheduleBB(currBB);
    }
    bool scheduled = false;
    for (auto it=reservation->begin(); it!=reservation->end(); ) {
        Instruction instr = (*it)->getInstruction(); // Update instruction data struct
        DPRINTF(LLVMInterface, "Next:(%s)\n", instr.general.llvm_Line);
        // If instruction is not of terminator type and has no hot dependencies
        if (!(instr.general.terminator) && !((*it)->checkDependency())) {
            DPRINTF(LLVMInterface, "Non-Terminator Instruction Operation!\n");
            if (instr.general.opCode.find("load") == 0) { // Load Instruction
                DPRINTF(LLVMInterface, "Queueing Load Instruction!\n");
                scheduled = true;
                execnodes++;
                readQueue->push_back(*it); // Interface instruction with gem5 memory management
                (*it)->compute(); // Send instruction to runtime computation simulator
            } else if (instr.general.opCode.find("store") == 0) { // Store Instruction
                DPRINTF(LLVMInterface, "Queueing Store Instruction!\n");
                scheduled = true;
                execnodes++;
                writeQueue->push_back(*it); // Interface instruction with gem5 memory management
                (*it)->compute(); // Send instruction to runtime computation simulator
            } else if (instr.general.opCode.find("phi") == 0) { // Phi Instruction
                DPRINTF(LLVMInterface, "Queueing Phi Instruction!\n");
                scheduled = true;
                (*it)->compute(); // Send instruction t144834computation simulator
                (*it)->commit(); // Phi instructions ha144834ndencies within BB, commit immediately 
            } else { // General Computation Instruction144834
                DPRINTF(LLVMInterface, "Queueing Compute Instruction!\n"); 
                scheduled = true;
                execnodes++;
                computeQueue->push_back(*it); // Interface instruction with runtime computation engine
                (*it)->compute(); // Send instruction to runtime computation simulator
            }
            pwrUtil->update(instr);
            // opCount[instr.general.opCode]++; // Increment instruction use counter
            it = reservation->erase(it); // Remove instruction from reservation table
        } else if ((instr.general.opCode.compare("br") == 0) && !((*it)->checkDependency())) {
            // If instruction is a branch instruction with no hot dependencies
            if(readQueue->empty() && writeQueue->empty() && computeQueue->empty()) {
                // Only excute branch instructions once all other compute nodes in BB are complete
                DPRINTF(LLVMInterface, "Branch Operation In Progress!\n");
                prevBB = currBB; // Store current BB as previous BB for use with Phi instructions
                (*it)->compute(); // Send instruction to runtime computation simulator 
                instr = (*it)->getInstruction(); // Update instruction data struct
                DPRINTF(LLVMInterface, "Branching to Basic Block: (%s)\n", instr.terminator.dest);
                currBB = findBB(instr.terminator.dest); // Set pointer to next basic block
                pwrUtil->update(instr);
                // opCount[instr.general.opCode]++; // Increment intruction use counter 
                it = reservation->erase(it); // Remove instruction from reservation table
                scheduleBB(currBB); // Add next basic block to reservation table
            } else {
                ++it; // If compute nodes still remain in queue, do not branch
            }
        } else if ((instr.general.opCode.compare("switch") == 0) && !((*it)->checkDependency())) {
            // If instruction is a switch instruction with no hot dependencies
            if(readQueue->empty() && writeQueue->empty() && computeQueue->empty()) {
                // Only excute switch instructions once all other compute nodes in BB are complete
                DPRINTF(LLVMInterface, "Switch Operation In Progress!\n");
                prevBB = currBB; // Store current BB as previous BB for use with Phi instructions
                (*it)->compute(); // Send instruction to runtime computation simulator
                instr = (*it)->getInstruction(); // Update instruction data struct
                DPRINTF(LLVMInterface, "Branching to Basic Block from Switch Statement: (%s)\n", instr.terminator.dest);
                currBB = findBB(instr.terminator.dest); // Set pointer to next basic block
                pwrUtil->update(instr);
                // opCount[instr.general.opCode]++; // Increment intruction use counter
                it = reservation->erase(it); // Remove instruction from reservation table
                scheduleBB(currBB); // Add next basic block to reservation table
            } else {
                ++it; // If compute nodes still remain in queue, do not branch
            }
        }else if (instr.general.opCode.compare("ret") == 0) { // Return Instruction
            if(readQueue->empty() && writeQueue->empty() && computeQueue->empty()) {
                // Only execute instruction once all other compute nodes in BB are complete
                running = false;
                //We are done!!!!
                /*
                DPRINTF(IOAcc,"%s %s %d %s %f %s %d %s %d %s %d %s",
                        "\n*******************************************************************************",
                        "\n   Runtime (Cycles): ", cycle,
                        "\n   Runtime (Seconds):", (cycle*10*(1e-12)),
                        "\n   Stalls  (Cycles): ", stalls,
                        "\n   Executed Nodes: ", execnodes,
                        "\n   Number of Registers: ", regList->size(),
                        "\n*******************************************************************************\n");
                */
                regList->calculateTotals();
                pwrUtil->calculateLeakagePowerUsage();
                pwrUtil->calculateRegisterPowerUsage(regList->getReads(),regList->getWrites(),regList->getCount(),regList->getWordSize());
                pwrUtil->calculateArea();
                statistics(); // Prints out instruction use count
                comm->finish(); // Signal to gem5 simulation is complete
                break; 
            }
        } else {
            ++it; // Current compute node is not ready to be executed
        }
    }
    if (!scheduled) stalls++; // No new compute node was scheduled this cycle
    if (running && !tickEvent.scheduled())
    {
        schedule(tickEvent, curTick() + clock_period * process_delay);
    }
}

void
LLVMInterface::scheduleBB(BasicBlock * bb) {
/*********************************************************************************************
 BB Scheduling

 As BBs are scheduled they are added to the reservation table. Once the BB is scheduled check
 if the the BB includes Phi instructions, and if so commit them immediately. Also checks if
 the only remaining instructions are unconditional branches such as pre-headers and crit-edges
 and immediately compute and commit.
*********************************************************************************************/
    DPRINTF(LLVMInterface, "Adding BB: (%s) to Reservation Table!\n", bb->getName());
    for(auto it=bb->cnList->begin(); it!=bb->cnList->end(); ++it) {
        Instruction instr = (*it)->getInstruction(); // Update instruction data struct
        //if it is a phi and we don't have an unmet dependency -> commit immediately
        if(instr.general.phi) {
            (*it)->setPrevBB(prevBB->name); // Set previous BB in computation simulator
            if (!(*it)->checkDependency()) { 
                // Phi should never have runtime dependency once BB is scheduled
                (*it)->compute(); // Send instruction to runtime computation simulator
               // (*it)->commit(); // Store Phi 
               pwrUtil->update(instr);
               // opCount[instr.general.opCode]++; // Increment intruction use counter
            } else {
                // Phi instruction should never have a dependency when scheduled
                DPRINTF(IOAcc, "Error: Phi Instruction Scheduled with Dependency!\n");
                (*it)->reset(); // Set return registers back to hot 
                reservation->push_back(*it); // Add instruction back to reservation table
            } 
        DPRINTF(LLVMInterface, "Phi Instructions Completed!\n");
        //else if it is an unconditional branch -> evaluate immediately
        } else if (instr.general.terminator && instr.terminator.unconditional) {
            prevBB = currBB; // Set previous BB
            (*it)->compute(); // Send instruction to runtime computation simulator
            instr = (*it)->getInstruction(); // Update instruction data struct
            currBB = findBB(instr.terminator.dest); // Set current BB
            assert(currBB); 
            scheduleBB(currBB); // Add BB to reservation table
        } else {
            // BB does not contain any additonal Phi nodes and Terminator is not Ready 
            (*it)->reset(); // Set return registers back to hot 
            reservation->push_back(*it); // Add instruction back to reservation table
        }
    }
}

void
LLVMInterface::constructBBList() {
/*********************************************************************************************
 Constructing Basic Block List 

 Parses LLVM file and creates the CDFG passed to our runtime simulation engine. 
*********************************************************************************************/
    DPRINTF(LLVMInterface, "Constructing Dependency Graph!\n");
    bbList = new std::list<BasicBlock*>();
    regList = new RegisterList();
    typeList = new TypeList();
    pwrUtil = new Utilization(clock_period/1000);
    std::ifstream llvmFile(filename, std::ifstream::in);
    std::string line;
    regList->addRegister(new Register("ImmediateValue"));
    regList->addRegister(new Register("Label"));
    bool inFunction = false;
    unsigned bbnum = 0;
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
                    DPRINTF(LLVMParse, "Found Basic Block: (%s)\n", currBB->name);
                    bbnum++; // Increment BB count
                    bbList->push_back(currBB); // Add BB to BB list
                }
            } else { // Already within a function, begin defining basic blocks and compute nodes
                if (line.find("\n") > 0) { // Skip blank lines
                    if (line.find("; <label>:") == 0) { // Found new basic block
                        int labelEnd = line.find(" ", 10);
                        prevBB = currBB; // Set previous basic block
                        currBB = new BasicBlock(line.substr(10,(labelEnd - 10)), bbnum); // Create new basic block
                        DPRINTF(LLVMParse, "Found Basic Block: (%s)\n", currBB->name);
                        bbnum++; // Increment BB count
                        bbList->push_back(currBB); // Add BB to BB list
                    } else if (line.find(".") == 0) { // Found new basic block (edge)
                        int labelEnd = line.find(" "); 
                        prevBB = currBB; // Set previous basic block
                        currBB = new BasicBlock(line.substr(0,(labelEnd-1)), bbnum); // Create new basic block
                        DPRINTF(LLVMParse, "Found Basic Block: (%s)\n", currBB->name); 
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
                            currBB->addNode(new ComputeNode(line, regList, prevBB->getName(), comm, typeList));
                        } else { // Add instruction line to compute node list in current BB (Fist BB Only)
                            currBB->addNode(new ComputeNode(line, regList, "NULL", comm, typeList));
                        }
                    }
                }
            }
        }
        currBB = findBB("0"); // 
    } else { // Could not find LLVM file
        panic("Unable to open LLVM file!\n");
    }
    if(PRINTREGISTERS) regList->printRegNames(); // Debug option, prints all registers created during parsing.
}

BasicBlock*
LLVMInterface::findBB(std::string bbname) {
/*********************************************************************************************
 Find Basic Block 

 Iterates through list of known basic block and returns a pointer to passed BB name if found
 and NULL if the BB does not exist in the BB list.
*********************************************************************************************/
    for (auto it = bbList->begin(); it != bbList->end(); ++it) {
        if ((*it)->name.compare(bbname) == 0)
            return (*it);
    }
    return NULL;
}

void
LLVMInterface::readCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Read Request 
*********************************************************************************************/
    for (auto it=readQueue->begin(); it!=readQueue->end(); ++it) {
        if ((*it)->getReq() == req) {
            Instruction instr = (*it)->getInstruction(); // Update instruction data struct
            instr.general.returnRegister->setValue(req->buffer); // Store read value into the return register of compute node
            instr.general.returnRegister->commit(); // Commit the stored value and set register as ready to be read
            DPRINTF(LLVMInterface, "Read Operation Complete!\n");
            DPRINTF(LLVMRegister, "Commit Loaded Value Into Register: (%s)\n", instr.general.returnRegister->getName());
            it = readQueue->erase(it); // Remove compute node from queue
        }
    }
}

void
LLVMInterface::writeCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Write Request 
*********************************************************************************************/    
    for (auto it=writeQueue->begin(); it!=writeQueue->end(); ++it) {
        if ((*it)->getReq() == req) {
            Instruction instr = (*it)->getInstruction(); // Update instruction data struct
            DPRINTF(LLVMInterface, "Store Operation Complete!\n");
            if(instr.memory.store.immediate) DPRINTF(LLVMRegister, "Immediate Value Stored Into Memory!\n");
            else DPRINTF(LLVMRegister, "Value From Register (%s) Stored Into Memory!\n", instr.memory.store.value->getName());
            it = writeQueue->erase(it); // Remove compute node from queue
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
    reservation = new std::list<ComputeNode*>();
    DPRINTF(LLVMInterface, "Initializing readQueue Queue!\n");
    readQueue = new std::list<ComputeNode*>();
    DPRINTF(LLVMInterface, "Initializing writeQueue Queue!\n");
    writeQueue = new std::list<ComputeNode*>();
    DPRINTF(LLVMInterface, "Initializing computeQueue List!\n");
    computeQueue = new std::list<ComputeNode*>();
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

}

