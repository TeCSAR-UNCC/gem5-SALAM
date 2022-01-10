#ifndef __HWMODEL_XOR_INST_HH__
#define __HWMODEL_XOR_INST_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/XorInst.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class XorInst: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		XorInst();
		XorInst(const XorInstParams &params);
};
#endif // __HWMODEL_XOR_INST_HH__