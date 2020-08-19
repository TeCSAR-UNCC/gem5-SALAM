//------------------------------------------//
#include "hwacc/llvm_interface.hh"
//------------------------------------------//

LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    ComputeUnit(p),
    filename(p->in_file),
    lockstep(p->lockstep_mode),
    scheduling_threshold(p->sched_threshold),
    counter_units(p->FU_counter),
    int_adder_units(p->FU_int_adder),
    int_multiply_units(p->FU_int_multiplier),
    int_shifter_units(p->FU_int_shifter),
    int_bit_units(p->FU_int_bit),
    fp_sp_adder(p->FU_fp_sp_adder),
    fp_dp_adder(p->FU_fp_dp_adder),
    fp_sp_multiply(p->FU_fp_sp_multiplier),
    fp_sp_division(p->FU_fp_sp_divider),
    fp_dp_multiply(p->FU_fp_dp_multiplier),
    fp_dp_division(p->FU_fp_dp_divider),
    compare(p->FU_compare),
    gep(p->FU_GEP),
    conversion(p->FU_conversion),
    pipelined(p->FU_pipelined),
    fu_latency(p->FU_clock_period),
    clock_period(p->clock_period) {
    // bbList = NULL;
    regList = NULL;
    // currBB = NULL;
    // prevBB = NULL;
    typeList = NULL;
    running = false;
    clock_period = clock_period * 1000;
    //process_delay = 1; //Number of cycles a compute_node needs to complete
}

