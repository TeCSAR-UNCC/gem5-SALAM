#include "instruction.hh"


namespace SALAM {

//---------------------------------------------------------------------------//
//--------- Instruction Base Class ------------------------------------------//
//---------------------------------------------------------------------------//


Instruction::Instruction(uint64_t id) :
                         Value(id)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

Instruction::Instruction(uint64_t id,
                         uint64_t OpCode) :
                         Value(id),
                         llvmOpCode(OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

Instruction::Instruction(uint64_t id,
                         uint64_t OpCode,
                         uint64_t cycles) :
                         Value(id),
                         llvmOpCode(OpCode),
                         cycleCount(cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Instruction::instantiate(llvm::Value *irval,
                         irvmap *irmap,
                         SALAM::valueListTy *valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // Fetch the operands of the instruction
    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    llvm::Instruction * inst = llvm::dyn_cast<llvm::Instruction>(irval);
    assert(iruser);
    assert(inst);
    int phiBB = 0;
    for (auto const op : iruser->operand_values()) {
        auto mapit = irmap->find(op);
        //op->printAsOperand(llvm::errs());
        //llvm::errs() << " ";
        std::shared_ptr<SALAM::Value> opval;
        if(mapit == irmap->end()) {
            // TODO: Handle constant data and constant expressions
            if (dbg) DPRINTF(LLVMInterface, "Instantiate Operand as Constant Data/Expression\n");
            uint64_t id = valueList->back()->getUID() + 1;
            std::shared_ptr<SALAM::Constant> con = std::make_shared<SALAM::Constant>(id);
            valueList->push_back(con);
            irmap->insert(SALAM::irvmaptype(op, con));
            con->initialize(op, irmap, valueList);
            opval = con;
        } else {
            if (dbg) DPRINTF(LLVMInterface, "Instantiate Operands on Value List\n");
            opval = mapit->second;
        }
        if (dbg) DPRINTF(LLVMInterface, "Link Operand to Static Operands List\n");
        staticOperands.push_back(opval);
        if(llvm::isa<llvm::PHINode>(inst)) {
            llvm::PHINode * phi = llvm::dyn_cast<llvm::PHINode>(inst);
            llvm::Value * bb = llvm::dyn_cast<llvm::Value>(phi->getIncomingBlock(phiBB));
            mapit = irmap->find(bb);
            opval = mapit->second;
            staticOperands.push_back(opval);
            ++phiBB;
        }
    }
    if (dbg) DPRINTF(LLVMInterface, "Initialize Value - Instruction::instantiate\n");
    SALAM::Value::initialize(irval, irmap);
}

void
Instruction::signalUsers()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    for (auto user : dynamicUsers) {
        user->fetchDependencyVal(this);
    }
}

// SALAM-Ret // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createRetInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Ret>(id, OpCode, cycles);
}

Ret::Ret(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Ret::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Ret::compute()
{

}

// SALAM-Br // --------------------------------------------------------------//

std::shared_ptr<SALAM::Instruction>
createBrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Br>(id, OpCode, cycles);
}

Br::Br(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Br::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    llvm::BranchInst * br = llvm::dyn_cast<llvm::BranchInst>(irval);
    assert(br);
    isConditional(br->isConditional());
    if (isConditional()) {
        condition = getStaticOperands(0);
        trueDestination = getStaticOperands(1);
        falseDestination = getStaticOperands(2);
    } else {
        defaultDestination = getStaticOperands(0);
    }
}

std::shared_ptr<SALAM::Value>
Br::destination()
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    if(conditional) {
        if(condition->getReg()->getIntData()->isOneValue()) return trueDestination;
        else return falseDestination;
    }
    return defaultDestination;
}

void
Br::compute()
{
    // See destination
}

// SALAM-Switch // ----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createSwitchInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Switch>(id, OpCode, cycles);
}

Switch::Switch(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Switch::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    llvm::SwitchInst * switchInst = llvm::dyn_cast<llvm::SwitchInst>(irval);
    assert(switchInst);
    caseArgs newArgs;
    for (int i = 0; i < getStaticOperands().size();) {
        newArgs.first = getStaticOperands(i); ++i;
        newArgs.second = getStaticOperands(i); ++i;
        this->arguments.push_back(newArgs);
    }
}

std::shared_ptr<SALAM::Value>
Switch::destination(int switchVar)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    for (int i = 2; i < this->arguments.size(); ++i) {
        if (this->arguments.at(i).first->getReg()->getIntData()->getSExtValue() == switchVar) return this->arguments.at(i).second;
    }
    return this->defaultDest();
}

void
Switch::compute()
{
    // See destination
}

