#include "instruction.hh"

namespace SALAM
{

//---------------------------------------------------------------------------//
//--------- Instruction Base Class ------------------------------------------//
//---------------------------------------------------------------------------//


SALAM::Instruction::Instruction(uint64_t id) :
                         Value(id)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbg = true;
        this->inst_dbg = new Instruction_Debugger();
    }
    currentCycle = 0;
}

Instruction::Instruction(uint64_t id,
                         uint64_t OpCode) :
                         Value(id),
                         llvmOpCode(OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbg = true;
        this->inst_dbg = new Instruction_Debugger();
    }
    currentCycle = 0;
}

Instruction::Instruction(uint64_t id,
                         uint64_t OpCode,
                         uint64_t cycles) :
                         Value(id),
                         llvmOpCode(OpCode),
                         cycleCount(cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbg = true;
        this->inst_dbg = new Instruction_Debugger();
    }
    currentCycle = 0;
}

Instruction::~Instruction() 
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace Deleted: %s \n", __PRETTY_FUNCTION__);
    //if (DTRACE(SALAM_Debug)) delete inst_dbg;
}

Instruction::Instruction_Debugger::Instruction_Debugger()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

void
Instruction::Instruction_Debugger::dumper(Instruction *inst)
{
    if (DTRACE(SALAM_Debug)) {
        if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
        DPRINTF(SALAM_Debug, "%s \n\t\t %s%d \n", 
            "|-(Instruction Base) ",
            " | UID: ", inst->getUID()
        );
        inst->value_dump();
    }
}

void
Instruction::initialize(llvm::Value *irval,
                         irvmap *irmap,
                         SALAM::valueListTy *valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    DPRINTF(LLVMInterface, "Initialize Value - Instruction::initialize\n");
    SALAM::Value::initialize(irval, irmap);
    // Fetch the operands of the instruction
    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    llvm::Instruction * inst = llvm::dyn_cast<llvm::Instruction>(irval);
    assert(iruser);
    assert(inst);
    for (auto const op : iruser->operand_values()) {
        auto mapit = irmap->find(op);
        if(dbg) {
            std::cout << "| Operand Found: ";
            op->printAsOperand(llvm::errs());
            llvm::errs() << "\n";
        }
        std::shared_ptr<SALAM::Value> opval;
        if(mapit == irmap->end()) {
            // TODO: Handle constant data and constant expressions
            DPRINTF(LLVMInterface, "Instantiate Operand as Constant Data/Expression\n");
            uint64_t id = valueList->back()->getUID() + 1;
            std::shared_ptr<SALAM::Constant> con = std::make_shared<SALAM::Constant>(id);
            valueList->push_back(con);
            irmap->insert(SALAM::irvmaptype(op, con));
            con->initialize(op, irmap, valueList);
            opval = con;
        } else {
            DPRINTF(LLVMInterface, "Instantiate Operands on Value List\n");
            opval = mapit->second;
        }
        DPRINTF(LLVMInterface, "Link Operand to Static Operands List\n");
        staticDependencies.push_back(opval);
        if(llvm::isa<llvm::PHINode>(inst)) {
            DPRINTF(LLVMInterface, "Phi Node Initiated\n");
            uint64_t phiBB = 0;
            llvm::PHINode * phi = llvm::dyn_cast<llvm::PHINode>(inst);
            llvm::Value * bb = llvm::dyn_cast<llvm::Value>(phi->getIncomingBlock(phiBB));
            mapit = irmap->find(bb);
            opval = mapit->second;
            staticDependencies.push_back(opval);
            ++phiBB;
        } else if(llvm::isa<llvm::CmpInst>(inst)) {
            DPRINTF(LLVMInterface, "Compare Instruction Initiated\n");
        }
    }
}

void
Instruction::signalUsers()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++signalUsers()\n");
    uint64_t count = 0;
    for (auto user : dynamicUsers) {
        DPRINTF(Runtime, "|| User[%i] =============\n", count);
        if(DTRACE(SALAM_Debug)) {
            user->dump();
            user->value_dump();
        }
        user->setOperandValue(this);
        user->removeDynamicDependency(count);
        count++;
    }
    DPRINTF(Runtime, "||==signalUsers==========\n");
}

