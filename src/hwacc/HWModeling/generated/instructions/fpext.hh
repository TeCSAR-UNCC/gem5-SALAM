#ifndef __HWMODEL_FPEXT_HH__
#define __HWMODEL_FPEXT_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Fpext.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Fpext: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Fpext();
		Fpext(const FpextParams &params);
};
#endif // __HWMODEL_FPEXT_HH__