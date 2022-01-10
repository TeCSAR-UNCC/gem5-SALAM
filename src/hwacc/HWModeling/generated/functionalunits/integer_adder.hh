#ifndef __HWMODEL_INTEGER_ADDER_HH__
#define __HWMODEL_INTEGER_ADDER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/IntegerAdder.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class IntegerAdder: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		IntegerAdder();
		IntegerAdder(const IntegerAdderParams &params);
};
#endif // __HWMODEL_INTEGER_ADDER_HH__