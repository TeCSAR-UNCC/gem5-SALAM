#ifndef __HWMODEL_SELECT_HH__
#define __HWMODEL_SELECT_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Select.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Select: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Select();
		Select(const SelectParams &params);
};
#endif // __HWMODEL_SELECT_HH__