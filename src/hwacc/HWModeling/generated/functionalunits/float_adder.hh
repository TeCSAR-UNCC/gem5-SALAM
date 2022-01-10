#ifndef __HWMODEL_FLOAT_ADDER_HH__
#define __HWMODEL_FLOAT_ADDER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/FloatAdder.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class FloatAdder: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		FloatAdder();
		FloatAdder(const FloatAdderParams &params);
};
#endif // __HWMODEL_FLOAT_ADDER_HH__