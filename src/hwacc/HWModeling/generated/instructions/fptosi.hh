#ifndef __HWMODEL_FPTOSI_HH__
#define __HWMODEL_FPTOSI_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Fptosi.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Fptosi: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Fptosi();
		Fptosi(const FptosiParams &params);
};
#endif // __HWMODEL_FPTOSI_HH__