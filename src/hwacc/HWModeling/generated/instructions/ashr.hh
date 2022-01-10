#ifndef __HWMODEL_ASHR_HH__
#define __HWMODEL_ASHR_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Ashr.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Ashr: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Ashr();
		Ashr(const AshrParams &params);
};
#endif // __HWMODEL_ASHR_HH__