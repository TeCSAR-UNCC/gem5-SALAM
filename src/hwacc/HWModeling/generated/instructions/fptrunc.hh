#ifndef __HWMODEL_FPTRUNC_HH__
#define __HWMODEL_FPTRUNC_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Fptrunc.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Fptrunc: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Fptrunc();
		Fptrunc(const FptruncParams &params);
};
#endif // __HWMODEL_FPTRUNC_HH__