#ifndef __HWMODEL_SDIV_HH__
#define __HWMODEL_SDIV_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Sdiv.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Sdiv: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Sdiv();
		Sdiv(const SdivParams &params);
};
#endif // __HWMODEL_SDIV_HH__