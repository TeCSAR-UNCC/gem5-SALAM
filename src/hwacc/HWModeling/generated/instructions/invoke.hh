#ifndef __HWMODEL_INVOKE_HH__
#define __HWMODEL_INVOKE_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Invoke.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Invoke: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Invoke();
		Invoke(const InvokeParams &params);
};
#endif // __HWMODEL_INVOKE_HH__