//------------------------------------------//
#include "hwacc/llvm_interface.hh"
//------------------------------------------//

LLVMInterface::LLVMInterface(LLVMInterfaceParams *p) :
    ComputeUnit(p),
    filename(p->in_file),
    topName("vadd"),
    //topName(p->top_name),
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
    clock_period(p->clock_period),
    lockstep(p->lockstep_mode) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    typeList = NULL;
    clock_period = clock_period * 1000;
}

std::shared_ptr<SALAM::Value> createClone(const std::shared_ptr<SALAM::Value>& b)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    std::shared_ptr<SALAM::Value> clone = b->clone();
    return clone;
}

/*

// ===== BB Level 

Sams

basicBlockScheduler( std::shared_ptr<SALAM::BasicBlock> )
Br - std::shared_ptr<SALAM::BasicBlock> getTarget()
// Per Active Function
std::list<ActiveFunction> activeFunctions
    // Instruction Sheduling
    - While instructions from BB onto reserve queue (std::list<std::shared_ptr<SALAM::Instruction> > reservation)
        - clone()
        - If unconditional branch
            - Immediately begin scheduling next basic block
            - remove from queue
        - else
            - link dependencies
JS              - findDynamicDeps(std::list<std::shared_ptr<SALAM::Instructions>, std::shared_ptr<SALAM::Instruction>)
                - only parse queue once for each instruction until all dependencies are found
                - include self in dependency list
                    - Register dynamicUser/dynamicDependencies std::deque<std::shared_ptr<SALAM::Instructon> >
*/
void
LLVMInterface::ActiveFunction::scheduleBB(std::shared_ptr<SALAM::BasicBlock> bb)
{
    if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else DPRINTFR(Runtime,"|---[Schedule BB - UID:%i ]\n", bb->getUID());
    auto instruction_list = *(bb->Instructions());
    for (auto inst : instruction_list) {
        std::shared_ptr<SALAM::Instruction> clone_inst = inst->clone();
        DPRINTFR(Runtime, "\t\t Instruction Cloned [UID: %d] \n", inst->getUID());
        if (clone_inst->isBr()) {
            DPRINTFR(Runtime, "\t\t Branch Instruction Found\n");
            auto branch = std::dynamic_pointer_cast<SALAM::Br>(clone_inst);
            if (branch && !(branch->isConditional())) {
                DPRINTFR(Runtime, "\t\t Unconditional Branch, Scheduling Next BB\n");
                previousBB = bb;
                scheduleBB(branch->getTarget());
            }
        } else {
            if (clone_inst->isPhi()) {
                DPRINTFR(Runtime, "\t\t Phi Instruction Found\n");
                auto phi = std::dynamic_pointer_cast<SALAM::Phi>(clone_inst);
                if (phi) phi->setPrevBB(previousBB);
            }
            findDynamicDeps(clone_inst);
            reservation.push_back(clone_inst);
        }
    }
    previousBB = bb;
}
/*
// ===== Runtime Queue Level - tick()

        During Runtime

        // First
        // (std::list<std::shared_ptr<SALAM::Instruction> > computeQueue)
        - bool commit()
            - if(cycleCount()) // Completed its cycle count
                - // 0 cycles commit immediately 
                - // Perform computation
                - // Set return register with result
                - // Signal users
                - Instruction alerts users its ready, user reads from register when ready
                - Removes itself from dependencies list
                - return true
            - else
                - cycleCount++
                - return false

        // Second
        // std::list<std::shared_ptr<SALAM::Instruction> > reservation
        
Sam      - bool ready(return specific) // checks dependencies, return true if satisfied 


JS       - bool ready() // checks dependencies, return true if satisfied 

        // Make special case for return, queue must also be empty 

        - if(ready())
            // Return true if no dependencies remain
            - When dependencies list has no elements other than self
            - if (launch())
                - // finished
                - return true
            - else
                - Move to computeQueue
                - return false
        - else
            // Do nothing

        - bool launch()
            - Sam - Special Cases
                - // Call Instructions
                    
                - // Load 

                - // Store

                - // Conditional Terminator
                    - BB getTarget()

                - // Return Instruction

            - JS - // Anything Else
            - Performs computation
            //Internally calls commit
            - return commit();
*/

