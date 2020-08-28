#include "instruction.hh"


namespace SALAM {
    void
    Instruction::initialize(llvm::Value * irval, irvmap * irmap) {
        SALAM::Value::initialize(irval, irmap);
    }
    
    // First Pass - Data Structure Initialization --------------- //
    
    std::shared_ptr<SALAM::Instruction>
    createRetInst(uint64_t id) { 

        return std::make_shared<SALAM::Ret>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createBrInst(uint64_t id) { 

        return std::make_shared<SALAM::Br>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createSwitchInst(uint64_t id) { 

        return std::make_shared<SALAM::Switch>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createAddInst(uint64_t id) { 
        // ID only as arguments

        return std::make_shared<SALAM::Add>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFAddInst(uint64_t id) { 

        return std::make_shared<SALAM::FAdd>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createSubInst(uint64_t id) { 

        return std::make_shared<SALAM::Sub>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFSubInst(uint64_t id) { 

        return std::make_shared<SALAM::FSub>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createMulInst(uint64_t id) { 

        return std::make_shared<SALAM::Mul>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFMulInst(uint64_t id) { 

        return std::make_shared<SALAM::FMul>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createUDivInst(uint64_t id) { 

        return std::make_shared<SALAM::UDiv>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createSDivInst(uint64_t id) { 

        return std::make_shared<SALAM::SDiv>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFDivInst(uint64_t id) { 

        return std::make_shared<SALAM::FDiv>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createURemInst(uint64_t id) { 

        return std::make_shared<SALAM::URem>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createSRemInst(uint64_t id) { 

        return std::make_shared<SALAM::SRem>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFRemInst(uint64_t id) { 

        return std::make_shared<SALAM::FRem>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createShlInst(uint64_t id) { 

        return std::make_shared<SALAM::Shl>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createLShrInst(uint64_t id) { 

        return std::make_shared<SALAM::LShr>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createAShrInst(uint64_t id) { 

        return std::make_shared<SALAM::AShr>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createAndInst(uint64_t id) { 

        return std::make_shared<SALAM::And>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createOrInst(uint64_t id) { 

        return std::make_shared<SALAM::Or>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createXorInst(uint64_t id) { 

        return std::make_shared<SALAM::Xor>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createLoadInst(uint64_t id) { 

        return std::make_shared<SALAM::Load>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createStoreInst(uint64_t id) { 

        return std::make_shared<SALAM::Store>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createGetElementPtrInst(uint64_t id) { 

        return std::make_shared<SALAM::GetElementPtr>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createTruncInst(uint64_t id) { 

        return std::make_shared<SALAM::Trunc>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createZExtInst(uint64_t id) { 

        return std::make_shared<SALAM::ZExt>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createSExtInst(uint64_t id) { 

        return std::make_shared<SALAM::SExt>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFPToUIInst(uint64_t id) { 

        return std::make_shared<SALAM::FPToUI>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFPToSIInst(uint64_t id) { 

        return std::make_shared<SALAM::FPToSI>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createUIToFPInst(uint64_t id) { 

        return std::make_shared<SALAM::UIToFP>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createSIToFPInst(uint64_t id) { 

        return std::make_shared<SALAM::SIToFP>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFPTruncInst(uint64_t id) { 

        return std::make_shared<SALAM::FPTrunc>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFPExtInst(uint64_t id) { 

        return std::make_shared<SALAM::FPExt>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createPtrToIntInst(uint64_t id) { 

        return std::make_shared<SALAM::PtrToInt>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createIntToPtrInst(uint64_t id) { 

        return std::make_shared<SALAM::IntToPtr>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createICmpInst(uint64_t id) { 

        return std::make_shared<SALAM::ICmp>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createFCmpInst(uint64_t id) { 

        return std::make_shared<SALAM::FCmp>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createPHIInst(uint64_t id) { 

        return std::make_shared<SALAM::Phi>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createCallInst(uint64_t id) { 
        // TODO - No Class for Call Function Yet
        return std::make_shared<SALAM::Instruction>(id);
    }

    std::shared_ptr<SALAM::Instruction>
    createSelectInst(uint64_t id) { 

        return std::make_shared<SALAM::Select>(id);
    }

    // Compute // -------------------------------------------------------- //


    //------------------------------------------//

    void
    Add::compute() {
    // Integer Addition
        // <result> = add <ty> <op1>, <op2>; yields ty : result
        // <result> = add nuw <ty> <op1>, <op2>; yields ty : result
        // <result> = add nsw <ty> <op1>, <op2>; yields ty : result
        // <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result

    }

    void
    Sub::compute() {
        // Subtract Instruction
        // <result> = sub <ty> <op1>, <op2>; yields ty : result
        // <result> = sub nuw <ty> <op1>, <op2>; yields ty : result
        // <result> = sub nsw <ty> <op1>, <op2>; yields ty : result
        // <result> = sub nuw nsw <ty> <op1>, <op2>; yields ty : result

    }

    void
    Mul::compute() {
        // Multiply Instruction
        // <result> = mul <ty> <op1>, <op2>          ; yields ty:result
        // <result> = mul nuw <ty> <op1>, <op2>; yields ty : result
        // <result> = mul nsw <ty> <op1>, <op2>; yields ty : result
        // <result> = mul nuw nsw <ty> <op1>, <op2>; yields ty : result
 
    }

    void
    UDiv::compute() {
        // Unsigned Division

    }

    void
    SDiv::compute() {
        // Signed Division

    }

    void
    URem::compute() {
        //Unsigned modulo division

        }

    void
    SRem::compute() {
        //Signed modulo division
    
    }

    void
    FAdd::compute() {
        // Floating point Addition
        // <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }

    void
    FSub::compute() {
        // Floating point Subtraction
        // <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }

    void
    FMul::compute() {
        // Floating point Multiplication
        // <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }

    void
    FDiv::compute() {
        // Floating point Division
        // <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }

    void
    FRem::compute() { }

    void
    Shl::compute() {
        // Shift Left Operation
        // <result> = shl <ty> <op1>, <op2>; yields ty : result
        // <result> = shl nuw <ty> <op1>, <op2>; yields ty : result
        // <result> = shl nsw <ty> <op1>, <op2>; yields ty : result
        // <result> = shl nuw nsw <ty> <op1>, <op2>; yields ty : result
        
    }

    void
    LShr::compute() {
        
    }

    void
    AShr::compute() {
        // Arithmatic Shift Right Operation
        // <result> = ashr <ty> <op1>, <op2>; yields ty : result
        // <result> = ashr exact <ty> <op1>, <op2>; yields ty : result
        
    }

    void
    And::compute() {
        // And Operation
        // <result> = and <ty> <op1>, <op2>; yields ty : result
        
    }

    void
    Or::compute() {
        // Or Operation
        // <result> = or <ty> <op1>, <op2>; yields ty : result
        
    }

    void
    Xor::compute() {
        // Xor Operation
        // <result> = xor <ty> <op1>, <op2>; yields ty : result
        
    }


    void
    Load::compute() {
        
    }

    void
    Store::compute() {
        
    }


    void
    Ret::compute() {

    }

    void
    Br::compute() {
        

    }

    void
    Switch::compute() {
    
    }

    void
    GetElementPtr::compute() {
        // <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
        // <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
        // <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
        
    }


    void
    Trunc::compute() {
        
    }

    void
    ZExt::compute() {
        
    }

    void
    SExt::compute() {
        
    }

    void
    FPToUI::compute() {
        
    }

    void
    FPToSI::compute() {
        
    }

    void
    UIToFP::compute() {
        
    }

    void
    SIToFP::compute() {
        
    }

    void
    FPTrunc::compute() {
        
    }

    void
    FPExt::compute() {
        
    }

    void
    PtrToInt::compute() { }
    void
    IntToPtr::compute() { }
  
    void
    FCmp::compute() {

    }

    void
    ICmp::compute() {
       
    }

    void
    Phi::compute() {

    }

    void
    Select::compute() {
       
    }
}