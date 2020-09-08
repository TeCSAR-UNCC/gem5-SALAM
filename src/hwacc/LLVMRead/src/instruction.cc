#include "instruction.hh"


namespace SALAM {

    void
    Instruction::initialize(llvm::Value * irval, irvmap * irmap, SALAM::valueListTy * valueList) {
    	TRACEOUT("SALAM::Instruction::initialize");
        // Fetch the operands of the instruction
    	llvm::User * iruser = llvm::dyn_cast<llvm::User>(irval);
    	assert(iruser);
    	for (auto op : iruser->operand_values()) {
    		auto mapit = irmap->find(op);
    		std::shared_ptr<SALAM::Value> opval;
    		if(mapit == irmap->end()) {
    			// TODO: Handle constant data and constant expressions
    			uint64_t id = valueList->back()->getUID() + 1;
    			std::shared_ptr<SALAM::Constant> con = std::make_shared<SALAM::Constant>(id);
		        valueList->push_back(con);
		        irmap->insert(SALAM::irvmaptype(op, con));
		        con->initialize(op, irmap, valueList);
		        opval = con;
    		} else {
    			opval = mapit->second;
    		}
    		staticDependencies.push_back(opval);
    	}
        SALAM::Value::initialize(irval, irmap);
    }
    
    // SALAM-Ret // -------------------------------------------------------------//    
    std::shared_ptr<SALAM::Instruction>
    createRetInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createRetInst");
        return std::make_shared<SALAM::Ret>(id, OpCode);
    }

    void
    initializeRetInst(SALAM::Ret &retInst) {
        TRACEOUT("SALAM::initializeRetInst");
        retInst.setA().setB();
    }

    void
    Ret::compute() {

    }

    // SALAM-Br // --------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createBrInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createBrInst");
        return std::make_shared<SALAM::Br>(id, OpCode);
    }

    void
    initializeBrInst() {

    }

    void
    Br::compute() {
        

    }

    // SALAM-Switch // ----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createSwitchInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createSwitchInst");
        return std::make_shared<SALAM::Switch>(id, OpCode);
    }

    void
    initializeSwitchInst() {
        
    }

    void
    Switch::compute() {
    
    }

    // SALAM-Add // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createAddInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createAddInst");
        return std::make_shared<SALAM::Add>(id, OpCode);
    }

    void
    initializeAddInst() {
        
    }

    void
    Add::compute() {
    // Integer Addition
        // <result> = add <ty> <op1>, <op2>; yields ty : result
        // <result> = add nuw <ty> <op1>, <op2>; yields ty : result
        // <result> = add nsw <ty> <op1>, <op2>; yields ty : result
        // <result> = add nuw nsw <ty> <op1>, <op2>; yields ty : result

    }
    
    // SALAM-FAdd // ------------------------------------------------------------//    
    std::shared_ptr<SALAM::Instruction>
    createFAddInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFAddInst");
        return std::make_shared<SALAM::FAdd>(id, OpCode);
    }

    void
    initializeFAddInst() {
        
    }

    void
    FAdd::compute() {
        // Floating point Addition
        // <result> = fadd [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }
    
    // SALAM-Sub // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createSubInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createSubInst");
        return std::make_shared<SALAM::Sub>(id, OpCode);
    }
    
    void
    initializeSubInst() {
        
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
    createFSubInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFSubInst");
        return std::make_shared<SALAM::FSub>(id, OpCode);
    }

    void
    initializeFSubInst() {
        
    }

    void
    FSub::compute() {
        // Floating point Subtraction
        // <result> = fsub [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }

    // SALAM-Mul // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createMulInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createMulInst");
        return std::make_shared<SALAM::Mul>(id, OpCode);
    }

    void
    initializeMulInst() {
        
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
    createFMulInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFMulInst");
        return std::make_shared<SALAM::FMul>(id, OpCode);
    }

    void
    initializeFMulInst() {
        
    }

    void
    FMul::compute() {
        // Floating point Multiplication
        // <result> = fmul [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }

    // SALAM-UDiv // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createUDivInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createUDivInst");
        return std::make_shared<SALAM::UDiv>(id, OpCode);
    }

    void
    initializeUDivInst() {
        
    }

    void
    UDiv::compute() {
        // Unsigned Division

    }

    // SALAM-SDiv // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createSDivInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createSDivInst");
        return std::make_shared<SALAM::SDiv>(id, OpCode);
    }

    void
    initializeSDivInst() {
        
    }

    void
    SDiv::compute() {
        // Signed Division

    }

    // SALAM-FDiv // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createFDivInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFDivInst");
        return std::make_shared<SALAM::FDiv>(id, OpCode);
    }

    void
    initializeFDivInst() {
        
    }

    void
    FDiv::compute() {
        // Floating point Division
        // <result> = fdiv [fast-math flags]* <ty> <op1>, <op2>   ; yields ty:result
        
    }

    // SALAM-URem // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createURemInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createURemInst");
        return std::make_shared<SALAM::URem>(id, OpCode);
    }

    void
    initializeURemInst() {
        
    }

    void
    URem::compute() {
        //Unsigned modulo division

    }

    // SALAM-SRem // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createSRemInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createSRemInst");
        return std::make_shared<SALAM::SRem>(id, OpCode);
    }

    void
    initializeSRemInst() {
        
    }

    void
    SRem::compute() {
        //Signed modulo division
    
    }

    // SALAM-FRem // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createFRemInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFRemInst");
        return std::make_shared<SALAM::FRem>(id, OpCode);
    }

    void
    initializeFRemInst() {
        
    }

    void
    FRem::compute() {
        //Floating Point modulo division
    
    }

    // SALAM-Shl // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createShlInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createShlInst");
        return std::make_shared<SALAM::Shl>(id, OpCode);
    }

    void
    initializeShlInst() {
        
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
    createLShrInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createLShrInst");
        return std::make_shared<SALAM::LShr>(id, OpCode);
    }

    void
    initializeLShrInst() {
        
    }

    void
    LShr::compute() {
        
    }

    // SALAM-AShr // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createAShrInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createAShrInst");
        return std::make_shared<SALAM::AShr>(id, OpCode);
    }

    void
    initializeAShrInst() {
        
    }

    void
    AShr::compute() {
        // Arithmatic Shift Right Operation
        // <result> = ashr <ty> <op1>, <op2>; yields ty : result
        // <result> = ashr exact <ty> <op1>, <op2>; yields ty : result
        
    }

    // SALAM-And // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createAndInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createAndInst");
        return std::make_shared<SALAM::And>(id, OpCode);
    }

    void
    initializeAndInst() {
        
    }

    void
    And::compute() {
        // And Operation
        // <result> = and <ty> <op1>, <op2>; yields ty : result
        
    }

    // SALAM-Or // --------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createOrInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createOrInst");
        return std::make_shared<SALAM::Or>(id, OpCode);
    }

    void
    initializeOrInst() {
        
    }

    void
    Or::compute() {
        // Or Operation
        // <result> = or <ty> <op1>, <op2>; yields ty : result
        
    }

    // SALAM-Xor // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createXorInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createXorInst");
        return std::make_shared<SALAM::Xor>(id, OpCode);
    }

    void
    initializeXorInst() {
        
    }

    void
    Xor::compute() {
        // Xor Operation
        // <result> = xor <ty> <op1>, <op2>; yields ty : result
        
    }
    
    // SALAM-Load // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createLoadInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createLoadInst");
        return std::make_shared<SALAM::Load>(id, OpCode);
    }

    void
    initializeLoadInst() {
        
    }

    void
    Load::compute() {
        
    }

    // SALAM-Store // -----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createStoreInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createStoreInst");
        return std::make_shared<SALAM::Store>(id, OpCode);
    }

    void
    initializeStoreInst() {
        
    }

    void
    Store::compute() {
        
    }

    // SALAM-GEP // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createGetElementPtrInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createGetElementPtrInst");
        return std::make_shared<SALAM::GetElementPtr>(id, OpCode);
    }

    void
    initializeGetElementPtrInst() {
        
    }

    void
    GetElementPtr::compute() {
        // <result> = getelementptr <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
        // <result> = getelementptr inbounds <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
        // <result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
        
    }

    // SALAM-Trunc // -----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createTruncInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createTruncInst");
        return std::make_shared<SALAM::Trunc>(id, OpCode);
    }

    void
    initializeTruncInst() {
        
    }

    void
    Trunc::compute() {
        
    }

    // SALAM-ZExt // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createZExtInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createZExtInst");
        return std::make_shared<SALAM::ZExt>(id, OpCode);
    }

    void
    initializeZExtInst() {
        
    }

    void
    ZExt::compute() {
        
    }

    // SALAM-SExt // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createSExtInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createSExtInst");
        return std::make_shared<SALAM::SExt>(id, OpCode);
    }

    void
    initializeSExtInst() {
        
    }

    void
    SExt::compute() {
        
    }

    // SALAM-FPToUI // ----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createFPToUIInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFPToUIInst");
        return std::make_shared<SALAM::FPToUI>(id, OpCode);
    }

    void
    initializeFPToUIInst() {
        
    }

    void
    FPToUI::compute() {
        
    }

    // SALAM-FPToSI // ----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createFPToSIInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFPToSIInst");
        return std::make_shared<SALAM::FPToSI>(id, OpCode);
    }

    void
    initializeFPToSIInst() {
        
    }

    void
    FPToSI::compute() {
        
    }

    // SALAM-UIToFP // ----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createUIToFPInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createUIToFPInst");
        return std::make_shared<SALAM::UIToFP>(id, OpCode);
    }

    void
    initializeUIToFPInst() {
        
    }

    void
    UIToFP::compute() {
        
    }

    // SALAM-SIToFP // ----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createSIToFPInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createSIToFPInst");
        return std::make_shared<SALAM::SIToFP>(id, OpCode);
    }

    void
    initializeSIToFPInst() {
        
    }

    void
    SIToFP::compute() {
        
    }

    // SALAM-FPTrunc // ---------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createFPTruncInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFPTruncInst");
        return std::make_shared<SALAM::FPTrunc>(id, OpCode);
    }

    void
    initializeFPTruncInst() {
        
    }

    void
    FPTrunc::compute() {
        
    }

    // SALAM-FPExt // -----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createFPExtInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFPExtInst");
        return std::make_shared<SALAM::FPExt>(id, OpCode);
    }

    void
    initializeFPExtInst() {
        
    }

    void
    FPExt::compute() {
        
    }

    // SALAM-PtrToInt // --------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createPtrToIntInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createPtrToIntInst");
        return std::make_shared<SALAM::PtrToInt>(id, OpCode);
    }

    void
    initializePtrToIntInst() {
        
    }

    void
    PtrToInt::compute() { 

    }

    // SALAM-IntToPtr // --------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createIntToPtrInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createIntToPtrInst");
        return std::make_shared<SALAM::IntToPtr>(id, OpCode);
    }

    void
    initializeIntToPtrInst() {
        
    }

    void
    IntToPtr::compute() { 
        
    }  

    // SALAM-ICmp // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createICmpInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createICmpInst");
        return std::make_shared<SALAM::ICmp>(id, OpCode);
    }

    void
    initializeICmpInst() {
        
    }

    void
    ICmp::compute() {
       
    }

    // SALAM-FCmp // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createFCmpInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createFCmpInst");
        return std::make_shared<SALAM::FCmp>(id, OpCode);
    }

    void
    initializeFCmpInst() {
        
    }

    void
    FCmp::compute() {

    }

    // SALAM-Phi // -------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createPHIInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createPhiInst");
        return std::make_shared<SALAM::Phi>(id, OpCode);
    }

    void
    initializePhiInst() {
        
    }

    void
    Phi::compute() {

    }

    // SALAM-Call // ------------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createCallInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createCallInst");
        return std::make_shared<SALAM::Instruction>(id, OpCode);
    }

    void
    initializeCallInst() {
        
    }

    void
    Call::compute() {

    }

    // SALAM-Select // ----------------------------------------------------------//
    std::shared_ptr<SALAM::Instruction>
    createSelectInst(uint64_t id, uint64_t OpCode) { 
        TRACEOUT("SALAM::createSelectInst");
        return std::make_shared<SALAM::Select>(id, OpCode);
    }

    void
    initializeSelectInst() {
        
    }

    void
    Select::compute() {
       
    }
}
    //---------------------------------------------------------------------------//