void
LLVMInterface::ActiveFunction::processQueues()
{
    if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else DPRINTFR(Runtime,"|-------[Process Queues]\n");
    // First pass, computeQueue is empty 
    for (auto active_inst : computeQueue) {
        DPRINTFR(Runtime, "\t\t Compute Instruction: %s - UID[%i]\n", llvm::Instruction::getOpcodeName(active_inst->getOpode()), active_inst->getUID());
        if(active_inst->commit()) active_inst->reset();
    }
    if (canReturn()) {
        // Handle function return
        DPRINTFR(Runtime, "[[Function Return]]\n\n");
    } else {
        for (auto it = reservation.begin(); it != reservation.end();) {
            DPRINTFR(Runtime, "\n\t\t %s \n\t\t %s%s%s%d%s \n",
                " |-[Reserve Queue]--------------", 
                " | Instruction: ", llvm::Instruction::getOpcodeName((*it)->getOpode()),
                " | UID[", (*it)->getUID(), "]"
                );
            if ((*it)->isReturn() == false) {
                if ((*it)->ready()) {
                    (*it)->launch();
                    if ((*it)->isLoad()) {
                        auto memReq = (*it)->createMemoryRequest();
                        readQueue.insert({memReq, (*it)});
                    } else if ((*it)->isStore()) {
                        auto memReq = (*it)->createMemoryRequest();
                        writeQueue.insert({memReq, (*it)});
                    } else if ((*it)->isTerminator()) {
                        scheduleBB((*it)->getTarget());
                    } else if ((*it)->isCommitted() == false) {
                        DPRINTFR(Runtime, "\t\t  | Added to Compute Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*it)->getOpode()), (*it)->getUID());
                        computeQueue.push_back(*it);
                    }  else if ((*it)->isCommitted()) {
                        DPRINTFR(Runtime, "\t\t  | Instruction Committed: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*it)->getOpode()), (*it)->getUID());
                        (*it)->reset();
                    } else {
                        panic("Unknown Scheduler Argument!");
                    }
                    DPRINTFR(Runtime, "\t\t  |-Erase From Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*it)->getOpode()), (*it)->getUID());
                    it = reservation.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }
}



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
void
LLVMInterface::tick()
{
        if (DTRACE(Step)) {
        char response;
        do
        {
            std::cout << '\n' << "Press enter key to continue, or q then enter key to exit...";
            response = std::cin.get();
            if (response == 'q') panic("Exit Program");
        } while (response != '\n');
    }

    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    DPRINTF(LLVMInterface, "\n%s\n%s %d\n%s\n",
        "********************************************************************************",
        "   Cycle", cycle,
        "********************************************************************************");
    cycle++;
    // comm->refreshMemPorts(); // Deprecated

    // Process Queues in Active Functions
    if (activeFunctions.empty()) {
        // We are finished executing all functions. Signal completion to the CommInterface
        finalize();
    } else {
        for (auto it = activeFunctions.begin(); it != activeFunctions.end(); ++it) {
            it->processQueues();
        }
    }

    //////////////// Schedule Next Cycle ////////////////////////
    if (running && !tickEvent.scheduled()) {
        schedule(tickEvent, curTick() + clock_period);// * process_delay);
    }
}


/*********************************************************************************************
- findDynamicDeps(std::list<std::shared_ptr<SALAM::Instructions>, std::shared_ptr<SALAM::Instruction>)
- only parse queue once for each instruction until all dependencies are found
- include self in dependency list
- Register dynamicUser/dynamicDependencies std::deque<std::shared_ptr<SALAM::Instructon> >
*********************************************************************************************/
void // Add third argument, previous BB
LLVMInterface::ActiveFunction::findDynamicDeps(std::shared_ptr<SALAM::Instruction> inst)
{
    if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // The list of UIDs for any dependencies we want to find
    std::vector<uint64_t> dep_uids;
    std::map<uint64_t , std::shared_ptr<SALAM::Value>> dependencies;
    // An instruction is a runtime dependency for itself since multiple
    // instances of the same instruction shouldn't execute simultaneously
    dep_uids.push_back(inst->getUID());
    // dependencies.insert(std::pair<uint64_t, std::shared_ptr<SALAM::Value>>(inst->getUID(), std::dynamic_pointer_cast<SALAM::Value>(inst)));

    // Fetch the UIDs of static operands
    for (auto static_dep : inst->getStaticDependencies()) {
        dep_uids.push_back(static_dep->getUID());
        dependencies.insert(std::pair<uint64_t, std::shared_ptr<SALAM::Value>>(static_dep->getUID(), static_dep));
        // Inintialize the operands here
        // +++ call constructor for Operand
        //      copy constructor for value
        //      with an extra register, and functions for that register
        // SALAM::Operand newOp(static_dep->getUID());
        SALAM::Operand *newOp = new SALAM::Operand(*(static_dep.get()));
        inst->linkOperands(*newOp);
        // Ordered the same as static dependencies
        // Also matches order of IR 
    }

    // Find dependencies currently in queues

    // Reverse search the reservation queue because we want to link only the last instance of each dep
    for (auto it = reservation.rbegin(); it != reservation.rend(); ++it) {
        auto queued_inst = *it;
        // Look at each instruction in runtime queue once
        for (auto dep : dep_uids) {
            // Check if any of the instruction to be scheduled dependencies match the current instruction from queue
            if (queued_inst->getUID() == dep) {
                // If dependency found, create two way link
                inst->addRuntimeDependency(queued_inst);
                queued_inst->addRuntimeUser(inst);
                // Remove UID if dependency exists
                dependencies.erase(dep);
            }
        }
    }

    // The other queues do not need to be reverse-searched since only 1 instance of any instruction can exist in them
    // Check the compute queue
    for (auto queued_inst : computeQueue) {
        // Look at each instruction in runtime queue once
        for (auto dep : dep_uids) {
            // Check if any of the instruction to be scheduled dependencies match the current instruction from queue
            if (queued_inst->getUID() == dep) {
                // If dependency found, create two way link
                inst->addRuntimeDependency(queued_inst);
                queued_inst->addRuntimeUser(inst);
                // Remove UID if dependency exists
                dependencies.erase(dep);
            }
        }
    }
    // Check the memory read queue
    for (auto queued_read : readQueue) {
        auto queued_inst = queued_read.second;
        // Look at each instruction in runtime queue once
        for (auto dep : dep_uids) {
            // Check if any of the instruction to be scheduled dependencies match the current instruction from queue
            if (queued_inst->getUID() == dep) {
                // If dependency found, create two way link
                inst->addRuntimeDependency(queued_inst);
                queued_inst->addRuntimeUser(inst);
                // Remove UID if dependency exists
                dependencies.erase(dep);
            }
        }
    }
    // Check the memory write queue
    for (auto queued_write : writeQueue) {
        auto queued_inst = queued_write.second;
        // Look at each instruction in runtime queue once
        for (auto dep : dep_uids) {
            // Check if any of the instruction to be scheduled dependencies match the current instruction from queue
            if (queued_inst->getUID() == dep) {
                // If dependency found, create two way link
                inst->addRuntimeDependency(queued_inst);
                queued_inst->addRuntimeUser(inst);
                // Remove UID if dependency exists
                dependencies.erase(dep);
            }
        }
    }

    // Fetch values for resolved dependencies, static elements, and immediate values
    if (!dependencies.empty()) {
        for (auto resolved : dependencies) {
            // If this dependency exists, then lock value into operand
            inst->setOperandValue(resolved.first);
        }
    }
}

void
LLVMInterface::dumpModule(llvm::Module *M) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
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

    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (dbg) DPRINTF(LLVMInterface, "Constructing Static Dependency Graph\n");
    // bbList = new std::list<SALAM::BasicBlock*>(); // Create New Basic Block List
    typeList = new TypeList(); // Create New User Defined Types List

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
    // Generate SALAM::Values for llvm::GlobalVariables
    DPRINTF(LLVMInterface, "Instantiate SALAM::GlobalConstants\n");
    for (auto glob_iter = m->global_begin(); glob_iter != m->global_end(); glob_iter++) {
        llvm::GlobalVariable &glb = *glob_iter;
        std::shared_ptr<SALAM::GlobalConstant> sglb = std::make_shared<SALAM::GlobalConstant>(valueID);
        values.push_back(sglb);
        vmap.insert(SALAM::irvmaptype(&glb, sglb));
        valueID++;
    }
    // Generate SALAM::Functions
    DPRINTF(LLVMInterface, "Instantiate SALAM::Functions\n");
    for (auto func_iter = m->begin(); func_iter != m->end(); func_iter++) {
        llvm::Function &func = *func_iter;
        std::shared_ptr<SALAM::Function> sfunc = std::make_shared<SALAM::Function>(valueID);
        values.push_back(sfunc);
        functions.push_back(sfunc);
        vmap.insert(SALAM::irvmaptype(&func, sfunc));
        valueID++;
        // Generate args for SALAM:Functions
        DPRINTF(LLVMInterface, "Instantiate SALAM::Functions::Arguments\n");
        for (auto arg_iter = func.arg_begin(); arg_iter != func.arg_end(); arg_iter++) {
            llvm::Argument &arg = *arg_iter;
            std::shared_ptr<SALAM::Argument> sarg = std::make_shared<SALAM::Argument>(valueID);
            values.push_back(sarg);
            vmap.insert(SALAM::irvmaptype(&arg, sarg));
            valueID++;
        }
        // Generate SALAM::BasicBlocks
        DPRINTF(LLVMInterface, "Instantiate SALAM::Functions::BasicBlocks\n");
        for (auto bb_iter = func.begin(); bb_iter != func.end(); bb_iter++) {
            llvm::BasicBlock &bb = *bb_iter;
            std::shared_ptr<SALAM::BasicBlock> sbb = std::make_shared<SALAM::BasicBlock>(valueID);
            values.push_back(sbb);
            vmap.insert(SALAM::irvmaptype(&bb, sbb));
            valueID++;
            //Generate SALAM::Instructions
            DPRINTF(LLVMInterface, "Instantiate SALAM::Functions::BasicBlocks::Instructions\n");
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
    DPRINTF(LLVMInterface, "Initialize SALAM::GlobalConstants\n");
    for (auto glob_iter = m->global_begin(); glob_iter != m->global_end(); glob_iter++) {
        llvm::GlobalVariable &glb = *glob_iter;
        std::shared_ptr<SALAM::Value> glbval = vmap.find(&glb)->second;
        assert(glbval);
        std::shared_ptr<SALAM::GlobalConstant> sglb = std::dynamic_pointer_cast<SALAM::GlobalConstant>(glbval);
        assert(sglb);
        sglb->initialize(&glb, &vmap, &values);
    }
    // Functions will initialize BasicBlocks, which will initialize Instructions
    DPRINTF(LLVMInterface, "Initialize SALAM::Functions\n");
    for (auto func_iter = m->begin(); func_iter != m->end(); func_iter++) {
        llvm::Function &func = *func_iter;
        std::shared_ptr<SALAM::Value> funcval = vmap.find(&func)->second;
        assert(funcval);
        std::shared_ptr<SALAM::Function> sfunc = std::dynamic_pointer_cast<SALAM::Function>(funcval);
        assert(sfunc);
        sfunc->initialize(&func, &vmap, &values, topName);
    }
}

void
LLVMInterface::readCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Read Request
*********************************************************************************************/
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto it = globalReadQueue.find(req);
    if (it != globalReadQueue.end()) {
        it->second->readCommit(req);
        delete it->first;
        globalReadQueue.erase(it);
    } else {
        panic("Could not find memory request in global read queue!");
    }
}

void
LLVMInterface::ActiveFunction::readCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Read Request
*********************************************************************************************/
    if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto it = readQueue.find(req);
    if (it != readQueue.end()) {
        auto load_inst = it->second;
        uint8_t * readBuff = req->getBuffer();
        load_inst->setRegisterValue(readBuff);
        load_inst->commit();
        readQueue.erase(it);
    } else {
        panic("Could not find memory request in read queue for function %u!", func->getUID());
    }
}

void
LLVMInterface::writeCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Write Request
*********************************************************************************************/
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto it = globalWriteQueue.find(req);
    if (it != globalWriteQueue.end()) {
        it->second->writeCommit(req);
        delete it->first;
        globalWriteQueue.erase(it);
    } else {
        panic("Could not find memory request in global write queue!");
    }
}

void
LLVMInterface::ActiveFunction::writeCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Write Request
*********************************************************************************************/
    if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto it = writeQueue.find(req);
    if (it != writeQueue.end()) {
        it->second->commit();
        writeQueue.erase(it);
    } else {
        panic("Could not find memory request in write queue for function %u!", func->getUID());
    }
}

void
LLVMInterface::initialize() {
/*********************************************************************************************
 Initialize the Runtime Engine

 Calls function that constructs the basic block list, initializes the reservation table and
 read, write, and compute queues. Set all data collection variables to zero.
*********************************************************************************************/
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    DPRINTF(LLVMInterface, "Initializing LLVM Runtime Engine!\n");
    constructStaticGraph();
    DPRINTF(LLVMInterface, "================================================================\n");
    //debug(1);
    launchTopFunction();


    // panic("Kill Simulation");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing Reservation Table!\n");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing readQueue Queue!\n");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing writeQueue Queue!\n");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing computeQueue List!\n");
    if (debug()) DPRINTF(LLVMInterface, "\n%s\n%s\n%s\n",
           "*******************************************************************************",
           "*                 Begin Runtime Simulation Computation Engine                 *",
           "*******************************************************************************");
    running = true;
    cycle = 0;
    stalls = 0;
    tick();
}

void
LLVMInterface::debug(uint64_t flags) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Dump 
    for (auto func_iter = functions.begin(); func_iter != functions.end(); func_iter++) {
        // Function Level
        // (*func_iter)->dump(); 
        for (auto bb_iter = (*func_iter)->getBBList()->begin(); bb_iter != (*func_iter)->getBBList()->end(); bb_iter++) {
            // Basic Block Level
            (*bb_iter)->dump();
            for (auto inst_iter = (*bb_iter)->Instructions()->begin(); inst_iter != (*bb_iter)->Instructions()->end(); inst_iter++) {
                // Instruction Level
                (*inst_iter)->dump();
            }
        }
    }
    // Dump
    // SALAM::Operand test;
    // test.setOp()
}

void
LLVMInterface::startup() {
/*********************************************************************************************
 Initialize communications between gem5 interface and simulator
*********************************************************************************************/
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    comm->registerCompUnit(this);
}

LLVMInterface*
LLVMInterfaceParams::create() {
/*********************************************************************************************
 Create new interface between the llvm IR and our simulation engine
*********************************************************************************************/
    if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return new LLVMInterface(this);
}

void
LLVMInterface::finalize() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Simulation Times
    comm->finish();
}

void
LLVMInterface::printPerformanceResults() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
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
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
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

void
LLVMInterface::launchFunction(std::shared_ptr<SALAM::Function> callee,
                              std::shared_ptr<SALAM::Instruction> caller) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Add the callee to our list of active functions
    activeFunctions.push_back(ActiveFunction(this, callee, caller));
    activeFunctions.back().launch();
}

