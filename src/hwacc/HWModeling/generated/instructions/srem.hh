#ifndef __HWMODEL_SREM_HH__
#define __HWMODEL_SREM_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Srem.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Srem: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Srem();
		Srem(const SremParams &params);
};
#endif // __HWMODEL_SREM_HH__