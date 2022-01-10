#ifndef __HWMODEL_DOUBLE_DIVIDER_HH__
#define __HWMODEL_DOUBLE_DIVIDER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/DoubleDivider.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class DoubleDivider: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		DoubleDivider();
		DoubleDivider(const DoubleDividerParams &params);
};
#endif // __HWMODEL_DOUBLE_DIVIDER_HH__