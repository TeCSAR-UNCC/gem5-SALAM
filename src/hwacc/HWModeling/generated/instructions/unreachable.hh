#ifndef __HWMODEL_UNREACHABLE_HH__
#define __HWMODEL_UNREACHABLE_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Unreachable.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Unreachable: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Unreachable();
		Unreachable(const UnreachableParams &params);
};
#endif // __HWMODEL_UNREACHABLE_HH__