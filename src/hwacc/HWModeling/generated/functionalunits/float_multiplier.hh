#ifndef __HWMODEL_FLOAT_MULTIPLIER_HH__
#define __HWMODEL_FLOAT_MULTIPLIER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/FloatMultiplier.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class FloatMultiplier: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		FloatMultiplier();
		FloatMultiplier(const FloatMultiplierParams &params);
};
#endif // __HWMODEL_FLOAT_MULTIPLIER_HH__