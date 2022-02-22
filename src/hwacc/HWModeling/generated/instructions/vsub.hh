#ifndef __HWMODEL_VSUB_HH__
#define __HWMODEL_VSUB_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Vsub.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Vsub: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Vsub();
		Vsub(const VsubParams &params);
};
#endif // __HWMODEL_VSUB_HH__