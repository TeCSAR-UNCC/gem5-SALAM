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
Instruction::Instruction_Debugger::dumper(Instruction * inst)
{
    if (DTRACE(SALAM_Debug)) {
        if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
        DPRINTF(SALAM_Debug, "| %s | \n\t\t %s %d  \n", 
            "************** Instruction Dump **************",
            "    UID: ", inst->getUID()
        );
        inst->value_dump();
    }
}

void
Instruction::instantiate(llvm::Value *irval,
                         irvmap *irmap,
                         SALAM::valueListTy *valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
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
        // Push back pointers to registers for operands
        // TODO: Maybe remove this 
        opReg.push_back(opval->getReg());
        if(llvm::isa<llvm::PHINode>(inst)) {
            uint64_t phiBB = 0;
            llvm::PHINode * phi = llvm::dyn_cast<llvm::PHINode>(inst);
            llvm::Value * bb = llvm::dyn_cast<llvm::Value>(phi->getIncomingBlock(phiBB));
            mapit = irmap->find(bb);
            opval = mapit->second;
            staticDependencies.push_back(opval);
            ++phiBB;
        }
    }

    DPRINTF(LLVMInterface, "Initialize Value - Instruction::instantiate\n");
    SALAM::Value::initialize(irval, irmap);
}

void
Instruction::signalUsers()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    for (auto user : dynamicUsers) {
        user->getDependencyValue(this);
    }
}

void
Instruction::operandValueFetch(std::shared_ptr<SALAM::Value> val)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    uint64_t fetchedUID = val->getUID();
    std::shared_ptr<SALAM::Register> fetchedRegister = val->getRegister();
    for (auto op : operands) {
        if (op.getUID() == fetchedUID) {
            op.setRegisterValue(fetchedRegister);
            break;
        }
    }
}

void
Instruction::linkOperands(const SALAM::Operand &newOp)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Operand op_copy = newOp;
    operands.push_back(op_copy);
    DPRINTF(Runtime, "Begin Operand Init: [UID = %u]\n", op_copy.getUID());
    operands.back().initOperandReg();

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
    currentCycle = 0;
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
}

bool
Ret::ready()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Ret::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Ret::compute() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Perform computations
    // Store results in temp location
}

bool
Ret::commit()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Ret::getDependencyValue(Instruction *dep) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Rework **
}

void
Ret::reset() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Lock results from temp location
}

// SALAM-Br // --------------------------------------------------------------//
void // Debugging Interface 
Br::dumper() {
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
    currentCycle = 0;
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

/*
void
Br::linkOperands() 
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}
*/

std::shared_ptr<SALAM::BasicBlock>
Br::getTarget() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if(conditional) {
        if(condition->getReg()->getIntData()->isOneValue()) return std::dynamic_pointer_cast<SALAM::BasicBlock>(trueDestination);
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

bool
Br::ready() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Br::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Br::compute()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Perform computations
    // Store results in temp location
}

bool
Br::commit()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Br::getDependencyValue(Instruction *dep) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Rework **
}

void
Br::reset() {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Lock results from temp location
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
    currentCycle = 0;
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

bool
Switch::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Switch::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Switch::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Switch::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Switch::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Switch::reset() {
    // Lock results from temp location
}

// SALAM-Add // -------------------------------------------------------------//
void // Debugging Interface 
Add::dumper() {
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
    currentCycle = 0;
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
}

// do computation using llvm::Type
// llvm::Type *getType() { return irtype; }
// llvm::Type* variable1 = this->getType(); - worked
bool
Add::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Add::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Add::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Add::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Add::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Add::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
}

bool
FAdd::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FAdd::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FAdd::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FAdd::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FAdd::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FAdd::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
Sub::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Sub::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Sub::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Sub::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Sub::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Sub::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FSub::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FSub::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FSub::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FSub::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FSub::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FSub::reset() {
    // Lock results from temp location
}