bool
Instruction::ready()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++ready()\n");
    DPRINTF(Runtime, "|| Remaining Dependencies: %i \n", getDependencyCount());
    if (getDependencyCount() == 0) {
        isready = true;
        DPRINTF(Runtime, "||==Return: %s\n", isready ? "true" : "false");
        DPRINTF(Runtime, "||==ready=================\n");
        return true;
    } else {
        if(DTRACE(SALAM_Debug)) {
            uint64_t count = 0;
            for (auto deps : getDynamicDependencies()) { 
                DPRINTF(Runtime, "|| -Dep[%i] = UID[%i]\n", count, getDynamicDependencies(count)->getUID());
                count++; 
            }
        }
    }
    DPRINTF(Runtime, "||==Return: %s\n", isready ? "true" : "false");
    DPRINTF(Runtime, "||==ready=================\n");
    return false;
}

bool
Instruction::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++launch()\n");
    launched = true;
    if (getCycleCount() == 0) { // Instruction ready to be committed
        DPRINTF(Runtime, "||  0 Cycle Instruction\n");
        compute();
        commit();
    } else {
        currentCycle++;
        compute();
    }
    DPRINTF(Runtime, "||==Return: %s\n", isCommitted() ? "true" : "false");
    DPRINTF(Runtime, "||==launch================\n");
    return isCommitted();
}

bool
Instruction::commit()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++commit()\n");
    DPRINTF(Runtime, "||  Current Cycle: %i\n", getCurrentCycle());
    if (getCurrentCycle() == getCycleCount()) { // Instruction ready to be committed
        signalUsers();
        committed = true;
        DPRINTF(Runtime, "||==Return: %s\n", committed ? "true" : "false");
        DPRINTF(Runtime, "||==commit================\n");
        return true;
    } else {
        DPRINTF(Runtime, "||  Remaining Cycles: %i\n", getCycleCount() - getCurrentCycle());
        currentCycle++;
    }
    DPRINTF(Runtime, "||==Return: %s\n", committed ? "true" : "false");
    DPRINTF(Runtime, "||==commit================\n");
    return false;
}

void
Instruction::setOperandValue(Instruction *dep)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++setOperandValue()\n");
    uint64_t count = 0;
    for (auto ops : operands) {
        if (dep->getUID() == ops.getUID()) {
            DPRINTF(Runtime, "|| Storing Value in Op[%i]\n", count++);
            ops.setRegisterValue(dep->getReg());
            break;
        } else count++;
    }
}

void
Instruction::setOperandValue(uint64_t uid)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||--setOperandValue()\n");
    for (auto op : operands) {
        if (op.getUID() == uid) {
            op.updateOperandRegister();
            break;
        }
    }
}

void
Instruction::reset() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||--reset()\n");
    isready = false;
    launched = false;
    committed = false;
    currentCycle = 0;
    DPRINTF(Runtime, "||==reset=================\n");
}



void
Instruction::linkOperands(const SALAM::Operand &newOp)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Operand op_copy = newOp;
    operands.push_back(op_copy);
}

// SALAM-Ret // -------------------------------------------------------------//
void // Debugging Interface 
Ret::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createRetInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Ret>(id, OpCode, cycles);
}

Ret::Ret(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Ret::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
}

void
Ret::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Perform computations
    // Store results in temp location
}

// SALAM-Br // --------------------------------------------------------------//
void // Debugging Interface 
Br::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createBrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Br>(id, OpCode, cycles);
}

Br::Br(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

std::shared_ptr<SALAM::BasicBlock>
Br::getTarget() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++getTarget()\n");
    if(conditional) {
        if(!condition->getReg()->getIntData()->isOneValue()) return std::dynamic_pointer_cast<SALAM::BasicBlock>(trueDestination);
        else return std::dynamic_pointer_cast<SALAM::BasicBlock>(falseDestination);
    }
    return std::dynamic_pointer_cast<SALAM::BasicBlock>(defaultDestination);
}