// SALAM::Instruction* createClone(const std::shared_ptr<SALAM::Instruction>& b) {
//     SALAM::Instruction* clone = b->clone();
//     return clone;
// }

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
    bool dbg = debug();
    if (dbg) {
        DPRINTF(LLVMInterface, "\n%s\n%s %d\n%s\n",
            "********************************************************************************",
            "   Cycle", cycle,
            "********************************************************************************");
    }
    cycle++;
    comm->refreshMemPorts();
    // ///////////////////////////
    // loadOpScheduled = false;
    // storeOpScheduled = false;
    // compOpScheduled = false;
    // loadInFlight = readQueue.size();
    // storeInFlight = writeQueue.size();
    // compInFlight = computeQueue.size();
    // ///////////////////////////
    // if (dbg) DPRINTF(LLVMInterface, "Queue In-Flight Status: Cmp:%d Rd:%d Wr:%d\n", computeQueue.size(), readQueue.size(), writeQueue.size());
    // //Check our compute queue to see if any compute nodes are ready to commit
    // if (dbg) DPRINTF(LLVMInterface, "Checking Compute Queue for Nodes Ready for Commit!\n");
    // for(auto i = 0; i < computeQueue.size();) {
    //     if (dbg) DPRINTF(LLVMOp, "Checking if %s has finished\n", computeQueue.at(i)->_OpCode);
    //     if(computeQueue.at(i)->commit()) {
    //     //////////////// Computation Complete ////////////////////////
    //         if (hardware->isMultistaged((int) (computeQueue.at(i)->_FunctionalUnit))) {
    //             if (dbg) DPRINTF(LLVMInterface, "Multistage Operation Completed\n");
    //             computeQueue.at(i)->stage();
    //         }
    //         computeQueue.at(i)->reset();
    //         auto it = computeQueue.erase(computeQueue.begin() + i);
    //         i = std::distance(computeQueue.begin(), it);
    //     } else {
    //     //////////////// Computation Incomplete ////////////////////////
    //         if (hardware->isMultistaged((int) (computeQueue.at(i)->_FunctionalUnit))) {
    //         //////////////// Multistaged Scheduling ////////////////////////
    //             if (dbg) DPRINTF(LLVMInterface, "Multistage Operation: Current Stage = %d \n", computeQueue.at(i)->currentStage());
    //             // Check if current cycle is at a stage boundary
    //             if ((computeQueue.at(i)->_CurrCycle) == computeQueue.at(i)->stageCycle(computeQueue.at(i)->currentStage())) {
    //                 // Check if already in final stage, gets priority on available hardware
    //                 if (computeQueue.at(i)->currentStage() == computeQueue.at(i)->stageCount()) {
    //                     if (dbg) DPRINTF(LLVMInterface, "Multistage Operation: Final Stage \n");
    //                     hardware->updateStage(computeQueue.at(i)->_FunctionalUnit, (computeQueue.at(i)->currentStage()), (computeQueue.at(i)->getActiveFU()));
    //                     if ((computeQueue.at(i)->_ReturnRegister) != NULL) computeQueue.at(i)->cycle();
    //                 // If not final stage but at stage boundary, check if there is available hardware for the next stage          
    //                 } else if (hardware->updateStage(computeQueue.at(i)->_FunctionalUnit, (computeQueue.at(i)->currentStage() + 1), (computeQueue.at(i)->getActiveFU()))) {
    //                     if (dbg) DPRINTF(LLVMInterface, "%s Functional Unit Stage %d Available, Advancing to Next Stage \n", computeQueue.at(i)->_OpCode, (computeQueue.at(i)->currentStage()+1));
    //                     if ((computeQueue.at(i)->_ReturnRegister) != NULL) computeQueue.at(i)->cycle();
    //                     computeQueue.at(i)->stage();
    //                 // No hardware available, wait until next cycle
    //                 } else {
    //                     if (dbg) DPRINTF(LLVMInterface, "%s Functional Unit Stage %d Not Available, Cycle Count Stalled \n", computeQueue.at(i)->_OpCode, (computeQueue.at(i)->currentStage()+1));
    //                     hardware->updateStage(computeQueue.at(i)->_FunctionalUnit, (computeQueue.at(i)->_CurrStage), (computeQueue.at(i)->getActiveFU()));
    //                 }
    //             // Multistaged but not at boundary, cycle like normal 
    //             } else { 
    //                 if((computeQueue.at(i)->_ReturnRegister) != NULL) computeQueue.at(i)->cycle();
    //                 hardware->updateStage(computeQueue.at(i)->_FunctionalUnit, (computeQueue.at(i)->_CurrStage), (computeQueue.at(i)->getActiveFU()));
    //             }
    //         //////////////// Single Stage Scheduling ////////////////////////
    //         } else {
    //             if ((computeQueue.at(i)->_ReturnRegister) != NULL) computeQueue.at(i)->cycle();
    //         } 
    //         i++;
    //     }
    // }
    // if (reservation.empty()) { // If no compute nodes in reservation queue, load next basic block
    //     if (dbg) DPRINTF(LLVMInterface, "Schedule Basic Block!\n");
    //     scheduleBB(currBB);
    // }
    // //////////////// Re-Check In-Flight Queues ////////////////////////
    // if (lockstep && (!computeQueue.empty() || !readQueue.empty() || !writeQueue.empty() )) {
    //     //Do nothing - all queues empty
    // } else {
    // //////////////// Check Reservation Queues ////////////////////////
    //     for (auto i = 0; i < reservation.size();) {
    //         if (reservation.at(i)->_ReturnRegister == NULL) {
    //             if (dbg) DPRINTF(RuntimeQueues, "Checking if %s can launch\n", reservation.at(i)->_OpCode);
    //         } else {
    //             if (dbg) DPRINTF(RuntimeQueues, "Checking if %s returning to %s can launch\n", reservation.at(i)->_OpCode, reservation.at(i)->_ReturnRegister->getName());
    //         }
    //         if (reservation.at(i)->_ActiveParents == 0) {
    //             if(!(reservation.at(i)->_Terminator)) { 
    //             //////////////// Computation & Memory Operations ////////////////////////
    //                 if(reservation.at(i)->_OpCode == "load") {
    //                 //////////////// Load ////////////////////////
    //                     loadOpScheduled = true;
    //                     readQueue.push_back(reservation.at(i));
    //                     reservation.at(i)->compute();
    //                     reservation.at(i)->used();
    //                     hardware->memoryLoad(reservation.at(i)->_ReturnRegister->getSize());
    //                     auto it = reservation.erase(reservation.begin()+i);
    //                     i = std::distance(reservation.begin(), it);
    //                 } else if(reservation.at(i)->_OpCode == "store") {
    //                 //////////////// Store ////////////////////////
    //                     storeOpScheduled = true;
    //                     writeQueue.push_back(reservation.at(i));
    //                     reservation.at(i)->compute();
    //                     reservation.at(i)->used();
    //                     //hardware->memoryStore(reservation.at(i)->_ReturnRegister->getSize());
    //                     auto it = reservation.erase(reservation.begin()+i);
    //                     i = std::distance(reservation.begin(), it);
    //                 } else if(reservation.at(i)->_MaxCycle == 0) { 
    //                 //////////////// 0-Cycle ////////////////////////
    //                     if(hardware->reserveFU(reservation.at(i)->_FunctionalUnit) >= 0) {
    //                         if (dbg) DPRINTF(LLVMInterface, "Functional Units Available\n");
    //                         reservation.at(i)->compute();
    //                         reservation.at(i)->used();
    //                         reservation.at(i)->commit();
    //                         if((reservation.at(i)->_ReturnRegister) != NULL) reservation.at(i)->cycle();
    //                         compOpScheduled = true;
    //                         auto it = reservation.erase(reservation.begin()+i);
    //                         i = std::distance(reservation.begin(), it);
    //                     } else {
    //                         if (dbg) DPRINTF(LLVMInterface, "No Functional Units Available\n");
    //                         i++;
    //                     }
    //                 } else { 
    //                 //////////////// All Other Instructions ////////////////////////
    //                     if(hardware->reserveFU(reservation.at(i)->_FunctionalUnit) >= 0) {
    //                         if (dbg) DPRINTF(LLVMInterface, "Functional Units Available\n");
    //                         reservation.at(i)->setActiveFU(hardware->reserveFU(reservation.at(i)->_FunctionalUnit));
    //                         computeQueue.push_back(reservation.at(i));
    //                         reservation.at(i)->compute();
    //                         reservation.at(i)->used();
    //                         reservation.at(i)->commit();
    //                         if((reservation.at(i)->_ReturnRegister) != NULL) reservation.at(i)->cycle();
    //                         compOpScheduled = true;
    //                         auto it = reservation.erase(reservation.begin()+i);
    //                         i = std::distance(reservation.begin(), it);
    //                     } else {
    //                         if (dbg) DPRINTF(LLVMInterface, "No Functional Units Available\n");
    //                         i++;
    //                     }
    //                 }
    //             } else if ((reservation.at(i)->_OpCode != "ret")) {
    //             //////////////// Control Flow Instructions ////////////////////////
    //                 if (reservation.size() < scheduling_threshold) {
    //                     hardware->controlFlow();
    //                     prevBB = currBB; // Store current BB as previous BB for use with Phi instructions
    //                     reservation.at(i)->compute(); // Send instruction to runtime computation simulator
    //                     reservation.at(i)->used();
    //                     currBB = findBB(reservation.at(i)->_Dest); // Set pointer to next basic block
    //                     auto it = reservation.erase(reservation.begin()+i); // Remove instruction from reservation table
    //                     i = std::distance(reservation.begin(), it);
    //                     scheduleBB(currBB);
    //                 } else i++;
    //             }
    //         } else {
    //             if (reservation.at(i)->_OpCode == "ret") {
    //             //////////////// Return Instruction ////////////////////////
    //                 if (dbg) DPRINTF(LLVMInterface, "Simulation Complete \n");
    //                 if (i==0 && computeQueue.empty() && readQueue.empty() && writeQueue.empty()) {
    //                     finalize();
    //                 }
    //             }
    //             i++;
    //         }
    //     }
    // }
    //////////////// Update Hardware Utilization ////////////////////////
    hardware->update();
    occupancy();
    //////////////// Schedule Next Cycle ////////////////////////
    if (running && !tickEvent.scheduled()) {
        schedule(tickEvent, curTick() + clock_period);// * process_delay);
    }
}

