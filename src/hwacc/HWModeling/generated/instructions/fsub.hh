#ifndef __HWMODEL_FSUB_HH__
#define __HWMODEL_FSUB_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Fsub.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Fsub: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Fsub();
		Fsub(const FsubParams &params);
};
#endif // __HWMODEL_FSUB_HH__