#ifndef __HWMODEL_FENCE_HH__
#define __HWMODEL_FENCE_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Fence.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Fence: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Fence();
		Fence(const FenceParams &params);
};
#endif // __HWMODEL_FENCE_HH__