// void
// LLVMInterface::scheduleBB(SALAM::BasicBlock* bb) {
// /*********************************************************************************************
//  BB Scheduling

//  As BBs are scheduled they are added to the reservation table. Once the BB is scheduled check
//  if the the BB includes Phi instructions, and if so commit them immediately. Also checks if
//  the only remaining instructions are unconditional branches such as pre-headers and crit-edges
//  and immediately compute and commit.
// *********************************************************************************************/
//     if (debug()) DPRINTF(LLVMInterface, "Adding BB: (%s) to Reservation Table!\n", bb->getName());
//     for (auto i = 0; i < bb->Nodes()->size(); i++) {
//         if (debug()) DPRINTF(LLVMOp, "Adding %s to reservation table\n", bb->Nodes()->at(i)->_OpCode);
//         reservation.push_back(createClone(bb->Nodes()->at(i)));
//         if (reservation.back()->_ReturnRegister) { //Search for other instances of the same instruction
//             InstructionBase * parent = findParent(reservation.back()->_LLVMLine);
//             if (parent) {
//                 if (debug()) DPRINTF(LLVMOp, "Previous instance found\n");
//                 reservation.back()->registerParent(parent);
//                 parent->registerChild(reservation.back());
//             } else {
//                 if (debug()) DPRINTF(LLVMOp, "No previous instance found\n");
//             }
//         }
//         if (reservation.back()->_OpCode == "load") {
//             InstructionBase * parent = detectRAW(dynamic_cast<Load*>(reservation.back())->_RawCheck);
//             if (parent) {
//                 if (debug()) DPRINTF(LLVMOp, "Memory RAW dependency corrected!\n");
//                 reservation.back()->registerParent(parent);
//                 parent->registerChild(reservation.back());
//             } else {
//                 if (debug()) DPRINTF(LLVMOp, "No RAW dependency found\n");
//             }
//         }
//         if (reservation.back()->_OpCode == "getelementptr") {
//             InstructionBase * parent = findParent(dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal);
//             if (parent) {
//                 if (debug()) DPRINTF(LLVMOp, "Parent returning to base register:%s found\n", dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal->getName());
//                 reservation.back()->registerParent(parent);
//                 parent->registerChild(reservation.back());
//             } else {
//                 if (debug()) DPRINTF(LLVMOp, "No parent returning to base register:%s found\n", dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal->getName());
//                 dynamic_cast<GetElementPtr*>(reservation.back())->_ActivePtr = dynamic_cast<GetElementPtr*>(reservation.back())->_PtrVal->getValue();
//             }
//         }
//         std::vector<Register*> depList = reservation.back()->runtimeDependencies(prevBB->getName());
//         if (depList.size() > 0) {
//             for (auto j = 0; j < depList.size(); j++) { //Search for parent nodes in scheduling and in-flight queues
//                 if (depList.at(j)!=NULL) {
//                     InstructionBase * parent = findParent(depList.at(j));
//                     if (parent) {
//                         if (debug()) DPRINTF(LLVMOp, "Parent returning to register:%s found\n", depList.at(j)->getName());
//                         reservation.back()->registerParent(parent);
//                         parent->registerChild(reservation.back());
//                     } else {
//                         if (debug()) DPRINTF(LLVMOp, "No parent returning to register:%s found\n", depList.at(j)->getName());
//                         reservation.back()->fetchDependency(j);
//                     }
//                 }
//             }
//         }
//     }
//     if (debug()) DPRINTF(LLVMInterface, "Adding BB: Complete!\n");
//     if (debug()) DPRINTF(RuntimeQueues, "Active Scheduling Window\n");
//     for (auto i = 0; i < reservation.size(); i++) {
//         if (reservation.at(i)->_ReturnRegister == NULL) {
//             if (debug()) DPRINTF(RuntimeQueues, "%s\n", reservation.at(i)->_OpCode);
//         } else {
//             if (debug()) DPRINTF(RuntimeQueues, "%s %s\n", reservation.at(i)->_OpCode, reservation.at(i)->_ReturnRegister->getName());
//         }
//     }
// }

