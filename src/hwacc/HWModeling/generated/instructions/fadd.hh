#ifndef __HWMODEL_FADD_HH__
#define __HWMODEL_FADD_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Fadd.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Fadd: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Fadd();
		Fadd(const FaddParams &params);
};
#endif // __HWMODEL_FADD_HH__