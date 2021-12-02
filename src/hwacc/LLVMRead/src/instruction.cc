#include "instruction.hh"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include "llvm/IR/DataLayout.h"

#include <cmath>

namespace SALAM
{

//---------------------------------------------------------------------------//
//--------- Instruction Base Class ------------------------------------------//
//---------------------------------------------------------------------------//


SALAM::Instruction::Instruction(uint64_t id) :
                         Value(id)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbg = true;
    //     this->inst_dbg = new Instruction_Debugger();
    // }
    currentCycle = 0;
}

SALAM::Instruction::Instruction(uint64_t id,
                         uint64_t OpCode) :
                         Value(id),
                         llvmOpCode(OpCode)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbg = true;
    //     this->inst_dbg = new Instruction_Debugger();
    // }
    currentCycle = 0;
}

SALAM::Instruction::Instruction(uint64_t id,
                         uint64_t OpCode,
                         uint64_t cycles) :
                         Value(id),
                         llvmOpCode(OpCode),
                         cycleCount(cycles)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbg = true;
    //     this->inst_dbg = new Instruction_Debugger();
    // }
    currentCycle = 0;
}

SALAM::Instruction::Instruction(uint64_t id,
                         uint64_t OpCode,
                         uint64_t cycles,
                         uint64_t fu) :
                         Value(id),
                         llvmOpCode(OpCode),
                         cycleCount(cycles),
                         functional_unit(fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbg = true;
    //     this->inst_dbg = new Instruction_Debugger();
    // }
    currentCycle = 0;
}

SALAM::Instruction::~Instruction()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace Deleted: %s \n", __PRETTY_FUNCTION__);
    //// if (DTRACE(SALAM_Debug)) delete inst_dbg;
}

SALAM::Instruction::Instruction_Debugger::Instruction_Debugger()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
}

void
SALAM::Instruction::Instruction_Debugger::dumper(Instruction *inst)
{
    // if (DTRACE(SALAM_Debug)) {
        // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    //     DPRINTF(SALAM_Debug, "%s \n\t\t %s%d \n",
    //         "|-(Instruction Base) ",
    //         " | UID: ", inst->getUID()
    //     );
    //     inst->value_dump();
    // }
}

void
SALAM::Instruction::initialize(llvm::Value *irval,
                         irvmap *irmap,
                         SALAM::valueListTy *valueList)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    DPRINTF(LLVMInterface, "Initialize Value - Instruction::initialize\n");
    SALAM::Value::initialize(irval, irmap);
    // Fetch the operands of the instruction
    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    llvm::Instruction * inst = llvm::dyn_cast<llvm::Instruction>(irval);
    assert(iruser);
    assert(inst);
    uint64_t phiBB = 0;
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
SALAM::Instruction::signalUsers()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++signalUsers()\n");
    uint64_t count = 0;
    for (auto user : dynamicUsers) {
        DPRINTF(Runtime, "|| User[%i] =============\n", count);
        if(DTRACE(SALAM_Debug)) {
            user->dump();
            user->value_dump();
        }
        user->setOperandValue(uid);
        count++;
    }
    DPRINTF(Runtime, "||==signalUsers==========\n");
}

void
SALAM::Instruction::removeDynamicDependency(uint64_t opuid)
{
    auto end = dynamicDependencies.end();
    auto it = dynamicDependencies.find(opuid);
    if (it != end) dynamicDependencies.erase(it);
}

bool
SALAM::Instruction::ready()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++ready()\n");
    DPRINTF(Runtime, "|| Remaining Dependencies: %i \n", getDependencyCount());
    if (getDependencyCount() == 0) {
        isready = true;
        DPRINTF(Runtime, "||==Return: %s\n", isready ? "true" : "false");
        DPRINTF(Runtime, "||==ready=================\n");
        return true;
    } else {
        // if(DTRACE(SALAM_Debug)) {
        //     uint64_t count = 0;
        //     for (auto deps : getDynamicDependencies()) {
        //         DPRINTF(Runtime, "|| -Dep[%i] = UID[%i]\n", count, getDynamicDependencies(count)->getUID());
        //         count++;
        //     }
        // }
    }
    DPRINTF(Runtime, "||==Return: %s\n", isready ? "true" : "false");
    DPRINTF(Runtime, "||==ready=================\n");
    return false;
}

bool
SALAM::Instruction::launch()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++launch()\n");
    if (hasFunctionalUnit()) {
        if(!hw_interface->availableFunctionalUnit(getFunctionalUnit())) {
            return false;
            std::cout << "Waiting on next available FU\n"; 
        } else {
            
        }
    }
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
SALAM::Instruction::commit()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++commit()\n");
    DPRINTF(Runtime, "||  Current Cycle: %i\n", getCurrentCycle());
    if (getCurrentCycle() == getCycleCount()) { // Instruction ready to be committed
        signalUsers();
        committed = true;
        DPRINTF(Runtime, "||==Return: %s\n", committed ? "true" : "false");
        DPRINTF(Runtime, "||==commit================\n");
        if (hasFunctionalUnit()) {
            hw_interface->clearFunctionalUnit(getFunctionalUnit());
        } else {
            // 
        }
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
SALAM::Instruction::setOperandValue(uint64_t opuid)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||--setOperandValue()\n");
    uint64_t count = 0;
    for (auto it = operands.begin(); it != operands.end(); ++it) {
        auto op = *it;
        if (op.getUID() == opuid) {
            DPRINTF(Runtime, "|| Storing Value in Op[%i]\n", count++);
            op.updateOperandRegister();
            //break;
        } else count++;
    }
    removeDynamicDependency(opuid);
}

void
SALAM::Instruction::reset() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||--reset()\n");
    isready = false;
    launched = false;
    committed = false;
    currentCycle = 0;
    DPRINTF(Runtime, "||==reset=================\n");
}

void
SALAM::Instruction::linkOperands(const SALAM::Operand &newOp)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Operand op_copy = newOp;
    operands.push_back(op_copy);
}

//std::deque<uint64_t>
std::vector<uint64_t>
SALAM::Instruction::runtimeInitialize() {
    assert(getDependencyCount() == 0);
    //std::deque<uint64_t> dep_uids;
    std::vector<uint64_t> dep_uids;

    for (auto it = staticDependencies.begin(); it != staticDependencies.end(); ++it) {
        std::shared_ptr<SALAM::Value> static_dependency = *it;
        auto dep_uid = static_dependency->getUID();
        operands.push_back(SALAM::Operand(static_dependency));
        if ((static_dependency->isConstant()) || (static_dependency->isArgument())) {
            operands.back().updateOperandRegister();
        } else {
            dep_uids.push_back(dep_uid);
        }
    }
    // dep_uids.push_back(uid);

    return dep_uids;
}

// SALAM-BadInstruction // --------------------------------------------------//

std::shared_ptr<SALAM::Instruction>
createBadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::BadInstruction>(id, OpCode, cycles, fu);
}

BadInstruction::BadInstruction(uint64_t id,
                               uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
                               Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
BadInstruction::initialize(llvm::Value * irval,
                           irvmap * irmap,
                           SALAM::valueListTy * valueList)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
}

// SALAM-Ret // -------------------------------------------------------------//
void // Debugging Interface
Ret::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createRetInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Ret>(id, OpCode, cycles, fu);
}