// SALAM-Mul // -------------------------------------------------------------//
void // Debugging Interface 
Mul::dumper() {
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
    currentCycle = 0;
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
    // ****** //
}

bool
Mul::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Mul::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Mul::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Mul::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Mul::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Mul::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FMul::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FMul::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FMul::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FMul::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FMul::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FMul::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
UDiv::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
UDiv::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
UDiv::compute() {
    // Perform computations
    // Store results in temp location
}

bool
UDiv::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
UDiv::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
UDiv::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
SDiv::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
SDiv::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
SDiv::compute() {
    // Perform computations
    // Store results in temp location
}

bool
SDiv::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
SDiv::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
SDiv::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FDiv::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FDiv::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FDiv::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FDiv::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FDiv::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FDiv::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
URem::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
URem::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
URem::compute() {
    // Perform computations
    // Store results in temp location
}

bool
URem::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
URem::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
URem::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
SRem::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
SRem::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
SRem::compute() {
    // Perform computations
    // Store results in temp location
}

bool
SRem::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
SRem::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
SRem::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FRem::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FRem::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FRem::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FRem::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FRem::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FRem::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
Shl::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Shl::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Shl::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Shl::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Shl::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Shl::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
LShr::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
LShr::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
LShr::compute() {
    // Perform computations
    // Store results in temp location
}

bool
LShr::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
LShr::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
LShr::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
AShr::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
AShr::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
AShr::compute() {
    // Perform computations
    // Store results in temp location
}

bool
AShr::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
AShr::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
AShr::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
And::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
And::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
And::compute() {
    // Perform computations
    // Store results in temp location
}

bool
And::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
And::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
And::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
Or::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Or::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Or::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Or::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Or::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Or::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
Xor::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Xor::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Xor::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Xor::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Xor::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Xor::reset() {
    // Lock results from temp location
}

// SALAM-Load // ------------------------------------------------------------//
void // Debugging Interface 
Load::dumper() {
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
    currentCycle = 0;
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
    // ****** //

}

bool
Load::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Load::launch()
{
    // UNUSED
    return true;
}

void
Load::compute() {
    // UNUSED
}

bool
Load::commit()
{
    reset();
    signalUsers();
    committed = true;
    return true;
}

void
Load::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Load::reset() {
    // Lock results from temp location
}

MemoryRequest *
Load::createMemoryRequest() {
    Addr memAddr = *(operands.front().getPtrRegValue());
    size_t reqLen = operands.front().getSizeInBytes();

    return new MemoryRequest(memAddr, reqLen);
}

// SALAM-Store // -----------------------------------------------------------//
void // Debugging Interface 
Store::dumper() {
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
    currentCycle = 0;
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
    // ****** //
}

bool
Store::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Store::launch()
{
    // UNUSED
    return true;
}

void
Store::compute() {
    // UNUSED
}

bool
Store::commit()
{
    reset();
    signalUsers();
    committed = true;
    return true;
}

