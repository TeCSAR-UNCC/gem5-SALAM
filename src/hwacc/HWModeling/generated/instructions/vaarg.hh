#ifndef __HWMODEL_VAARG_HH__
#define __HWMODEL_VAARG_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Vaarg.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Vaarg: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Vaarg();
		Vaarg(const VaargParams &params);
};
#endif // __HWMODEL_VAARG_HH__