Ret::Ret(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
}

void
Ret::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Ret never calls compute. Special handling occurs in the scheduler.
}

// SALAM-Br // --------------------------------------------------------------//
void // Debugging Interface
Br::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createBrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Br>(id, OpCode, cycles, fu);
}

Br::Br(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

std::shared_ptr<SALAM::BasicBlock>
Br::getTarget() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++getTarget()\n");
    DPRINTF(RuntimeCompute, "|| Launching Branch: %s\n", ir_string);
    if(conditional) {
    #if USE_LLVM_AP_VALUES
        if (condition->getIntRegValue().isOneValue()) {
            DPRINTF(RuntimeCompute, "|| Condition: TRUE, Fetching target %s\n",
                trueDestination->getIRStub());
            return trueDestination;
        } else {
            DPRINTF(RuntimeCompute, "|| Condition: FALSE, Fetching target %s\n",
                falseDestination->getIRStub());
            return falseDestination;
        }
    #else
        if(condition->getUIntRegValue() == 1) {
            DPRINTF(RuntimeCompute, "|| Condition: TRUE, Fetching target %s\n",
                trueDestination->getIRStub());
            return trueDestination;
        } else {
            DPRINTF(RuntimeCompute, "|| Condition: FALSE, Fetching target %s\n",
                falseDestination->getIRStub());
            return falseDestination;
        }
    #endif
    }
    DPRINTF(RuntimeCompute, "|| Fetching target %s\n", defaultDestination->getIRStub());
    return defaultDestination;
}

void
Br::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // SALAM::Instruction::initialize(irval, irmap, valueList); // We don't use the normal init fxn
    SALAM::Value::initialize(irval, irmap);
    llvm::BranchInst * br = llvm::dyn_cast<llvm::BranchInst>(irval);
    assert(br);
    isConditional(br->isConditional());
    llvm::Value * defaultDestValue = br->getSuccessor(0);
    auto mapit = irmap->find(defaultDestValue);
    if(mapit == irmap->end()) {
        DPRINTF(Runtime, "ERROR. Could not find default successor for Br in IR map.");
        assert(0);
    } else {
        defaultDestination = std::dynamic_pointer_cast<SALAM::BasicBlock>(mapit->second);
    }
    if (isConditional()) {
        llvm::Value * condValue = br->getCondition();
        mapit = irmap->find(condValue);
        if(mapit == irmap->end()) {
            DPRINTF(Runtime, "ERROR. Could not find condition for Br in IR map.");
            assert(0);
        } else {
            condition = mapit->second;
            staticDependencies.push_back(condition);
            trueDestination = defaultDestination;

            llvm::Value * falseDestValue = br->getSuccessor(1);
            mapit = irmap->find(falseDestValue);
            if(mapit == irmap->end()) {
                DPRINTF(Runtime, "ERROR. Could not find secondary successor for Br in IR map.");
                assert(0);
            } else {
                falseDestination = std::dynamic_pointer_cast<SALAM::BasicBlock>(mapit->second);
            }
        }
    }
}

void
Br::compute()
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    // Br does not use compute. Special handling occurs in the scheduler.
}

// SALAM-Switch // ----------------------------------------------------------//
void // Debugging Interface
Switch::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createSwitchInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Switch>(id, OpCode, cycles, fu);
}

Switch::Switch(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

std::shared_ptr<SALAM::BasicBlock>
Switch::getTarget() {
    DPRINTF(RuntimeCompute, "|| Launching Switch: %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    auto opdata = (operands.front().getIntRegValue());

    for (auto it = cases.begin(); it != cases.end(); ++it) {
        if (it->first->getIntRegValue().eq(opdata)){
            return it->second;
        }
    }
    return defaultDestination;
#else
    auto opdata = operands.front().getSIntRegValue();

    for (auto it = cases.begin(); it != cases.end(); ++it) {
        if (it->first->getSIntRegValue() == opdata){
            return it->second;
        }
    }
    return defaultDestination;
#endif
}

void
Switch::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // SALAM::Instruction::initialize(irval, irmap, valueList);
    SALAM::Value::initialize(irval, irmap);

    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    llvm::Instruction * inst = llvm::dyn_cast<llvm::Instruction>(irval);
    assert(iruser);
    assert(inst);

    SALAM::valueListTy tmpStaticDeps;
    for (auto const op : iruser->operand_values()) {
        auto mapit = irmap->find(op);
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
        tmpStaticDeps.push_back(opval);
    }

    llvm::SwitchInst * switchInst = llvm::dyn_cast<llvm::SwitchInst>(irval);
    assert(switchInst);
    caseArgs newArgs;
    for (int i = 2; i < tmpStaticDeps.size();) {
        newArgs.first = tmpStaticDeps.at(i); ++i;
        newArgs.second = std::dynamic_pointer_cast<SALAM::BasicBlock>(tmpStaticDeps.at(i)); ++i;
        this->cases.push_back(newArgs);
    }

    staticDependencies.push_back(tmpStaticDeps.front());
    defaultDestination = std::dynamic_pointer_cast<SALAM::BasicBlock>(tmpStaticDeps.at(1));
}

// std::shared_ptr<SALAM::Value>
// Switch::destination(int switchVar)
// {
//     // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
//     for (int i = 2; i < this->arguments.size(); ++i) {
//     #if USE_LLVM_AP_VALUES
//         if (this->arguments.at(i).first->getReg()->getIntData()->getSExtValue() == switchVar) return this->arguments.at(i).second;
//     #else
//         if (this->arguments.at(i).first->getSIntRegValue() == switchVar) return this->arguments.at(i).second;
//     #endif
//     }
//     return this->defaultDest();
// }


void
Switch::compute() {
    // Perform computations
    // Store results in temp location
}

// SALAM-Add // -------------------------------------------------------------//
void // Debugging Interface
Add::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Add>(id, OpCode, cycles, fu);
}

Add::Add(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
}

