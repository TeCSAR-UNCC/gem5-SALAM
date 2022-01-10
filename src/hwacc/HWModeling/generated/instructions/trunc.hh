#ifndef __HWMODEL_TRUNC_HH__
#define __HWMODEL_TRUNC_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Trunc.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Trunc: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Trunc();
		Trunc(const TruncParams &params);
};
#endif // __HWMODEL_TRUNC_HH__