void
Br::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    llvm::BranchInst * br = llvm::dyn_cast<llvm::BranchInst>(irval);
    assert(br);
    isConditional(br->isConditional());
    if (isConditional()) {
        condition = getStaticDependencies(0);
        trueDestination = getStaticDependencies(1);
        falseDestination = getStaticDependencies(2);
    } else {
        defaultDestination = getStaticDependencies(0);
    }
}

void
Br::compute()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
}

// SALAM-Switch // ----------------------------------------------------------//
void // Debugging Interface 
Switch::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createSwitchInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Switch>(id, OpCode, cycles);
}

Switch::Switch(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

std::shared_ptr<SALAM::BasicBlock>
Switch::getTarget() {
    
    return nullptr;
}

void
Switch::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    llvm::SwitchInst * switchInst = llvm::dyn_cast<llvm::SwitchInst>(irval);
    assert(switchInst);
    caseArgs newArgs;
    for (int i = 0; i < getStaticDependencies().size();) {
        newArgs.first = getStaticDependencies(i); ++i;
        newArgs.second = getStaticDependencies(i); ++i;
        this->arguments.push_back(newArgs);
    }
}

std::shared_ptr<SALAM::Value>
Switch::destination(int switchVar)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    for (int i = 2; i < this->arguments.size(); ++i) {
        if (this->arguments.at(i).first->getReg()->getIntData()->getSExtValue() == switchVar) return this->arguments.at(i).second;
    }
    return this->defaultDest();
}


void
Switch::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Add // -------------------------------------------------------------//
void // Debugging Interface 
Add::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Add>(id, OpCode, cycles);
}

Add::Add(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Add::initialize(llvm::Value *irval,
                SALAM::irvmap *irmap,
                SALAM::valueListTy *valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
}

void
Add::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    llvm::APInt op1 = operands.at(0).getIntRegValue()->trunc(size);
    llvm::APInt op2 = operands.at(1).getIntRegValue()->trunc(size);
    llvm::APInt result = op1 + op2;
    //llvm::outs() << op1+op2;
    DPRINTF(Runtime, "|| (op1) %s + (op2) %s \n", op1.toString(10, true), op2.toString(10, true));
    DPRINTF(Runtime, "|| Result: %s\n", result.toString(10, true));
    setRegisterValue(result);
}

// SALAM-FAdd // ------------------------------------------------------------//
void // Debugging Interface 
FAdd::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FAdd>(id, OpCode, cycles);
}

FAdd::FAdd(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FAdd::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
}

void
FAdd::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Sub // -------------------------------------------------------------//
void // Debugging Interface 
Sub::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Sub>(id, OpCode, cycles);
}

Sub::Sub(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Sub::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Sub::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-FSub // -------------------------------------------------------------//
void // Debugging Interface 
FSub::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FSub>(id, OpCode, cycles);
}

FSub::FSub(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FSub::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FSub::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Mul // -------------------------------------------------------------//
void // Debugging Interface 
Mul::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Mul>(id, OpCode, cycles);
}

Mul::Mul(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Mul::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Mul::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    llvm::APInt op1 = operands.at(0).getIntRegValue()->trunc(size);
    llvm::APInt op2 = operands.at(1).getIntRegValue()->trunc(size);
    llvm::APInt result = op1 * op2;
    DPRINTF(Runtime, "|| (op1) %s * (op2) %s \n", op1.toString(10, true), op2.toString(10, true));
    DPRINTF(Runtime, "|| Result: %s\n", result.toString(10, true));
    setRegisterValue(result);
}

// SALAM-FMul // ------------------------------------------------------------//
void // Debugging Interface 
FMul::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FMul>(id, OpCode, cycles);
}

FMul::FMul(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FMul::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FMul::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-UDiv // ------------------------------------------------------------//
void // Debugging Interface 
UDiv::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createUDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::UDiv>(id, OpCode, cycles);
}

UDiv::UDiv(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
UDiv::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
UDiv::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-SDiv // ------------------------------------------------------------//
void // Debugging Interface 
SDiv::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createSDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SDiv>(id, OpCode, cycles);
}

