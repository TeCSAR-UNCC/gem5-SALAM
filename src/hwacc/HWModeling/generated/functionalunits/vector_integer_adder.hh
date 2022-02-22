#ifndef __HWMODEL_VECTOR_INTEGER_ADDER_HH__
#define __HWMODEL_VECTOR_INTEGER_ADDER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/VectorIntegerAdder.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class VectorIntegerAdder: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		VectorIntegerAdder();
		VectorIntegerAdder(const VectorIntegerAdderParams &params);
};
#endif // __HWMODEL_VECTOR_INTEGER_ADDER_HH__