void
Add::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1 + op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s + (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 + op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d + (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-FAdd // ------------------------------------------------------------//
void // Debugging Interface
FAdd::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FAdd>(id, OpCode, cycles, fu);
}

FAdd::FAdd(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
}

void
FAdd::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APFloat op1 = (operands.at(0).getFloatRegValue());
    llvm::APFloat op2 = (operands.at(1).getFloatRegValue());
    llvm::APFloat result = op1 + op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toString(op1str);
    op2.toString(op2str);
    result.toString(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s + (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    uint64_t bitcastResult;
    switch(size) {
        case 32:
        {
            float op1 = operands.at(0).getFloatFromReg();
            float op2 = operands.at(1).getFloatFromReg();
            float result = op1 + op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f + (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        case 64:
        {
            double op1 = operands.at(0).getDoubleFromReg();
            double op2 = operands.at(1).getDoubleFromReg();
            double result = op1 + op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f + (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        default:
        {
            assert(0 && "Unsupported floating point type." &&
                   "Compile with AP values enabled for extended FP support.");
        }
    }
    setRegisterValue(bitcastResult);
#endif
}

// SALAM-Sub // -------------------------------------------------------------//
void // Debugging Interface
Sub::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Sub>(id, OpCode, cycles, fu);
}

Sub::Sub(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Sub::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1 - op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s - (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 - op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d - (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-FSub // -------------------------------------------------------------//
void // Debugging Interface
FSub::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FSub>(id, OpCode, cycles, fu);
}

FSub::FSub(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FSub::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APFloat op1 = (operands.at(0).getFloatRegValue());
    llvm::APFloat op2 = (operands.at(1).getFloatRegValue());
    llvm::APFloat result = op1 - op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toString(op1str);
    op2.toString(op2str);
    result.toString(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s - (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    uint64_t bitcastResult;
    switch(size) {
        case 32:
        {
            float op1 = operands.at(0).getFloatFromReg();
            float op2 = operands.at(1).getFloatFromReg();
            float result = op1 - op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f - (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        case 64:
        {
            double op1 = operands.at(0).getDoubleFromReg();
            double op2 = operands.at(1).getDoubleFromReg();
            double result = op1 - op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f - (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        default:
        {
            assert(0 && "Unsupported floating point type." &&
                   "Compile with AP values enabled for extended FP support.");
        }
    }
    setRegisterValue(bitcastResult);
#endif
}

// SALAM-Mul // -------------------------------------------------------------//
void // Debugging Interface
Mul::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Mul>(id, OpCode, cycles, fu);
}

Mul::Mul(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Mul::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1 * op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s * (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 * op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d * (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-FMul // ------------------------------------------------------------//
void // Debugging Interface
FMul::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FMul>(id, OpCode, cycles, fu);
}

FMul::FMul(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FMul::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APFloat op1 = (operands.at(0).getFloatRegValue());
    llvm::APFloat op2 = (operands.at(1).getFloatRegValue());
    llvm::APFloat result = op1 * op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toString(op1str);
    op2.toString(op2str);
    result.toString(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s * (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    uint64_t bitcastResult;
    switch(size) {
        case 32:
        {
            float op1 = operands.at(0).getFloatFromReg();
            float op2 = operands.at(1).getFloatFromReg();
            float result = op1 * op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f * (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        case 64:
        {
            double op1 = operands.at(0).getDoubleFromReg();
            double op2 = operands.at(1).getDoubleFromReg();
            double result = op1 * op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f * (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        default:
        {
            assert(0 && "Unsupported floating point type." &&
                   "Compile with AP values enabled for extended FP support.");
        }
    }
    setRegisterValue(bitcastResult);
#endif
}

// SALAM-UDiv // ------------------------------------------------------------//
void // Debugging Interface
UDiv::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createUDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::UDiv>(id, OpCode, cycles, fu);
}

UDiv::UDiv(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
UDiv::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1.udiv(op2);
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s / (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 / op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d / (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-SDiv // ------------------------------------------------------------//
void // Debugging Interface
SDiv::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createSDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SDiv>(id, OpCode, cycles, fu);
}

SDiv::SDiv(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SDiv::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1.sdiv(op2);
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringSigned(op1str);
    op2.toStringSigned(op2str);
    result.toStringSigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s / (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    int64_t op1 = operands.at(0).getSIntRegValue();
    int64_t op2 = operands.at(1).getSIntRegValue();
    int64_t result = op1 / op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d / (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
    setRegisterValue((uint64_t)result);
#endif
}

// SALAM-FDiv // ------------------------------------------------------------//
void // Debugging Interface
FDiv::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FDiv>(id, OpCode, cycles, fu);
}

FDiv::FDiv(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FDiv::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APFloat op1 = (operands.at(0).getFloatRegValue());
    llvm::APFloat op2 = (operands.at(1).getFloatRegValue());
    llvm::APFloat result = op1 / op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toString(op1str);
    op2.toString(op2str);
    result.toString(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s / (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    uint64_t bitcastResult;
    switch(size) {
        case 32:
        {
            float op1 = operands.at(0).getFloatFromReg();
            float op2 = operands.at(1).getFloatFromReg();
            float result = op1 / op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f / (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        case 64:
        {
            double op1 = operands.at(0).getDoubleFromReg();
            double op2 = operands.at(1).getDoubleFromReg();
            double result = op1 / op2;
            DPRINTF(RuntimeCompute, "|| (%s) %f / (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        default:
        {
            assert(0 && "Unsupported floating point type." &&
                   "Compile with AP values enabled for extended FP support.");
        }
    }
    setRegisterValue(bitcastResult);
#endif
}

// SALAM-URem // ------------------------------------------------------------//
void // Debugging Interface
URem::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createURemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::URem>(id, OpCode, cycles, fu);
}

URem::URem(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
URem::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1.urem(op2);
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s % (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 % op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d % (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-SRem // ------------------------------------------------------------//
void // Debugging Interface
SRem::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createSRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SRem>(id, OpCode, cycles, fu);
}

SRem::SRem(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SRem::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1.srem(op2);
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringSigned(op1str);
    op2.toStringSigned(op2str);
    result.toStringSigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s % (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    int64_t op1 = operands.at(0).getSIntRegValue();
    int64_t op2 = operands.at(1).getSIntRegValue();
    int64_t result = op1 % op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d % (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
    setRegisterValue((uint64_t)result);
#endif
}

// SALAM-FRem // ------------------------------------------------------------//
void // Debugging Interface
FRem::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FRem>(id, OpCode, cycles, fu);
}

FRem::FRem(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FRem::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APFloat op1 = (operands.at(0).getFloatRegValue());
    llvm::APFloat op2 = (operands.at(1).getFloatRegValue());
    llvm::APFloat result = op1;
    auto err = result.remainder(op2);
    assert(err == llvm::APFloatBase::opStatus::opOK);
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toString(op1str);
    op2.toString(op2str);
    result.toString(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s % (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    uint64_t bitcastResult;
    switch(size) {
        case 32:
        {
            float op1 = operands.at(0).getFloatFromReg();
            float op2 = operands.at(1).getFloatFromReg();
            float result = std::remainderf(op1, op2);
            DPRINTF(RuntimeCompute, "|| (%s) %f % (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        case 64:
        {
            double op1 = operands.at(0).getDoubleFromReg();
            double op2 = operands.at(1).getDoubleFromReg();
            double result = std::remainder(op1, op2);
            DPRINTF(RuntimeCompute, "|| (%s) %f % (%s) %f\n",
                operands.at(0).getIRStub(), op1,
                operands.at(1).getIRStub(), op2);
            DPRINTF(RuntimeCompute, "|| %s = %f\n", ir_stub, result);
            bitcastResult = *(uint64_t *)&result;
            break;
        }
        default:
        {
            assert(0 && "Unsupported floating point type." &&
                   "Compile with AP values enabled for extended FP support.");
        }
    }
    setRegisterValue(bitcastResult);
#endif
}

// SALAM-Shl // -------------------------------------------------------------//
void // Debugging Interface
Shl::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createShlInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Shl>(id, OpCode, cycles, fu);
}

Shl::Shl(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Shl::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1 << op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s << (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 << op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d << (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-LShr // ------------------------------------------------------------//
void // Debugging Interface
LShr::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createLShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::LShr>(id, OpCode, cycles, fu);
}

LShr::LShr(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
LShr::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1.lshr(op2);
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s >> (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 >> op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d >> (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-AShr // ------------------------------------------------------------//
void // Debugging Interface
AShr::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createAShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::AShr>(id, OpCode, cycles, fu);
}

AShr::AShr(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
AShr::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1.ashr(op2);
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringSigned(op1str);
    op2.toStringSigned(op2str);
    result.toStringSigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s >> (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    int64_t op1 = operands.at(0).getSIntRegValue();
    int64_t op2 = operands.at(1).getSIntRegValue();
    int64_t result = op1 >> op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d >> (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
    setRegisterValue((uint64_t)result);
#endif
}

// SALAM-And // -------------------------------------------------------------//
void // Debugging Interface
And::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createAndInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::And>(id, OpCode, cycles, fu);
}

And::And(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
And::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1 & op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s & (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 & op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d & (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-Or // --------------------------------------------------------------//
void // Debugging Interface
Or::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createOrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Or>(id, OpCode, cycles, fu);
}

Or::Or(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Or::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1 | op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s | (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 | op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d | (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-Xor // -------------------------------------------------------------//
void // Debugging Interface
Xor::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createXorInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Xor>(id, OpCode, cycles, fu);
}

Xor::Xor(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Xor::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt op1 = (operands.at(0).getIntRegValue());
    llvm::APInt op2 = (operands.at(1).getIntRegValue());
    llvm::APInt result = op1 ^ op2;
    llvm::SmallString<8> op1str;
    llvm::SmallString<8> op2str;
    llvm::SmallString<8> resstr;
    op1.toStringUnsigned(op1str);
    op2.toStringUnsigned(op2str);
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| (%s) %s ^ (%s) %s \n",
        operands.at(0).getIRStub(), op1str.c_str(),
        operands.at(1).getIRStub(), op2str.c_str());
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    uint64_t op1 = operands.at(0).getUIntRegValue();
    uint64_t op2 = operands.at(1).getUIntRegValue();
    uint64_t result = op1 ^ op2;
    DPRINTF(RuntimeCompute, "|| (%s) %d ^ (%s) %d\n",
        operands.at(0).getIRStub(), op1,
        operands.at(1).getIRStub(), op2);
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-Load // ------------------------------------------------------------//
void // Debugging Interface
Load::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createLoadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Load>(id, OpCode, cycles, fu);
}

Load::Load(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::LoadInst * inst = llvm::dyn_cast<llvm::LoadInst>(irval);
    this->align = inst->getAlignment();
    if (staticDependencies.front()->isGlobalConstant()) loadingInternal = true;
}

void
Load::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    // Load does not use compute normally. Special handling is used in the scheduler.
    // We instead use compute just for debug printout
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, registerDataString());
}

void
Load::loadInternal() {
    DPRINTF(RuntimeCompute, "|| Launching %s\n", ir_string);
    DPRINTF(RuntimeCompute, "|| Loading internal value from %s\n",
        operands.front().getIRString());
    setRegisterValue(operands.front().getOpRegister());
    commit();
}

MemoryRequest *
Load::createMemoryRequest() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++createMemoryRequest()\n");
    Addr memAddr = (operands.front().getPtrRegValue());
    size_t reqLen = getSizeInBytes();
    DPRINTF(RuntimeCompute, "|| Launching %s\n", ir_string);
    DPRINTF(RuntimeCompute, "|| Addr[%x] Size[%i]\n", memAddr, reqLen);
    return new MemoryRequest(memAddr, reqLen);
}

// SALAM-Store // -----------------------------------------------------------//
void // Debugging Interface
Store::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createStoreInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Store>(id, OpCode, cycles, fu);
}

Store::Store(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::StoreInst * inst = llvm::dyn_cast<llvm::StoreInst>(irval);
    this->align = inst->getAlignment();
}

void
Store::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    // Store does not use compute. Special handling is used in the scheduler.
}

MemoryRequest *
Store::createMemoryRequest() {
    Addr memAddr = (operands.at(1).getPtrRegValue());
    size_t reqLen = operands.at(0).getSizeInBytes();

    MemoryRequest * req;

    auto dataRegister = operands.at(0).getOpRegister();
    // Copy data from the register
    if (dataRegister->isPtr()) {
        uint64_t regData = dataRegister->getPtrData();
        req = new MemoryRequest(memAddr, (uint8_t *)&regData, reqLen);
    } else {
    #if USE_LLVM_AP_VALUES
        llvm::APInt regAPData;
        if (dataRegister->isInt()) {
            regAPData = (dataRegister->getIntData());
        } else {
            regAPData = dataRegister->getFloatData().bitcastToAPInt();
        }
        req = new MemoryRequest(memAddr, regAPData.getRawData(), reqLen);
    #else
        uint64_t regData;
        if (dataRegister->isInt()) {
            regData = dataRegister->getIntData();
        } else {
            regData = dataRegister->getFloatData();
        }
        req = new MemoryRequest(memAddr, (uint8_t *)&regData, reqLen);
    #endif
        DPRINTF(RuntimeCompute, "|| Launching %s\n", ir_string);
        DPRINTF(RuntimeCompute, "|| Addr[%x] Size[%i]\n", memAddr, reqLen);
    }

    return req;
}

// SALAM-GEP // -------------------------------------------------------------//
void // Debugging Interface
GetElementPtr::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createGetElementPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::GetElementPtr>(id, OpCode, cycles, fu);
}

GetElementPtr::GetElementPtr(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    assert(iruser);
    llvm::GetElementPtrInst * GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(irval);
    assert(GEP);
    resultElementType = GEP->getResultElementType();
    llvm::DataLayout layout(GEP->getModule());

    if (resultElementType->getTypeID() == llvm::Type::PointerTyID) {
        resultElementSize = 64; //We assume a 64-bit memory address space
    } else {
        resultElementSize = resultElementType->getScalarSizeInBits();
    }
    resultElementSizeInBytes = ((resultElementSize - 1) >> 3) + 1;

    llvm::Type * ElemTy = GEP->getSourceElementType();
    auto it = iruser->operand_values().begin();
    it++;
    std::vector<llvm::Value *> indexValues;
    for ( ; it != iruser->operand_values().end(); it++) {
        indexValues.push_back(*it);
    }
    llvm::ArrayRef<llvm::Value *> Indices = llvm::ArrayRef<llvm::Value *>(indexValues);
    llvm::generic_gep_type_iterator<llvm::Value* const*>
        GTI = gep_type_begin(ElemTy, Indices),
        GTE = gep_type_end(ElemTy, Indices);
    for ( ; GTI != GTE; ++GTI) {
        llvm::Value *idx = GTI.getOperand();
        auto SALAMValue = irmap->find(idx)->second;
        auto valueID = SALAMValue->getUID();
        if (llvm::StructType *STy = GTI.getStructTypeOrNull()) {
            assert(idx->getType()->isIntegerTy(32) && "Illegal struct idx");
            unsigned FieldNo = llvm::cast<llvm::ConstantInt>(idx)->getSExtValue();
            const llvm::StructLayout *Layout = layout.getStructLayout(STy);
            offsets.push_back(Layout->getElementOffset(FieldNo));
            offsetOfStruct.push_back(true);
            
        } else {
            llvm::Type * idxty = GTI.getIndexedType();
            offsets.push_back(1 * layout.getTypeAllocSize(idxty));
            offsetOfStruct.push_back(false);
        }
    }
}

void
GetElementPtr::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
    uint64_t ptr = (operands.front().getPtrRegValue());
    int64_t offset = 0;
    DPRINTF(RuntimeCompute, "|| Index Values\n");
    for (int i = 1; i < operands.size(); i++) {
        auto idx = operands.at(i);
        if (offsetOfStruct.at(i-1)) {
            offset += offsets.at(i-1);
            DPRINTF(RuntimeCompute, "|| %s, struct offset = %d\n", idx.getIRStub(), offsets.at(i-1));
        } else {
        #if USE_LLVM_AP_VALUES
            int64_t arrayIdx = idx.getIntRegValue().getSExtValue();
        #else
            int64_t arrayIdx = idx.getSIntRegValue();
        #endif
            DPRINTF(RuntimeCompute, "|| %s = %d, dimension offset = %d\n", idx.getIRStub(), arrayIdx, offsets.at(i-1));
            offset += arrayIdx * offsets.at(i-1);
        }
    }

    uint64_t result = ptr + offset;
    DPRINTF(RuntimeCompute, "|| Ptr[%x]  Offset[%x] (Flat Idx[%d])\n", ptr, offset, offset/resultElementSizeInBytes);
    DPRINTF(RuntimeCompute, "|| Result: Addr[%x]\n", result);
    setRegisterValue(result);
}

// SALAM-Trunc // -----------------------------------------------------------//
void // Debugging Interface
Trunc::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Trunc>(id, OpCode, cycles, fu);
}

Trunc::Trunc(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
Trunc::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt result = operands.at(0).getIntRegValue().trunc(size);
    llvm::SmallString<8> resstr;
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    // The trunc is handled automatically when we set the return register
    uint64_t result = operands.at(0).getUIntRegValue();
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-ZExt // ------------------------------------------------------------//
void // Debugging Interface
ZExt::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createZExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::ZExt>(id, OpCode, cycles, fu);
}

ZExt::ZExt(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
ZExt::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt result = operands.at(0).getIntRegValue().zext(size);
    llvm::SmallString<8> resstr;
    result.toStringUnsigned(resstr);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
#else
    // Unsigned data doesn't need any modification when ZExtending
    uint64_t result = operands.at(0).getUIntRegValue();
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
#endif
    setRegisterValue(result);
}

// SALAM-SExt // ------------------------------------------------------------//
void // Debugging Interface
SExt::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createSExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SExt>(id, OpCode, cycles, fu);
}

SExt::SExt(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SExt::compute() {
    // Perform computations
    // Store results in temp location
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
#if USE_LLVM_AP_VALUES
    llvm::APInt result = operands.at(0).getIntRegValue().sext(size);
    llvm::SmallString<8> resstr;
    result.toStringSigned(resstr);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, resstr.c_str());
    setRegisterValue(result);
#else
    int64_t result = operands.at(0).getSIntRegValue();
    DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, result);
    setRegisterValue((uint64_t)result);
#endif
}

// SALAM-FPToUI // ----------------------------------------------------------//
void // Debugging Interface
FPToUI::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFPToUIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPToUI>(id, OpCode, cycles, fu);
}

FPToUI::FPToUI(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPToUI::compute() {
#if USE_LLVM_AP_VALUES
    #if (LLVM_VERSION_MAJOR <= 9)
        auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
    #else
        auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
    #endif
    llvm::APSInt tmp(size, true);
    bool exact;
    auto opdata = operands.front().getFloatRegValue();
    auto err = opdata.convertToInteger(tmp,
                                      rounding,
                                      &exact);
    assert(err == llvm::APFloatBase::opStatus::opOK);
    setRegisterValue(tmp);
    llvm::SmallString<8> tmpstr;
    tmp.toString(tmpstr);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, tmpstr.c_str());
#else
    switch (operands.front().getSize()) {
        case 32:
        {
            float opdata = operands.front().getFloatFromReg();
            setRegisterValue((uint64_t)opdata);
            DPRINTF(RuntimeCompute, "|| Result: %u\n", (uint64_t)opdata);
            break;
        }
        case 64:
        {
            double opdata = operands.front().getDoubleFromReg();
            setRegisterValue((uint64_t)opdata);
            DPRINTF(RuntimeCompute, "|| Result: %u\n", (uint64_t)opdata);
            break;
        }
        default:
        {
            assert(0 && "Must use AP values for nonstandard FP sizes.");
            break;
        }
    }
#endif
}

// SALAM-FPToSI // ----------------------------------------------------------//
void // Debugging Interface
FPToSI::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFPToSIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPToSI>(id, OpCode, cycles, fu);
}

FPToSI::FPToSI(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPToSI::compute() {
#if USE_LLVM_AP_VALUES
    #if (LLVM_VERSION_MAJOR <= 9)
        auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
    #else
        auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
    #endif
    llvm::APSInt tmp(size, false);
    bool exact;
    auto opdata = operands.front().getFloatRegValue();
    auto err = opdata.convertToInteger(tmp,
                                      rounding,
                                      &exact);
    assert(err == llvm::APFloatBase::opStatus::opOK);
    setRegisterValue(tmp);
    llvm::SmallString<8> tmpstr;
    tmp.toString(tmpstr);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, tmpstr.c_str());
#else
    switch (operands.front().getSize()) {
        case 32:
        {
            float opdata = operands.front().getFloatFromReg();
            int64_t tmp = (int64_t)opdata; // Truncate to integer
            DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, tmp);
            setRegisterValue((uint64_t)tmp);
            break;
        }
        case 64:
        {
            double opdata = operands.front().getDoubleFromReg();
            int64_t tmp = (int64_t)opdata; // Truncate to integer
            DPRINTF(RuntimeCompute, "|| %s = %d\n", ir_stub, tmp);
            setRegisterValue((uint64_t)tmp);
            break;
        }
        default:
        {
            assert(0 && "Must use AP values for nonstandard FP sizes.");
            break;
        }
    }
#endif
}

// SALAM-UIToFP // ----------------------------------------------------------//
void // Debugging Interface
UIToFP::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createUIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::UIToFP>(id, OpCode, cycles, fu);
}

UIToFP::UIToFP(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
UIToFP::compute() {
#if USE_LLVM_AP_VALUES
    #if (LLVM_VERSION_MAJOR <= 9)
        auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
    #else
        auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
    #endif
    auto opdata = operands.front().getIntRegValue();
    llvm::APFloat tmp(irtype->getFltSemantics());
    auto err = tmp.convertFromAPInt(opdata, false, rounding);
    assert(err == llvm::APFloatBase::opStatus::opOK);
    setRegisterValue(tmp);
#else
    auto opdata = operands.front().getUIntRegValue();
    switch (size) {
        case 32:
        {
            float tmp = (float)opdata; // Cast to float
            setRegisterValue(*(uint64_t *)&tmp); // Bitcast for writeback to reg
            break;
        }
        case 64:
        {
            double tmp = (double)opdata; // Cast to double
            setRegisterValue(*(uint64_t *)&tmp); // Bitcast for writeback to reg
            break;
        }
        default:
        {
            assert(0 && "Must use AP values for nonstandard FP sizes.");
            break;
        }
    }
#endif
}

// SALAM-SIToFP // ----------------------------------------------------------//
void // Debugging Interface
SIToFP::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createSIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::SIToFP>(id, OpCode, cycles, fu);
}

SIToFP::SIToFP(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
SIToFP::compute() {
#if USE_LLVM_AP_VALUES
    #if (LLVM_VERSION_MAJOR <= 9)
        auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
    #else
        auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
    #endif
    auto opdata = operands.front().getIntRegValue();
    llvm::APFloat tmp(irtype->getFltSemantics());
    auto err = tmp.convertFromAPInt(opdata, false, rounding);
    assert(err == llvm::APFloatBase::opStatus::opOK);
    setRegisterValue(tmp);
#else
    auto opdata = operands.front().getSIntRegValue();
    switch (size) {
        case 32:
        {
            float tmp = (float)opdata; // Cast to float
            setRegisterValue(*(uint64_t *)&tmp); // Bitcast for writeback to reg
            break;
        }
        case 64:
        {
            double tmp = (double)opdata; // Cast to double
            setRegisterValue(*(uint64_t *)&tmp); // Bitcast for writeback to reg
            break;
        }
        default:
        {
            assert(0 && "Must use AP values for nonstandard FP sizes.");
            break;
        }
    }
#endif
}

// SALAM-FPTrunc // ---------------------------------------------------------//
void // Debugging Interface
FPTrunc::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFPTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPTrunc>(id, OpCode, cycles, fu);
}

FPTrunc::FPTrunc(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPTrunc::compute() {
#if USE_LLVM_AP_VALUES
    #if (LLVM_VERSION_MAJOR <= 9)
        auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
    #else
        auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
    #endif
    auto opdata = operands.front().getFloatRegValue();
    llvm::APFloat tmp(opdata);
    bool losesInfo;
    auto err = tmp.convert(irtype->getFltSemantics(), rounding, &losesInfo);
    assert(err == llvm::APFloatBase::opStatus::opOK);
    setRegisterValue(tmp);
#else
    switch (operands.front().getSize()) {
        case 64:
        {
            double opdata = operands.front().getDoubleFromReg();
            float tmp = (float)opdata; // Cast to float
            setRegisterValue(*(uint64_t *)&tmp); // Bitcast for writeback to reg
            break;
        }
        default:
        {
            assert(0 && "Must use AP values for nonstandard FP sizes.");
        }
    }
#endif
}

// SALAM-FPExt // -----------------------------------------------------------//
void // Debugging Interface
FPExt::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFPExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FPExt>(id, OpCode, cycles, fu);
}

FPExt::FPExt(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
FPExt::compute() {
#if USE_LLVM_AP_VALUES
    #if (LLVM_VERSION_MAJOR <= 9)
        auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
    #else
        auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
    #endif
    auto opdata = operands.front().getFloatRegValue();
    llvm::APFloat tmp(opdata);
    bool losesInfo;
    auto err = tmp.convert(irtype->getFltSemantics(), rounding, &losesInfo);
    assert(err == llvm::APFloatBase::opStatus::opOK);
    setRegisterValue(tmp);
#else
    switch (operands.front().getSize()) {
        case 32:
        {
            float opdata = operands.front().getFloatFromReg();
            double tmp = (double)opdata; // Cast to double
            setRegisterValue(*(uint64_t *)&tmp); // Bitcast for writeback to reg
            break;
        }
        default:
        {
            assert(0 && "Must use AP values for nonstandard FP sizes.");
        }
    }
#endif
}

// SALAM-PtrToInt // --------------------------------------------------------//
void // Debugging Interface
PtrToInt::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createPtrToIntInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::PtrToInt>(id, OpCode, cycles, fu);
}

PtrToInt::PtrToInt(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
PtrToInt::compute() {
    auto opdata = operands.front().getPtrRegValue();
#if USE_LLVM_AP_VALUES
    setRegisterValue(llvm::APInt(64, opdata));
#else
    setRegisterValue(opdata);
#endif
}

// SALAM-IntToPtr // --------------------------------------------------------//
void // Debugging Interface
IntToPtr::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createIntToPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::IntToPtr>(id, OpCode, cycles, fu);
}

IntToPtr::IntToPtr(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
IntToPtr::compute() {
#if USE_LLVM_AP_VALUES
    auto opdata = operands.front().getIntRegValue();
    assert(opdata.isUnsigned());
    int64_t tmp = opdata.getExtValue();
    setRegisterValue(*(uint64_t *)&tmp);
#else
    auto opdata = operands.front().getUIntRegValue();
    setRegisterValue(opdata);
#endif
}

// SALAM-BitCast // --------------------------------------------------------//
void // Debugging Interface
BitCast::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createBitCastInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::BitCast>(id, OpCode, cycles, fu);
}

BitCast::BitCast(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
    std::vector<uint64_t> base_params;
    base_params.push_back(id);
    base_params.push_back(OpCode);
    base_params.push_back(cycles);
    conditions.push_back(base_params);
}

void
BitCast::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
}

void
BitCast::compute() {
#if USE_LLVM_AP_VALUES
    auto opdata = operands.front().getPtrRegValue();
    setRegisterValue(opdata);
#else
    auto opdata = operands.front().getPtrRegValue();
    setRegisterValue(opdata);
#endif
}

// SALAM-ICmp // ------------------------------------------------------------//
void // Debugging Interface
ICmp::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createICmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::ICmp>(id, OpCode, cycles, fu);
}

ICmp::ICmp(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    // ****** //
    llvm::CmpInst * inst = llvm::dyn_cast<llvm::CmpInst>(irval);
    this->predicate = inst->getPredicate();
    DPRINTF(SALAM_Debug, "Integer Comparison Predicate [%i | %s]\n", this->predicate, inst->getPredicateName(inst->getPredicate()).str());

}

void
ICmp::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
    bool result = false;
#if USE_LLVM_AP_VALUES
    if (operands.at(0).hasIntVal() && operands.at(1).hasIntVal()){
        switch (predicate) {
            case SALAM::Predicate::ICMP_EQ: { result = operands.at(0).getIntRegValue().eq((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_NE: { result = operands.at(0).getIntRegValue().ne((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_UGT: { result = operands.at(0).getIntRegValue().ugt((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_UGE: { result = operands.at(0).getIntRegValue().uge((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_ULT: { result = operands.at(0).getIntRegValue().ult((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_ULE: { result = operands.at(0).getIntRegValue().ule((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_SGT: { result = operands.at(0).getIntRegValue().sgt((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_SGE: { result = operands.at(0).getIntRegValue().sge((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_SLT: { result = operands.at(0).getIntRegValue().slt((operands.at(1).getIntRegValue())); break; }
            case SALAM::Predicate::ICMP_SLE: { result = operands.at(0).getIntRegValue().sle((operands.at(1).getIntRegValue())); break; }
            default: break;
        }
    } else if (operands.at(0).hasPtrVal() && operands.at(1).hasPtrVal()) {
        switch (predicate) {
            case SALAM::Predicate::ICMP_EQ: { result = operands.at(0).getPtrRegValue() == operands.at(1).getPtrRegValue(); break; }
            case SALAM::Predicate::ICMP_NE: { result = operands.at(0).getPtrRegValue() != operands.at(1).getPtrRegValue(); break; }
            case SALAM::Predicate::ICMP_UGT: { result = operands.at(0).getPtrRegValue() > operands.at(1).getPtrRegValue(); break; }
            case SALAM::Predicate::ICMP_UGE: { result = operands.at(0).getPtrRegValue() >= operands.at(1).getPtrRegValue(); break; }
            case SALAM::Predicate::ICMP_ULT: { result = operands.at(0).getPtrRegValue() < operands.at(1).getPtrRegValue(); break; }
            case SALAM::Predicate::ICMP_ULE: { result = operands.at(0).getPtrRegValue() <= operands.at(1).getPtrRegValue(); break; }
            default: break;
         }
    } else {
        panic("Got either wrong or differing datatypes for ICMP");
    }
#else
    if (operands.at(0).hasIntVal() && operands.at(1).hasIntVal()){
        uint64_t uOp1 = operands.at(0).getUIntRegValue();
        uint64_t uOp2 = operands.at(1).getUIntRegValue();
        int64_t  sOp1 = operands.at(0).getSIntRegValue();
        int64_t  sOp2 = operands.at(1).getSIntRegValue();

        switch (predicate) {
            case SALAM::Predicate::ICMP_EQ: { result = (uOp1 == uOp2); break; }
            case SALAM::Predicate::ICMP_NE: { result = (uOp1 != uOp2); break; }
            case SALAM::Predicate::ICMP_UGT: { result = (uOp1 > uOp2); break; }
            case SALAM::Predicate::ICMP_UGE: { result = (uOp1 >= uOp2); break; }
            case SALAM::Predicate::ICMP_ULT: { result = (uOp1 < uOp2); break; }
            case SALAM::Predicate::ICMP_ULE: { result = (uOp1 <= uOp2); break; }
            case SALAM::Predicate::ICMP_SGT: { result = (sOp1 > sOp2); break; }
            case SALAM::Predicate::ICMP_SGE: { result = (sOp1 >= sOp2); break; }
            case SALAM::Predicate::ICMP_SLT: { result = (sOp1 < sOp2); break; }
            case SALAM::Predicate::ICMP_SLE: { result = (sOp1 <= sOp2); break; }
            default: break;
        }
    } else if (operands.at(0).hasPtrVal() && operands.at(1).hasPtrVal()) {
        uint64_t uOp1 = operands.at(0).getPtrRegValue();
        uint64_t uOp2 = operands.at(1).getPtrRegValue();
        switch (predicate) {
            case SALAM::Predicate::ICMP_EQ: { result = (uOp1 == uOp2); break; }
            case SALAM::Predicate::ICMP_NE: { result = (uOp1 != uOp2); break; }
            case SALAM::Predicate::ICMP_UGT: { result = (uOp1 > uOp2); break; }
            case SALAM::Predicate::ICMP_UGE: { result = (uOp1 >= uOp2); break; }
            case SALAM::Predicate::ICMP_ULT: { result = (uOp1 < uOp2); break; }
            case SALAM::Predicate::ICMP_ULE: { result = (uOp1 <= uOp2); break; }
            default: break;
        }
    } else {
        panic("Got either wrong or differing datatypes for ICMP");
    }
#endif
    setRegisterValue(result);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, result ? "TRUE" : "FALSE");
}

// SALAM-FCmp // ------------------------------------------------------------//
void // Debugging Interface
FCmp::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createFCmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::FCmp>(id, OpCode, cycles, fu);
}

FCmp::FCmp(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
    bool result = false;
#if USE_LLVM_AP_VALUES
    auto op1 = operands.at(0).getFloatRegValue();
    auto op2 = operands.at(1).getFloatRegValue();
    auto cmp = op1.compare(op2);
    switch(predicate)
    {
        case SALAM::Predicate::FCMP_FALSE: {
            result = false;
            break;
        }
        case SALAM::Predicate::FCMP_OEQ:   {
            result = (cmp == llvm::APFloatBase::cmpResult::cmpEqual);
            break;
        }
        case SALAM::Predicate::FCMP_OGT:   {
            result = (cmp == llvm::APFloatBase::cmpResult::cmpGreaterThan);
            break;
        }
        case SALAM::Predicate::FCMP_OGE:   {
            result = (cmp == llvm::APFloatBase::cmpResult::cmpEqual) || 
                          (cmp == llvm::APFloatBase::cmpResult::cmpGreaterThan);
            break;
        }
        case SALAM::Predicate::FCMP_OLT:   {
            result = (cmp == llvm::APFloatBase::cmpResult::cmpLessThan);
            break;
        }
        case SALAM::Predicate::FCMP_OLE:   {
            result = (cmp == llvm::APFloatBase::cmpResult::cmpEqual) || 
                          (cmp == llvm::APFloatBase::cmpResult::cmpLessThan);
            break;
        }
        case SALAM::Predicate::FCMP_ONE:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpUnordered) && 
                          (cmp != llvm::APFloatBase::cmpResult::cmpEqual);
            break;
        }
        case SALAM::Predicate::FCMP_ORD:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpUnordered);
            break;
        }
        case SALAM::Predicate::FCMP_UNO:   {
            result = (cmp == llvm::APFloatBase::cmpResult::cmpUnordered);
            break;
        }
        case SALAM::Predicate::FCMP_UEQ:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpUnordered) ||
                          (cmp == llvm::APFloatBase::cmpResult::cmpEqual);
            break;
        }
        case SALAM::Predicate::FCMP_UGT:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpUnordered) ||
                          (cmp == llvm::APFloatBase::cmpResult::cmpGreaterThan);
            break;
        }
        case SALAM::Predicate::FCMP_UGE:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpUnordered) ||
                          (cmp == llvm::APFloatBase::cmpResult::cmpEqual) ||
                          (cmp == llvm::APFloatBase::cmpResult::cmpGreaterThan);
            break;
        }
        case SALAM::Predicate::FCMP_ULT:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpUnordered) ||
                          (cmp == llvm::APFloatBase::cmpResult::cmpLessThan);
            break;
        }
        case SALAM::Predicate::FCMP_ULE:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpUnordered) ||
                          (cmp == llvm::APFloatBase::cmpResult::cmpEqual) ||
                          (cmp == llvm::APFloatBase::cmpResult::cmpLessThan);
            break;
        }
        case SALAM::Predicate::FCMP_UNE:   {
            result = (cmp != llvm::APFloatBase::cmpResult::cmpEqual);
            break;
        }
        case SALAM::Predicate::FCMP_TRUE:  {
            result = true;
            break;
        }
    }
#else
    double op1, op2;
    switch (operands.front().getSize()) {
        case 32:
        {
            op1 = (double)operands.at(0).getFloatFromReg();
            op2 = (double)operands.at(1).getFloatFromReg();
            break;
        }
        case 64:
        {
            op1 = operands.at(0).getDoubleFromReg();
            op2 = operands.at(1).getDoubleFromReg();
            break;
        }
        default:
        {
            assert(0 && "Must use AP values for nonstandard FP sizes.");
            break;
        }
    }
    bool unordered = (std::isnan(op1) || std::isnan(op2));
    switch (predicate) {
        case SALAM::Predicate::FCMP_FALSE: {
            result = false;
            break;
        }
        case SALAM::Predicate::FCMP_OEQ:   {
            result = (!unordered && (op1 == op2));
            break;
        }
        case SALAM::Predicate::FCMP_OGT:   {
            result = (!unordered && (op1 > op2));
            break;
        }
        case SALAM::Predicate::FCMP_OGE:   {
            result = (!unordered && (op1 >= op2));
            break;
        }
        case SALAM::Predicate::FCMP_OLT:   {
            result = (!unordered && (op1 < op2));
            break;
        }
        case SALAM::Predicate::FCMP_OLE:   {
            result = (!unordered && (op1 <= op2));
            break;
        }
        case SALAM::Predicate::FCMP_ONE:   {
            result = (!unordered && (op1 != op2));
            break;
        }
        case SALAM::Predicate::FCMP_ORD:   {
            result = (!unordered);
            break;
        }
        case SALAM::Predicate::FCMP_UNO:   {
            result = (unordered);
            break;
        }
        case SALAM::Predicate::FCMP_UEQ:   {
            result = (unordered || (op1 == op2));
            break;
        }
        case SALAM::Predicate::FCMP_UGT:   {
            result = (unordered || (op1 > op2));
            break;
        }
        case SALAM::Predicate::FCMP_UGE:   {
            result = (unordered || (op1 >= op2));
            break;
        }
        case SALAM::Predicate::FCMP_ULT:   {
            result = (unordered || (op1 < op2));
            break;
        }
        case SALAM::Predicate::FCMP_ULE:   {
            result = (unordered || (op1 <= op2));
            break;
        }
        case SALAM::Predicate::FCMP_UNE:   {
            result = (unordered || (op1 != op2));
            break;
        }
        case SALAM::Predicate::FCMP_TRUE:  {
            result = true;
            break;
        }
    }
#endif
    setRegisterValue(result);
    DPRINTF(RuntimeCompute, "|| Comparing %f, %f\n", op1,op2);
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, result ? "TRUE" : "FALSE");
}

// SALAM-Phi // -------------------------------------------------------------//
void // Debugging Interface
Phi::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "%s%s%s\n\t\t %s%d\n\t\t %s%d%s\n",
    //         "|-(", llvm::Instruction::getOpcodeName(conditions.at(0).at(1)), " Instruction)",
    //         " | Opcode: ", conditions.at(0).at(1),
    //         " | Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createPHIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Phi>(id, OpCode, cycles, fu);
}

Phi::Phi(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    llvm::PHINode * phi = llvm::dyn_cast<llvm::PHINode>(irval);
    assert(phi);
    phiArgTy args;
    for (int i = 0; i < Instruction::getStaticDependencies().size();) {
        args.second = Instruction::getStaticDependencies(i); ++i;
        args.first = std::dynamic_pointer_cast<SALAM::BasicBlock>(Instruction::getStaticDependencies(i)); ++i;
        this->phiArgs.insert(args);
    }
}

//std::deque<uint64_t>
std::vector<uint64_t>
Phi::runtimeInitialize() {
    assert(getDependencyCount() == 0);
    //std::deque<uint64_t> dep_uids;
    std::vector<uint64_t> dep_uids;
    std::shared_ptr<SALAM::Value> static_dependency;

    auto it = phiArgs.find(previousBB);
    if (it != phiArgs.end()) static_dependency = it->second;
    else assert(0 && "Previous BasicBlock not found in PHI args");

    auto dep_uid = static_dependency->getUID();
    operands.push_back(SALAM::Operand(static_dependency));
    if ((static_dependency->isConstant()) || (static_dependency->isArgument())) {
        operands.back().updateOperandRegister();
    } else {
        dep_uids.push_back(dep_uid);
    }

    return dep_uids;
}

void
Phi::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);
    DPRINTF(RuntimeCompute, "|| PHI entered from %s, using value: %s\n",
        previousBB->getIRStub(), operands.front().getIRString());

    setRegisterValue(operands.front().getOpRegister());
}

void
Phi::setPrevBB(std::shared_ptr<SALAM::BasicBlock> prevBB)
{
    auto it = phiArgs.find(prevBB);
    if (it != phiArgs.end()) previousBB = prevBB;
    else assert(0 && "Previous BasicBlock not found in PHI args");
}

valueListTy
Phi::getStaticDependencies() const {
    valueListTy deps;

    auto it = phiArgs.find(previousBB);
    if (it != phiArgs.end()) deps.push_back(it->second);
    else assert(0 && "Previous BasicBlock not found in PHI args");

    return deps;
}


// SALAM-Call // ------------------------------------------------------------//
void // Debugging Interface
Call::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createCallInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Call>(id, OpCode, cycles, fu);
}

Call::Call(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    callee = staticDependencies.back();
    staticDependencies.pop_back();
    // ****** //
}

void
Call::compute() {
    // Call does not use compute. Special handling is used in the scheduler.
}

// SALAM-Select // ----------------------------------------------------------//
void // Debugging Interface
Select::dumper() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     DPRINTF(SALAM_Debug, "| %s %s %s|\n\t\t %s %d \n\t\t %s %d \n\t\t %s %d %s \n",
    //         "************** [", llvm::Instruction::getOpcodeName(conditions.at(0).at(1))  ,"] Instruction Dump **************",
    //         "    UID: ", conditions.at(0).at(0),
    //         " Opcode: ", conditions.at(0).at(1),
    //         "Latency: ", conditions.at(0).at(2), " Cycles"
    //     );
    // }
}

std::shared_ptr<SALAM::Instruction>
createSelectInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    return std::make_shared<SALAM::Select>(id, OpCode, cycles, fu);
}

Select::Select(uint64_t id,
         uint64_t OpCode,
              uint64_t cycles,
              uint64_t fu) :
         Instruction(id, OpCode, cycles,fu)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // if (DTRACE(SALAM_Debug)) {
    //     this->dbgr = new Debugger();
    // }
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
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    SALAM::Instruction::initialize(irval, irmap, valueList);
    this->condition = getStaticDependencies(0);
    this->trueValue = getStaticDependencies(1);
    this->falseValue = getStaticDependencies(2);
    // ****** //
}

// std::shared_ptr<SALAM::Value>
// Select::evaluate() {
// #if USE_LLVM_AP_VALUES
//     if(condition->getIntRegValue().isOneValue()) return trueValue;
//     return falseValue;
// #else
//     if(condition->getUIntRegValue() == 1) return trueValue;
//     return falseValue;
// #endif
// }

void
Select::compute() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // else if(DTRACE(SALAM_Debug)) DPRINTF(Runtime, "||++compute()\n");
    DPRINTF(RuntimeCompute, "|| Computing %s\n", ir_string);

    auto cond = operands.at(0);
    auto trueVal = operands.at(1);
    auto falseVal = operands.at(2);

#if USE_LLVM_AP_VALUES
    auto resultReg = (cond.getIntRegValue().isOneValue()) ? trueVal.getOpRegister() : falseVal.getOpRegister();
#else
    auto resultReg = (cond.getUIntRegValue() == 1) ? trueVal.getOpRegister() : falseVal.getOpRegister();
    DPRINTF(RuntimeCompute, "|| Selecting %s condition\n", (cond.getUIntRegValue() == 1) ? "TRUE" : "FALSE");
    DPRINTF(RuntimeCompute, "|| %s = %s\n", ir_stub, (cond.getUIntRegValue() == 1) ? trueVal.getIRStub() : falseVal.getIRStub());
#endif
    setRegisterValue(resultReg);
}

} // namespace SALAM

//---------------------------------------------------------------------------//