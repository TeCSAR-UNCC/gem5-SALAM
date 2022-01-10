#ifndef __HWMODEL_FDIV_HH__
#define __HWMODEL_FDIV_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Fdiv.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Fdiv: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Fdiv();
		Fdiv(const FdivParams &params);
};
#endif // __HWMODEL_FDIV_HH__