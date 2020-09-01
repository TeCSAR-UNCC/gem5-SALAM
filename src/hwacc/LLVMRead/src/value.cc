#include "value.hh"

void
SALAM::Value::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	irtype = irval->getType();
	if (irtype->getTypeID() == llvm::Type::PointerTyID) {
		size = 64; //We assume a 64-bit memory address space
	} else {
		size = irtype->getScalarSizeInBits();
	}
}

void
SALAM::ConstantValue::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	// Get the const data of the value
	llvm::ConstantData * cd = llvm::dyn_cast<llvm::ConstantData>(irval);
	assert(cd);
	switch(irtype->getTypeID()) {
		case llvm::Type::FloatTyID: ///< 32-bit floating point type
			{
				llvm::ConstantFP * fp = llvm::dyn_cast<llvm::ConstantFP>(cd);
				assert(fp);
				float tmpFloat = fp->getValueAPF().convertToFloat();
				constData = *(uint64_t *)&tmpFloat;
			}
			break;
		case llvm::Type::DoubleTyID: ///< 64-bit floating point type
			{
				llvm::ConstantFP * fp = llvm::dyn_cast<llvm::ConstantFP>(cd);
				assert(fp);
				double tmpDouble = fp->getValueAPF().convertToDouble();
				constData = *(uint64_t *)&tmpDouble;
			}
			break;
		case llvm::Type::IntegerTyID: ///< Arbitrary bit width integers
			{
				llvm::ConstantInt * in = llvm::dyn_cast<llvm::ConstantInt>(cd);
				assert(in);
				constData = in->getZExtValue();
			}
			break;
		default:
			assert(0); //We currently only support integers and float/double for constants
	}

	//Initialize SALAM::Value
	SALAM::Value::initialize(irval, irmap);
}

void
SALAM::GlobalConstantValue::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	// Parse the initializer of the value
	auto glb = llvm::dyn_cast<llvm::GlobalVariable>(irval);
	assert(glb);
	assert(glb->hasInitializer());

	//Initialize SALAM::ConstantValue
	SALAM::ConstantValue::initialize(glb->getInitializer(), irmap);
}

void
SALAM::Argument::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
	addRegister();
	//Initialize SALAM::Value
	SALAM::Value::initialize(irval, irmap);
}