// InstructionBase *
// LLVMInterface::detectRAW(Register* reg) {
//     for(int i = reservation.size()-2; i >= 0; i--) { //Start with the second to last node to avoid linking a node to itself
//         if(reservation.at(i)->_OpCode == "store") {
//             if(reservation.at(i)->_RawCheck == reg) return reservation.at(i);
//         }
//     }
//     for(int i = writeQueue.size()-1; i >= 0; i--) {
//         if (writeQueue.at(i)->_RawCheck == reg) return writeQueue.at(i);
//     }
//     return NULL;
// }

// InstructionBase *
// LLVMInterface::findParent(Register* reg) {
//     //Search queues with return registers for last instance of a node with the same return register as our target reg
//     //Check the reservation queue first to ensure we get the last instance if multiple instances exist in queues
//     for(int i = reservation.size()-2; i >= 0; i--) { //Start with the second to last node to avoid linking a node to itself
//         if (reservation.at(i)->_ReturnRegister == reg) return reservation.at(i);
//     }
//     for(int i = computeQueue.size()-1; i >= 0; i--) {
//         if (computeQueue.at(i)->_ReturnRegister == reg) return computeQueue.at(i);
//     }
//     for(int i = readQueue.size()-1; i >= 0; i--) {
//         if (readQueue.at(i)->_ReturnRegister == reg) return readQueue.at(i);
//     }
//     return NULL;
// }