SDiv::SDiv(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
SDiv::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SDiv::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-FDiv // ------------------------------------------------------------//
void // Debugging Interface 
FDiv::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FDiv>(id, OpCode, cycles);
}

FDiv::FDiv(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FDiv::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FDiv::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-URem // ------------------------------------------------------------//
void // Debugging Interface 
URem::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createURemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::URem>(id, OpCode, cycles);
}

URem::URem(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
URem::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
URem::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-SRem // ------------------------------------------------------------//
void // Debugging Interface 
SRem::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createSRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SRem>(id, OpCode, cycles);
}

SRem::SRem(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
SRem::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SRem::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-FRem // ------------------------------------------------------------//
void // Debugging Interface 
FRem::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FRem>(id, OpCode, cycles);
}

FRem::FRem(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FRem::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FRem::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Shl // -------------------------------------------------------------//
void // Debugging Interface 
Shl::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createShlInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Shl>(id, OpCode, cycles);
}

Shl::Shl(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Shl::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Shl::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-LShr // ------------------------------------------------------------//
void // Debugging Interface 
LShr::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createLShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::LShr>(id, OpCode, cycles);
}

LShr::LShr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
LShr::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
LShr::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-AShr // ------------------------------------------------------------//
void // Debugging Interface 
AShr::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createAShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::AShr>(id, OpCode, cycles);
}

AShr::AShr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
AShr::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
AShr::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-And // -------------------------------------------------------------//
void // Debugging Interface 
And::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createAndInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::And>(id, OpCode, cycles);
}

And::And(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
And::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
And::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Or // --------------------------------------------------------------//
void // Debugging Interface 
Or::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createOrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Or>(id, OpCode, cycles);
}

Or::Or(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Or::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Or::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Xor // -------------------------------------------------------------//
void // Debugging Interface 
Xor::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createXorInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Xor>(id, OpCode, cycles);
}

Xor::Xor(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Xor::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Xor::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Load // ------------------------------------------------------------//
void // Debugging Interface 
Load::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createLoadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Load>(id, OpCode, cycles);
}

Load::Load(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Load::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::LoadInst * inst = llvm::dyn_cast<llvm::LoadInst>(irval);
    this->align = inst->getAlignment();
    DPRINTF(SALAM_Debug, "Load Instruction %d \n", inst->getPointerAddressSpace());
}

void
Load::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    // setRegisterValue(10);
}

MemoryRequest *
Load::createMemoryRequest() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++createMemoryRequest()\n");
    Addr memAddr = *(operands.front().getPtrRegValue());
    size_t reqLen = operands.front().getSizeInBytes();
    DPRINTF(Runtime, "|| Addr[%x] Size[%i]\n", memAddr, reqLen);
    return new MemoryRequest(memAddr, reqLen);
}

// SALAM-Store // -----------------------------------------------------------//
void // Debugging Interface 
Store::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createStoreInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Store>(id, OpCode, cycles);
}

Store::Store(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Store::initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::StoreInst * inst = llvm::dyn_cast<llvm::StoreInst>(irval);
    this->align = inst->getAlignment();
    DPRINTF(SALAM_Debug, "Load Instruction %d \n", inst->getPointerAddressSpace());
}

void
Store::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
}

MemoryRequest *
Store::createMemoryRequest() {
    Addr memAddr = *(operands.at(1).getPtrRegValue());
    size_t reqLen = operands.front().getSizeInBytes();

    MemoryRequest * req;

    auto dataRegister = operands.at(0).getReg();
    // Copy data from the register
    if (dataRegister->isPtr()) {
        req = new MemoryRequest(memAddr, dataRegister->getPtrData(), reqLen);
    } else {
        llvm::APInt regAPData;
        if (dataRegister->isInt()) {
            regAPData = *(dataRegister->getIntData());
        } else {
            regAPData = dataRegister->getFloatData()->bitcastToAPInt();
        }
        req = new MemoryRequest(memAddr, regAPData.getRawData(), reqLen);
    }

    return req;
}

// SALAM-GEP // -------------------------------------------------------------//
void // Debugging Interface 
GetElementPtr::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createGetElementPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::GetElementPtr>(id, OpCode, cycles);
}

