#ifndef __HWMODEL_SEXT_HH__
#define __HWMODEL_SEXT_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Sext.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Sext: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Sext();
		Sext(const SextParams &params);
};
#endif // __HWMODEL_SEXT_HH__