// InstructionBase *
// LLVMInterface::findParent(std::string line) {
//     //Search queues with return registers for last instance of a node with the same instruction
//     //Check the reservation queue first to ensure we get the last instance if multiple instances exist in queues
//     for(int i = reservation.size()-2; i >= 0; i--) { //Start with the second to last node to avoid linking a node to itself
//         if (reservation.at(i)->_LLVMLine == line) return reservation.at(i);
//     }
//     for(int i = computeQueue.size()-1; i >= 0; i--) {
//         if (computeQueue.at(i)->_LLVMLine == line) return computeQueue.at(i);
//     }
//     for(int i = readQueue.size()-1; i >= 0; i--) {
//         if (readQueue.at(i)->_LLVMLine == line) return readQueue.at(i);
//     }
//     return NULL;
// }

void
LLVMInterface::dumpModule(llvm::Module *M) {
    M->print(llvm::outs(), nullptr);
    for (const llvm::Function &F : *M) {
        for (const llvm::BasicBlock &BB : F) {
            for (const llvm::Instruction &I : BB) {
                I.print(llvm::outs());
            }
        }
    }
}

void
LLVMInterface::constructStaticGraph() {
/*********************************************************************************************
 Constructing the Static CDFG

 Parses LLVM file and creates the CDFG passed to our runtime simulation engine.
*********************************************************************************************/

    bool dbg = debug();
    if (dbg) DPRINTF(LLVMInterface, "Constructing Static Dependency Graph\n");
    // bbList = new std::list<SALAM::BasicBlock*>(); // Create New Basic Block List
    regList = new RegisterList(); // Create New Register List
    typeList = new TypeList(); // Create New User Defined Types List
    Register* alwaysTrue = new Register("alwaysTrue", ((uint64_t) 1));
    Register* alwaysFalse = new Register("alwaysFalse", ((uint64_t) 0));
    regList->addRegister(alwaysTrue);
    regList->addRegister(alwaysFalse);

    // SALAM::ir_parser(filename);

    llvm::StringRef file = filename;
    llvm::LLVMContext context;
    llvm::SMDiagnostic error;

    // Load LLVM IR file
    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileOrErr = llvm::MemoryBuffer::getFileOrSTDIN(file);
    if (std::error_code ec = fileOrErr.getError()) {
        panic(" Error opening input file: %s", ec.message());
    }

    // Load LLVM Module
    llvm::ErrorOr<std::unique_ptr<llvm::Module>> moduleOrErr = llvm::parseIRFile(file, error, context);
    if (std::error_code ec = moduleOrErr.getError()) {
        panic("Error reading Module: %s", ec.message());
    }

    std::unique_ptr<llvm::Module> m(llvm::parseIRFile(file, error, context));
    if(!m) panic("Error reading Module");

    // Construct the LLVM::Value to SALAM::Value map
    uint64_t valueID = 0;
    SALAM::irvmap vmap;
    // Generate SALAM::Functions
    for (auto func_iter = m->begin(); func_iter != m->end(); func_iter++) {
        llvm::Function &func = *func_iter;
        std::shared_ptr<SALAM::Function> sfunc = std::make_shared<SALAM::Function>(valueID);
        values.push_back(sfunc);
        functions.push_back(sfunc);
        vmap.insert(SALAM::irvmaptype(&func, sfunc));
        valueID++;
        // Generate args for SALAM:Functions
        for (auto arg_iter = func.arg_begin(); arg_iter != func.arg_end(); arg_iter++) {
            llvm::Argument &arg = *arg_iter;
            std::shared_ptr<SALAM::Value> sarg = std::make_shared<SALAM::Value>(valueID);
            values.push_back(sarg);
            vmap.insert(SALAM::irvmaptype(&arg, sarg));
            valueID++;
        }
        // Generate SALAM::BasicBlocks
        for (auto bb_iter = func.begin(); bb_iter != func.end(); bb_iter++) {
            llvm::BasicBlock &bb = *bb_iter;
            std::shared_ptr<SALAM::BasicBlock> sbb = std::make_shared<SALAM::BasicBlock>(valueID);
            values.push_back(sbb);
            vmap.insert(SALAM::irvmaptype(&bb, sbb));
            valueID++;
            //Generate SALAM::Instructions
            for (auto inst_iter = bb.begin(); inst_iter != bb.end(); inst_iter++) {
                llvm::Instruction &inst = *inst_iter;
                std::shared_ptr<SALAM::Instruction> sinst = createInstruction(&inst, valueID);
                values.push_back(sinst);
                vmap.insert(SALAM::irvmaptype(&inst, sinst));
                valueID++;
            }
        }
    }

    // Use value map to initialize SALAM::Values
    // Functions will initialize BasicBlocks, which will initialize Instructions
    for (auto func_iter = m->begin(); func_iter != m->end(); func_iter++) {
        llvm::Function &func = *func_iter;
        std::shared_ptr<SALAM::Value> funcval = vmap.find(&func)->second;
        assert(funcval);
        std::shared_ptr<SALAM::Function> sfunc = std::dynamic_pointer_cast<SALAM::Function>(funcval);
        assert(sfunc);
        sfunc->initialize(&func, &vmap, regList);
    }

    panic("Killing Sim");

    hardware = new Hardware(fu_latency, pipelined); // Initialize Hardware Functional Units
    hardware->linkRegList(regList);
    setupStop = std::chrono::high_resolution_clock::now();
    setupTime = std::chrono::duration_cast<std::chrono::duration<double>>(setupStop-timeStart);
}

