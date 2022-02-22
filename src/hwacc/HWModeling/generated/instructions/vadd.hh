#ifndef __HWMODEL_VADD_HH__
#define __HWMODEL_VADD_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Vadd.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Vadd: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Vadd();
		Vadd(const VaddParams &params);
};
#endif // __HWMODEL_VADD_HH__