GetElementPtr::GetElementPtr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
GetElementPtr::initialize(llvm::Value * irval,
                            irvmap * irmap,
                            SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    assert(iruser);
    llvm::GetElementPtrInst * GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(irval);
    assert(GEP);


}

void
GetElementPtr::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    uint64_t op1 = *(operands.at(0).getPtrRegValue());
    llvm::APInt op2 = operands.at(1).getIntRegValue()->trunc(size);
    uint64_t result = op1 + op2.getSExtValue();
    DPRINTF(Runtime, "|| Ptr[%x]  Offset[%s]\n", op1, op2.toString(10, true));
    DPRINTF(Runtime, "|| Result: Addr[%x]\n", result);
    setRegisterValue(result);
}

// SALAM-Trunc // -----------------------------------------------------------//
void // Debugging Interface 
Trunc::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Trunc>(id, OpCode, cycles);
}

Trunc::Trunc(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Trunc::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Trunc::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-ZExt // ------------------------------------------------------------//
void // Debugging Interface 
ZExt::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createZExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::ZExt>(id, OpCode, cycles);
}

ZExt::ZExt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
ZExt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
ZExt::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-SExt // ------------------------------------------------------------//
void // Debugging Interface 
SExt::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createSExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SExt>(id, OpCode, cycles);
}

SExt::SExt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
SExt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SExt::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-FPToUI // ----------------------------------------------------------//
void // Debugging Interface 
FPToUI::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFPToUIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPToUI>(id, OpCode, cycles);
}

FPToUI::FPToUI(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FPToUI::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPToUI::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-FPToSI // ----------------------------------------------------------//
void // Debugging Interface 
FPToSI::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFPToSIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPToSI>(id, OpCode, cycles);
}

FPToSI::FPToSI(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FPToSI::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPToSI::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-UIToFP // ----------------------------------------------------------//
void // Debugging Interface 
UIToFP::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createUIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::UIToFP>(id, OpCode, cycles);
}

UIToFP::UIToFP(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
UIToFP::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
UIToFP::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-SIToFP // ----------------------------------------------------------//
void // Debugging Interface 
SIToFP::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createSIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SIToFP>(id, OpCode, cycles);
}

SIToFP::SIToFP(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
SIToFP::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SIToFP::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-FPTrunc // ---------------------------------------------------------//
void // Debugging Interface 
FPTrunc::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFPTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPTrunc>(id, OpCode, cycles);
}

FPTrunc::FPTrunc(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FPTrunc::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPTrunc::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-FPExt // -----------------------------------------------------------//
void // Debugging Interface 
FPExt::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFPExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPExt>(id, OpCode, cycles);
}

FPExt::FPExt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FPExt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPExt::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-PtrToInt // --------------------------------------------------------//
void // Debugging Interface 
PtrToInt::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createPtrToIntInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::PtrToInt>(id, OpCode, cycles);
}

PtrToInt::PtrToInt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
PtrToInt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
PtrToInt::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-IntToPtr // --------------------------------------------------------//
void // Debugging Interface 
IntToPtr::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createIntToPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::IntToPtr>(id, OpCode, cycles);
}

IntToPtr::IntToPtr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
IntToPtr::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
IntToPtr::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-ICmp // ------------------------------------------------------------//
void // Debugging Interface 
ICmp::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createICmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::ICmp>(id, OpCode, cycles);
}

ICmp::ICmp(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
ICmp::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::CmpInst * inst = llvm::dyn_cast<llvm::CmpInst>(irval);
    this->predicate = inst->getPredicate();
    DPRINTF(SALAM_Debug, "Integer Comparison Predicate [%i | %s]\n", this->predicate, inst->getPredicateName(inst->getPredicate()).str());
    
}

