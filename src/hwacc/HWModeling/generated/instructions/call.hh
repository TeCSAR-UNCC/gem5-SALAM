#ifndef __HWMODEL_CALL_HH__
#define __HWMODEL_CALL_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Call.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Call: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Call();
		Call(const CallParams &params);
};
#endif // __HWMODEL_CALL_HH__