// SALAM-Add // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Add>(id, OpCode, cycles);
}

Add::Add(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Add::initialize(llvm::Value *irval,
                SALAM::irvmap *irmap,
                SALAM::valueListTy *valueList)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Add::compute()
{
    
}

// SALAM-FAdd // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFAddInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FAdd>(id, OpCode, cycles);
}

FAdd::FAdd(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FAdd::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FAdd::compute() {
    // Floating point Addition
    // <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

}

// SALAM-Sub // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Sub>(id, OpCode, cycles);
}

Sub::Sub(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Sub::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Sub::compute() {
    // Subtract Instruction
    // <result> = sub <ty> <op1>, <op2>; yields ty : result
    // <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
    // <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
    // <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result

}

// SALAM-FSub // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFSubInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FSub>(id, OpCode, cycles);
}

FSub::FSub(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FSub::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FSub::compute() {
    // Floating point Subtraction
    // <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

}

// SALAM-Mul // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Mul>(id, OpCode, cycles);
}

Mul::Mul(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Mul::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Mul::compute() {
    // Multiply Instruction
    // <result> = mul <ty> <op1>, <op2>          ; yields ty:result
    // <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
    // <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
    // <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result

}

// SALAM-FMul // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFMulInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FMul>(id, OpCode, cycles);
}

FMul::FMul(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FMul::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FMul::compute() {
    // Floating point Multiplication
    // <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

}

// SALAM-UDiv // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createUDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::UDiv>(id, OpCode, cycles);
}

UDiv::UDiv(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
UDiv::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
UDiv::compute() {
    // Unsigned Division

}

// SALAM-SDiv // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createSDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::SDiv>(id, OpCode, cycles);
}

SDiv::SDiv(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
SDiv::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
SDiv::compute() {
    // Signed Division

}

// SALAM-FDiv // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFDivInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FDiv>(id, OpCode, cycles);
}

FDiv::FDiv(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FDiv::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FDiv::compute() {
    // Floating point Division
    // <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result

}

// SALAM-URem // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createURemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::URem>(id, OpCode, cycles);
}

URem::URem(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
URem::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
URem::compute() {
    //Unsigned modulo division

}

// SALAM-SRem // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createSRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::SRem>(id, OpCode, cycles);
}

SRem::SRem(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
SRem::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
SRem::compute() {
    //Signed modulo division

}

// SALAM-FRem // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFRemInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FRem>(id, OpCode, cycles);
}

FRem::FRem(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FRem::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FRem::compute() {
    //Floating Point modulo division

}

// SALAM-Shl // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createShlInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Shl>(id, OpCode, cycles);
}

Shl::Shl(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Shl::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Shl::compute() {
    // Shift Left Operation
    // <result> = shl <ty> <op1>, <op2>; yields ty : result
    // <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
    // <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
    // <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result

}

// SALAM-LShr // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createLShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::LShr>(id, OpCode, cycles);
}

LShr::LShr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
LShr::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
LShr::compute() {

}

// SALAM-AShr // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createAShrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::AShr>(id, OpCode, cycles);
}

AShr::AShr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
AShr::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
AShr::compute() {
    // Arithmatic Shift Right Operation
    // <result> = ashr <ty> <op1>, <op2>; yields ty : result
    // <result> = ashr exact <ty> <op1>, <op2>; yields ty : result

}

// SALAM-And // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createAndInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::And>(id, OpCode, cycles);
}

And::And(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
And::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
And::compute() {
    // And Operation
    // <result> = and <ty> <op1>, <op2>; yields ty : result

}

// SALAM-Or // --------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createOrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Or>(id, OpCode, cycles);
}

Or::Or(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Or::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Or::compute() {
    // Or Operation
    // <result> = or <ty> <op1>, <op2>; yields ty : result

}

// SALAM-Xor // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createXorInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Xor>(id, OpCode, cycles);
}

Xor::Xor(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Xor::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Xor::compute() {
    // Xor Operation
    // <result> = xor <ty> <op1>, <op2>; yields ty : result

}

// SALAM-Load // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createLoadInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Load>(id, OpCode, cycles);
}

Load::Load(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Load::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //

}

void
Load::compute() {

}

// SALAM-Store // -----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createStoreInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Store>(id, OpCode, cycles);
}

Store::Store(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Store::initialize(llvm::Value * irval,
                        irvmap * irmap,
                        SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Store::compute() {

}

// SALAM-GEP // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createGetElementPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::GetElementPtr>(id, OpCode, cycles);
}

GetElementPtr::GetElementPtr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
GetElementPtr::initialize(llvm::Value * irval,
                            irvmap * irmap,
                            SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
    llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    assert(iruser);
    llvm::GetElementPtrInst * GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(irval);
    assert(GEP);


}

void
GetElementPtr::compute() {
    // <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
    // <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
    // <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>

}

// SALAM-Trunc // -----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Trunc>(id, OpCode, cycles);
}

