#ifndef __HWMODEL_LOAD_HH__
#define __HWMODEL_LOAD_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Load.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Load: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Load();
		Load(const LoadParams &params);
};
#endif // __HWMODEL_LOAD_HH__