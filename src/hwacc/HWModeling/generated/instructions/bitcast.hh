#ifndef __HWMODEL_BITCAST_HH__
#define __HWMODEL_BITCAST_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Bitcast.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Bitcast: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Bitcast();
		Bitcast(const BitcastParams &params);
};
#endif // __HWMODEL_BITCAST_HH__