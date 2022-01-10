#ifndef __HWMODEL_BIT_SHIFTER_HH__
#define __HWMODEL_BIT_SHIFTER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/BitShifter.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class BitShifter: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		BitShifter();
		BitShifter(const BitShifterParams &params);
};
#endif // __HWMODEL_BIT_SHIFTER_HH__