// SALAM::BasicBlock*
// LLVMInterface::findBB(std::string bbname) {
// /*********************************************************************************************
//  Find Basic Block

//  Iterates through list of known basic block and returns a pointer to passed BB name if found
//  and NULL if the BB does not exist in the BB list.
// *********************************************************************************************/
//     for (auto it = bbList->begin(); it != bbList->end(); ++it) {
//         if ((*it)->Name().compare(bbname) == 0)
//             return (*it);
//     }
//     return NULL;
// }

// SALAM::BasicBlock*
// LLVMInterface::findEntryBB() {
// /*********************************************************************************************
//  Find Entry Basic Block

//  Iterates through list of known basic block and returns a pointer to the first basic block
//  that contains instructions
// *********************************************************************************************/
//     for (auto it = bbList->begin(); it != bbList->end(); ++it) {
//         if (!((*it)->isEmpty()))
//             return (*it);
//     }
//     panic("No entry basic block found!\n");
//     return NULL;
// }

void
LLVMInterface::readCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Read Request
*********************************************************************************************/
    // for (auto i = 0; i < readQueue.size(); i++ ) {
    //     if(readQueue.at(i)->getReq() == req) {
    //         readQueue.at(i)->setResult(req->buffer);
    //         readQueue.at(i)->commit();
    //         readQueue.erase(readQueue.begin() + i);
    //     }
    // }
}