Trunc::Trunc(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Trunc::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Trunc::compute() {

}

// SALAM-ZExt // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createZExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::ZExt>(id, OpCode, cycles);
}

ZExt::ZExt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
ZExt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
ZExt::compute() {

}

// SALAM-SExt // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createSExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::SExt>(id, OpCode, cycles);
}

SExt::SExt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
SExt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
SExt::compute() {

}

// SALAM-FPToUI // ----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFPToUIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FPToUI>(id, OpCode, cycles);
}

FPToUI::FPToUI(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FPToUI::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FPToUI::compute() {

}

// SALAM-FPToSI // ----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFPToSIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FPToSI>(id, OpCode, cycles);
}

FPToSI::FPToSI(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FPToSI::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FPToSI::compute() {

}

// SALAM-UIToFP // ----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createUIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::UIToFP>(id, OpCode, cycles);
}

UIToFP::UIToFP(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
UIToFP::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
UIToFP::compute() {

}

// SALAM-SIToFP // ----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createSIToFPInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::SIToFP>(id, OpCode, cycles);
}

SIToFP::SIToFP(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
SIToFP::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
SIToFP::compute() {

}

// SALAM-FPTrunc // ---------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFPTruncInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FPTrunc>(id, OpCode, cycles);
}

FPTrunc::FPTrunc(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FPTrunc::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FPTrunc::compute() {

}

// SALAM-FPExt // -----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFPExtInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FPExt>(id, OpCode, cycles);
}

FPExt::FPExt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FPExt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FPExt::compute() {

}

// SALAM-PtrToInt // --------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createPtrToIntInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::PtrToInt>(id, OpCode, cycles);
}

PtrToInt::PtrToInt(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
PtrToInt::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
PtrToInt::compute() {

}

// SALAM-IntToPtr // --------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createIntToPtrInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::IntToPtr>(id, OpCode, cycles);
}

IntToPtr::IntToPtr(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
IntToPtr::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
IntToPtr::compute() {

}

// SALAM-ICmp // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createICmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::ICmp>(id, OpCode, cycles);
}

ICmp::ICmp(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
ICmp::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}


void
ICmp::compute() {

}

// SALAM-FCmp // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createFCmpInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::FCmp>(id, OpCode, cycles);
}

FCmp::FCmp(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
FCmp::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
FCmp::compute() {

}

// SALAM-Phi // -------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createPHIInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Phi>(id, OpCode, cycles);
}

Phi::Phi(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Phi::initialize(llvm::Value * irval,
                    irvmap * irmap,
                    SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    llvm::PHINode * phi = llvm::dyn_cast<llvm::PHINode>(irval);
    assert(phi);
    phiNode args;
    for (int i = 0; i < getStaticOperands().size();) {
        args.first = getStaticOperands(i); ++i;
        args.second = getStaticOperands(i); ++i;
        this->arguments.push_back(args);
    }
}

std::shared_ptr<SALAM::Value>
Phi::evaluate(std::shared_ptr<SALAM::Value> previousBB) {
    for (auto const it : arguments) {
        if(previousBB == it.second) return it.first;
    }
    return nullptr;
}

void
Phi::compute() {

}

// SALAM-Call // ------------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createCallInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Instruction>(id, OpCode, cycles);
}

Call::Call(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Call::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    // ****** //
}

void
Call::compute() {

}

// SALAM-Select // ----------------------------------------------------------//
std::shared_ptr<SALAM::Instruction>
createSelectInst(uint64_t id,
              uint64_t OpCode,
              uint64_t cycles) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    return std::make_shared<SALAM::Select>(id, OpCode, cycles);
}

Select::Select(uint64_t id,
         uint64_t OpCode,
         uint64_t cycles) :
         Instruction(id, OpCode)
{
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
}

void
Select::initialize(llvm::Value * irval,
                irvmap * irmap,
                SALAM::valueListTy * valueList) {
    if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __func__);
    this->condition = getStaticOperands(0);
    this->trueValue = getStaticOperands(1);
    this->falseValue = getStaticOperands(2);
    // ****** //
}

std::shared_ptr<SALAM::Value>
Select::evaluate() {
    if(condition->getReg()->getIntData()->isOneValue()) return trueValue;
    return falseValue;
}

void
Select::compute() {

}
}
//---------------------------------------------------------------------------//