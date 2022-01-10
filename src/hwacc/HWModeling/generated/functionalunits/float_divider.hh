#ifndef __HWMODEL_FLOAT_DIVIDER_HH__
#define __HWMODEL_FLOAT_DIVIDER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/FloatDivider.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class FloatDivider: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		FloatDivider();
		FloatDivider(const FloatDividerParams &params);
};
#endif // __HWMODEL_FLOAT_DIVIDER_HH__