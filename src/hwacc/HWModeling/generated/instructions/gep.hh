#ifndef __HWMODEL_GEP_HH__
#define __HWMODEL_GEP_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Gep.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Gep: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Gep();
		Gep(const GepParams &params);
};
#endif // __HWMODEL_GEP_HH__