void
ICmp::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    switch(predicate) 
    {
        case SALAM::Predicate::ICMP_EQ: { setRegisterValue(operands.at(0).getIntRegValue() == operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_NE: { setRegisterValue(operands.at(0).getIntRegValue() !=  operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_UGT: { setRegisterValue(operands.at(0).getIntRegValue() >  operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_UGE: { setRegisterValue(operands.at(0).getIntRegValue() >= operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_ULT: { setRegisterValue(operands.at(0).getIntRegValue() <  operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_ULE: { setRegisterValue(operands.at(0).getIntRegValue() <= operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_SGT: { setRegisterValue(operands.at(0).getIntRegValue() >  operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_SGE: { setRegisterValue(operands.at(0).getIntRegValue() >= operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_SLT: { setRegisterValue(operands.at(0).getIntRegValue() < operands.at(1).getIntRegValue()); break; }
        case SALAM::Predicate::ICMP_SLE: { setRegisterValue(operands.at(0).getIntRegValue() <= operands.at(1).getIntRegValue()); break; }
        default: break;

    }
}

// SALAM-FCmp // ------------------------------------------------------------//
void // Debugging Interface 
FCmp::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createFCmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FCmp>(id, OpCode, cycles);
}

FCmp::FCmp(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
FCmp::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::CmpInst * inst = llvm::dyn_cast<llvm::CmpInst>(irval);
    this->predicate = inst->getPredicate();
    DPRINTF(SALAM_Debug, "Floating-Point Comparison Predicate [%i | %s]\n", this->predicate, inst->getPredicateName(inst->getPredicate()).str());

}

void
FCmp::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Phi // -------------------------------------------------------------//
void // Debugging Interface 
Phi::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n", 
            "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
            " | Opcode: ", conditions.at(0).at(1),
            " | Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createPHIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Phi>(id, OpCode, cycles);
}

Phi::Phi(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Phi::initialize(llvm::Value * irval,
                    irvmap * irmap,
                    SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    llvm::PHINode * phi = llvm::dyn_cast<llvm::PHINode>(irval);
    assert(phi);
    phiArgTy args;
    for (int i = 0; i < Instruction::getStaticDependencies().size();) {
        args.first = Instruction::getStaticDependencies(i); ++i;
        args.second = std::dynamic_pointer_cast<SALAM::BasicBlock>(Instruction::getStaticDependencies(i)); ++i;
        this->phiArgs.push_back(args);
    }
}

void
Phi::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");

    // Perform computations
    // Store results in temp location
    // std::shared_ptr<SALAM::Value> node;
    // for (auto const it : arguments) {
    //     if(previousBB == it.second) node = it.first;
    // }
    // node is pointer where value help
    uint64_t count = 0;
    for (auto ops : operands) {
        DPRINTF(Runtime, "|| Phi Arg[%d]\n", count++);
        setRegisterValue(ops.getReg());
    }
    //setRegisterValue(operands.at(0).getReg());
}

valueListTy
Phi::getStaticDependencies() const {
    valueListTy deps;

    for (auto phiArg : phiArgs) {
        if (phiArg.second == previousBB) deps.push_back(phiArg.first);
    }

    return deps;
}


// SALAM-Call // ------------------------------------------------------------//
void // Debugging Interface 
Call::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createCallInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Instruction>(id, OpCode, cycles);
}

Call::Call(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Call::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Call::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Select // ----------------------------------------------------------//
void // Debugging Interface 
Select::dumper() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n", 
            "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",  
            "    UID: ", conditions.at(0).at(0),
            " Opcode: ", conditions.at(0).at(1),
            "Latency: ", conditions.at(0).at(2), " Cycles"
        );
    }
}

std::shared_ptr<SALAM::Instruction>
createSelectInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Select>(id, OpCode, cycles);
}

Select::Select(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (DTRACE(SALAM_Debug)) {
        this->dbgr = new Debugger();
    }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
Select::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    this->condition = getStaticDependencies(0);
    this->trueValue = getStaticDependencies(1);
    this->falseValue = getStaticDependencies(2);
    // ****** //
}

std::shared_ptr<SALAM::Value>
Select::evaluate() {
    if(condition->getReg()->getIntData()->isOneValue()) return trueValue;
    return falseValue;
}

void
Select::compute() {
    // Perform computations
    // Store results in temp location
}

} // namespace SALAM

//---------------------------------------------------------------------------//