MemoryRequest *
Store::createMemoryRequest() {
    Addr memAddr = *(operands.at(1).getPtrRegValue());
    size_t reqLen = operands.front().getSizeInBytes();

    MemoryRequest * req;

    auto dataRegister = operands.at(0).getRegister();
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

void
Store::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Store::reset() {
    // Lock results from temp location
}

// SALAM-GEP // -------------------------------------------------------------//
void // Debugging Interface 
GetElementPtr::dumper() {
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
    currentCycle = 0;
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
    // ****** //
    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    assert(iruser);
    llvm::GetElementPtrInst * GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(irval);
    assert(GEP);


}

bool
GetElementPtr::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
GetElementPtr::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
GetElementPtr::compute() {
    // Perform computations
    // Store results in temp location
}

bool
GetElementPtr::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
GetElementPtr::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
GetElementPtr::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
Trunc::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Trunc::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Trunc::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Trunc::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Trunc::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Trunc::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
ZExt::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
ZExt::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
ZExt::compute() {
    // Perform computations
    // Store results in temp location
}

bool
ZExt::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
ZExt::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
ZExt::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
SExt::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
SExt::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
SExt::compute() {
    // Perform computations
    // Store results in temp location
}

bool
SExt::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
SExt::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
SExt::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FPToUI::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FPToUI::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FPToUI::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FPToUI::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FPToUI::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FPToUI::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FPToSI::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FPToSI::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FPToSI::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FPToSI::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FPToSI::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FPToSI::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
UIToFP::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
UIToFP::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
UIToFP::compute() {
    // Perform computations
    // Store results in temp location
}

bool
UIToFP::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
UIToFP::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
UIToFP::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
SIToFP::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
SIToFP::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
SIToFP::compute() {
    // Perform computations
    // Store results in temp location
}

bool
SIToFP::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
SIToFP::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
SIToFP::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FPTrunc::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FPTrunc::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FPTrunc::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FPTrunc::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FPTrunc::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FPTrunc::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FPExt::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FPExt::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FPExt::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FPExt::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FPExt::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FPExt::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
PtrToInt::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
PtrToInt::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
PtrToInt::compute() {
    // Perform computations
    // Store results in temp location
}

bool
PtrToInt::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
PtrToInt::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
PtrToInt::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
IntToPtr::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
IntToPtr::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
IntToPtr::compute() {
    // Perform computations
    // Store results in temp location
}

bool
IntToPtr::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
IntToPtr::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
IntToPtr::reset() {
    // Lock results from temp location
}

// SALAM-ICmp // ------------------------------------------------------------//
void // Debugging Interface 
ICmp::dumper() {
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
    currentCycle = 0;
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
    // ****** //
}

bool
ICmp::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
ICmp::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
ICmp::compute() {
    // Perform computations
    // Store results in temp location
}

bool
ICmp::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
ICmp::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
ICmp::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
FCmp::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
FCmp::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
FCmp::compute() {
    // Perform computations
    // Store results in temp location
}

bool
FCmp::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
FCmp::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
FCmp::reset() {
    // Lock results from temp location
}

// SALAM-Phi // -------------------------------------------------------------//
void // Debugging Interface 
Phi::dumper() {
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
    currentCycle = 0;
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
    llvm::PHINode * phi = llvm::dyn_cast<llvm::PHINode>(irval);
    assert(phi);
    phiNode args;
    for (int i = 0; i < getStaticDependencies().size();) {
        args.first = getStaticDependencies(i); ++i;
        args.second = std::dynamic_pointer_cast<SALAM::BasicBlock>(getStaticDependencies(i)); ++i;
        this->arguments.push_back(args);
    }
}

bool
Phi::ready(std::shared_ptr<SALAM::BasicBlock> previousBB) {
    if (getDependencyCount() == 0) {
        isready = true;
        setPrevBB(previousBB);

        return true;
    }
    return false;
}

bool
Phi::launch()
{   
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Phi::compute() {
    // Perform computations
    // Store results in temp location
    std::shared_ptr<SALAM::Value> node;
    for (auto const it : arguments) {
        if(previousBB == it.second) node = it.first;
    }
    // node is pointer where value help
}

bool
Phi::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Phi::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Phi::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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
    // ****** //
}

bool
Call::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Call::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Call::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Call::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Call::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Call::reset() {
    // Lock results from temp location
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
    currentCycle = 0;
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

bool
Select::ready() {
    if (getDependencyCount() == 0) {
        isready = true;

        return true;
    }
    return false;
}

bool
Select::launch()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    launched = true;
    compute();
    return commit();
}

void
Select::compute() {
    // Perform computations
    // Store results in temp location
}

bool
Select::commit()
{
    if (getCurrentCycle() == 0) { // Instruction ready to be committed
        reset();
        signalUsers();
        committed = true;
        return true;
    } else {
        currentCycle--;
    }
    return false;
}

void
Select::getDependencyValue(Instruction *dep) {
    // Rework **
}

void
Select::reset() {
    // Lock results from temp location
}


} // namespace SALAM

//---------------------------------------------------------------------------//