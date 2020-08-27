#include "instruction.hh"


namespace SALAM {
    void
    Instruction::initialize(llvm::Value * irval, irvmap * irmap) {
        SALAM::Value::initialize(irval, irmap);
    }

    std::shared_ptr<SALAM::Instruction>
    retInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    brInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    switchInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    addInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    faddInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    subInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fsubInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    mulInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fmulInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    udivInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    sdivInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fdivInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    uremInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    sremInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fremInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    shlInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    lshrInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    ashrInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    andInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    orInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    xorInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    loadInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    storeInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    getelementptrInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    truncInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    zextInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    sextInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fptouiInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fptosiInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    uitofpInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    sitofpInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fptruncInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fpextInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    ptrtointInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    inttoptrInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    icmpInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    fcmpInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    phiInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    callInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
    }

    std::shared_ptr<SALAM::Instruction>
    selectInst(llvm::Instruction * inst, uint64_t id) { 

        return std::make_shared<SALAM::Instruction>(inst, id);
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
    LLVMSwitch::compute() {
    
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