void
LLVMInterface::writeCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Write Request
*********************************************************************************************/
   // for (auto i = 0; i < writeQueue.size(); i++ ) {
   //      if(writeQueue.at(i)->getReq() == req) {
   //          writeQueue.at(i)->commit();
   //          writeQueue.erase(writeQueue.begin() + i);
   //      }
   //  }
}

void
LLVMInterface::initialize() {
/*********************************************************************************************
 Initialize the Runtime Engine

 Calls function that constructs the basic block list, initializes the reservation table and
 read, write, and compute queues. Set all data collection variables to zero.
*********************************************************************************************/
    timeStart = std::chrono::high_resolution_clock::now();
    if (debug()) DPRINTF(LLVMInterface, "Initializing LLVM Runtime Engine!\n");
    constructStaticGraph();
    if (debug()) DPRINTF(LLVMInterface, "Initializing Reservation Table!\n");
    if (debug()) DPRINTF(LLVMInterface, "Initializing readQueue Queue!\n");
    if (debug()) DPRINTF(LLVMInterface, "Initializing writeQueue Queue!\n");
    if (debug()) DPRINTF(LLVMInterface, "Initializing computeQueue List!\n");
    if (debug()) DPRINTF(LLVMInterface, "\n%s\n%s\n%s\n",
            "*******************************************************************************",
            "*                 Begin Runtime Simulation Computation Engine                 *",
            "*******************************************************************************");
    running = true;
    cycle = 0;
    stalls = 0;
    hardware->updateLimit(counter_units,
                                int_adder_units,
                                int_multiply_units,
                                int_shifter_units,
                                int_bit_units,
                                fp_sp_adder,
                                fp_dp_adder,
                                fp_sp_multiply,
                                fp_sp_division,
                                fp_dp_multiply,
                                fp_dp_division,
                                compare,
                                gep,
                                conversion);
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
LLVMInterface::occupancy() {
        if (loadOpScheduled) {
        if (storeOpScheduled) {
            if (compOpScheduled) hardware->occ_scheduled.loadStoreComp++;
            else hardware->occ_scheduled.loadStore++;
        } else if (compOpScheduled) hardware->occ_scheduled.loadComp++;
        else hardware->occ_scheduled.loadOnly++;
    } else if(storeOpScheduled) {
        if (compOpScheduled) hardware->occ_scheduled.storeComp++;
        else hardware->occ_scheduled.storeOnly++;
    } else if (compOpScheduled) hardware->occ_scheduled.compOnly++;
    else {
        stalls++;
        if(loadInFlight) {
            if(storeInFlight) {
                if(compInFlight) hardware->occ_stalled.loadStoreComp++;
                else hardware->occ_stalled.loadStore++;
            } else if (compInFlight) hardware->occ_stalled.loadComp++;
            else hardware->occ_stalled.loadOnly++;
        } else if(storeInFlight) {
            if(compInFlight) hardware->occ_stalled.storeComp++;
            else hardware->occ_stalled.storeOnly++;
        } else if (compInFlight) hardware->occ_stalled.compOnly++;
    }
}

void
LLVMInterface::finalize() {
    // Simulation Times
    running = false;
    simStop = std::chrono::high_resolution_clock::now();
    simTime = std::chrono::duration_cast<std::chrono::duration<double>>(simStop-timeStart);

    printPerformanceResults();
    // comm->printResults() ?
    hardware->printResults();
    hardware->finalize();
    comm->finish();
}

void
LLVMInterface::printPerformanceResults() {
    Tick cycle_time = clock_period/1000;
/*********************************************************************************************
 Prints usage statistics of how many times each instruction was accessed during runtime
*********************************************************************************************/
    std::cout << "********************************************************************************" << std::endl;
    std::cout << name() << std::endl;
    std::cout << "   ========= Performance Analysis =============" << std::endl;
    std::cout << "   Setup Time:                      " << (double)(setupTime.count()) << "seconds" << std::endl;
    std::cout << "   Simulation Time:                 " << (double)(simTime.count()) << "seconds" << std::endl;
    std::cout << "   System Clock:                    " << 1.0/(cycle_time) << "GHz" << std::endl;
    std::cout << "   Transistor Latency:              " << fu_latency << "ns" << std::endl;
    std::cout << "   Runtime:                         " << cycle << " cycles" << std::endl;
    std::cout << "   Runtime:                         " << (cycle*cycle_time*(1e-3)) << " us" << std::endl;
    std::cout << "   Stalls:                          " << stalls << " cycles" << std::endl;
    std::cout << "   Executed Nodes:                  " << (cycle-stalls-1) << " cycles" << std::endl;
    std::cout << std::endl;
}
    /*
    // getCactiResults(int cache_size, int word_size, int ports, int type)
    // SPM cache_type = 0
    uca_org_t cacti_result_spm_opt = pwrUtil->getCactiResults(regList->count()*512, (read_bus_width/8), (read_ports+write_ports), 0);
    uca_org_t cacti_result_spm_leakage = pwrUtil->getCactiResults(spm_size, (read_bus_width/8), (read_ports+write_ports), 0);
    //uca_org_t cacti_result_spm_dynamic_read = pwrUtil->getCactiResults((int) (memory_loads*(read_bus_width/8)), (read_bus_width/8), (read_ports), 0);
    //uca_org_t cacti_result_spm_dynamic_write = pwrUtil->getCactiResults((int) (memory_stores*(read_bus_width/8)), (read_bus_width/8), (write_ports), 0);

    // Cache cache_type = 1
    // uca_org_t cacti_result_cache_leakage = pwrUtil->getCactiResults(cache_size, (read_bus_width/8), cache_ports, 1);
    // uca_org_t cacti_result_cache_dynamic_read = pwrUtil->getCactiResults(dma_loads*(read_bus_width/8), (read_bus_width/8), cache_ports, 1);
    // uca_org_t cacti_result_cache_dynamic_write = pwrUtil->getCactiResults(dma_stores*(read_bus_width/8), (read_bus_width/8), cache_ports, 1);
    double exponential = 1e9; // Units correction
    double leak = 1.0; // Remnant of old units difference
    */

void
LLVMInterface::dumpQueues() {
    // std::cout << "*********************************************************\n"
    //           << "Compute Queue\n"
    //           << "*********************************************************\n";
    // for (auto compute : computeQueue) {
    //     std::cout << compute->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "Read Queue\n"
    //           << "*********************************************************\n";
    // for (auto read : readQueue) {
    //     std::cout << read->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "Write Queue\n"
    //           << "*********************************************************\n";
    // for (auto write : writeQueue) {
    //     std::cout << write->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "Reservation Queue\n"
    //           << "*********************************************************\n";
    // for (auto reserved : reservation) {
    //     std::cout << reserved->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "End of queue dump\n"
    //           << "*********************************************************\n";
}

std::shared_ptr<SALAM::Instruction>
LLVMInterface::createInstruction(llvm::Instruction * inst, uint64_t id) {
    switch(inst->getOpcode()) {
        default:
            return std::make_shared<SALAM::Instruction>(id);
    }
}