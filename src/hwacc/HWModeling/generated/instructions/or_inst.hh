#ifndef __HWMODEL_OR_INST_HH__
#define __HWMODEL_OR_INST_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/OrInst.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class OrInst: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		OrInst();
		OrInst(const OrInstParams &params);
};
#endif // __HWMODEL_OR_INST_HH__