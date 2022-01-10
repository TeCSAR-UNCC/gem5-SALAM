#ifndef __HWMODEL_ZEXT_HH__
#define __HWMODEL_ZEXT_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Zext.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Zext: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Zext();
		Zext(const ZextParams &params);
};
#endif // __HWMODEL_ZEXT_HH__