void
LLVMInterface::launchTopFunction() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    for (auto func : functions) {
        if (func->isTop()) {
            // Launch the top level function
            launchFunction(func, nullptr);
            return;
        }
    }
    // Fallback if no function was marked as the top-level
    panic("No function marked as top-level. Set the top_name parameter for your LLVMInterface to the name of the top-level function\n");
}

void LLVMInterface::ActiveFunction::launch() {
    if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Fetch the arguments
    std::vector<std::shared_ptr<SALAM::Value>> funcArgs = *(func->getArguments());
    if (func->isTop()) {
        // We need to fetch argument values from the memory mapped registers
        CommInterface * comm = owner->getCommInterface();
        unsigned argOffset = 0;
        for (auto arg : funcArgs) {
            uint64_t argSizeInBytes = arg->getSizeInBytes();
            uint64_t regValue = comm->getGlobalVar(argOffset, argSizeInBytes);
            arg->setRegisterValue(regValue);
            argOffset += argSizeInBytes;
        }
    } else {
        // We need to fetch argument values from the calling function
        panic("Handling of subfunction calls is not yet supported");
    }
    // Schedule the first BB
    scheduleBB(func->entry());
}

std::shared_ptr<SALAM::Instruction>
LLVMInterface::createInstruction(llvm::Instruction * inst, uint64_t id) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    uint64_t OpCode = inst->Instruction::getOpcode();
    if (DTRACE(Trace)) DPRINTF(LLVMInterface, "Switch OpCode [%d]\n", OpCode);
    switch(OpCode) {
        case llvm::Instruction::Ret : return SALAM::createRetInst(id, OpCode, cycles->ret_inst); break;
        case llvm::Instruction::Br: return SALAM::createBrInst(id, OpCode, cycles->br_inst); break;
        case llvm::Instruction::Switch: return SALAM::createSwitchInst(id, OpCode, cycles->switch_inst); break;
        case llvm::Instruction::Add: return SALAM::createAddInst(id, OpCode, cycles->add_inst); break;
        case llvm::Instruction::FAdd: return SALAM::createFAddInst(id, OpCode, cycles->fadd_inst); break;
        case llvm::Instruction::Sub: return SALAM::createSubInst(id, OpCode, cycles->sub_inst); break;
        case llvm::Instruction::FSub: return SALAM::createFSubInst(id, OpCode, cycles->fsub_inst); break;
        case llvm::Instruction::Mul: return SALAM::createMulInst(id, OpCode, cycles->mul_inst); break;
        case llvm::Instruction::FMul: return SALAM::createFMulInst(id, OpCode, cycles->fmul_inst); break;
        case llvm::Instruction::UDiv: return SALAM::createUDivInst(id, OpCode, cycles->udiv_inst); break;
        case llvm::Instruction::SDiv: return SALAM::createSDivInst(id, OpCode, cycles->sdiv_inst); break;
        case llvm::Instruction::FDiv: return SALAM::createFDivInst(id, OpCode, cycles->fdiv_inst); break;
        case llvm::Instruction::URem: return SALAM::createURemInst(id, OpCode, cycles->urem_inst); break;
        case llvm::Instruction::SRem: return SALAM::createSRemInst(id, OpCode, cycles->srem_inst); break;
        case llvm::Instruction::FRem: return SALAM::createFRemInst(id, OpCode, cycles->frem_inst); break;
        case llvm::Instruction::Shl: return SALAM::createShlInst(id, OpCode, cycles->shl_inst); break;
        case llvm::Instruction::LShr: return SALAM::createLShrInst(id, OpCode, cycles->lshr_inst); break;
        case llvm::Instruction::AShr: return SALAM::createAShrInst(id, OpCode, cycles->ashr_inst); break;
        case llvm::Instruction::And: return SALAM::createAndInst(id, OpCode, cycles->and_inst); break;
        case llvm::Instruction::Or: return SALAM::createOrInst(id, OpCode, cycles->or_inst); break;
        case llvm::Instruction::Xor: return SALAM::createXorInst(id, OpCode, cycles->xor_inst); break;
        case llvm::Instruction::Load: return SALAM::createLoadInst(id, OpCode, cycles->load_inst); break;
        case llvm::Instruction::Store: return SALAM::createStoreInst(id, OpCode, cycles->store_inst); break; 
        case llvm::Instruction::GetElementPtr : return SALAM::createGetElementPtrInst(id, OpCode, cycles->gep_inst); break;
        case llvm::Instruction::Trunc: return SALAM::createTruncInst(id, OpCode, cycles->trunc_inst); break;
        case llvm::Instruction::ZExt: return SALAM::createZExtInst(id, OpCode, cycles->zext_inst); break;
        case llvm::Instruction::SExt: return SALAM::createSExtInst(id, OpCode, cycles->sext_inst); break;
        case llvm::Instruction::FPToUI: return SALAM::createFPToUIInst(id, OpCode, cycles->fptoui_inst); break;
        case llvm::Instruction::FPToSI: return SALAM::createFPToSIInst(id, OpCode, cycles->fptosi_inst); break;
        case llvm::Instruction::UIToFP: return SALAM::createUIToFPInst(id, OpCode, cycles->uitofp_inst); break;
        case llvm::Instruction::SIToFP: return SALAM::createSIToFPInst(id, OpCode, cycles->uitofp_inst); break; // FIX
        case llvm::Instruction::FPTrunc: return SALAM::createFPTruncInst(id, OpCode, cycles->fptrunc_inst); break;
        case llvm::Instruction::FPExt: return SALAM::createFPExtInst(id, OpCode, cycles->fpext_inst); break;
        case llvm::Instruction::PtrToInt: return SALAM::createPtrToIntInst(id, OpCode, cycles->ptrtoint_inst); break;
        case llvm::Instruction::IntToPtr: return SALAM::createIntToPtrInst(id, OpCode, cycles->inttoptr_inst); break;
        case llvm::Instruction::ICmp: return SALAM::createICmpInst(id, OpCode, cycles->icmp_inst); break;
        case llvm::Instruction::FCmp: return SALAM::createFCmpInst(id, OpCode, cycles->fcmp_inst); break;
        case llvm::Instruction::PHI: return SALAM::createPHIInst(id, OpCode, cycles->phi_inst); break;
        case llvm::Instruction::Call: return SALAM::createCallInst(id, OpCode, cycles->call_inst); break;
        case llvm::Instruction::Select: return SALAM::createSelectInst(id, OpCode, cycles->select_inst); break;
        default: // return SALAM::createBadInst(id); break;
            return std::make_shared<SALAM::Instruction>(id);
    }
}