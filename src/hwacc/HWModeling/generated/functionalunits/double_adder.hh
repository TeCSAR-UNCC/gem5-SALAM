#ifndef __HWMODEL_DOUBLE_ADDER_HH__
#define __HWMODEL_DOUBLE_ADDER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/DoubleAdder.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class DoubleAdder: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		DoubleAdder();
		DoubleAdder(const DoubleAdderParams &params);
};
#endif // __HWMODEL_DOUBLE_ADDER_HH__