#ifndef __HWMODEL_STORE_HH__
#define __HWMODEL_STORE_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Store.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Store: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Store();
		Store(const StoreParams &params);
};
#endif // __HWMODEL_STORE_HH__