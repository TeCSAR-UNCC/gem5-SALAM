#ifndef __HWMODEL_UDIV_HH__
#define __HWMODEL_UDIV_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Udiv.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Udiv: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Udiv();
		Udiv(const UdivParams &params);
};
#endif // __HWMODEL_UDIV_HH__