#ifndef __HWMODEL_FREM_HH__
#define __HWMODEL_FREM_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Frem.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Frem: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Frem();
		Frem(const FremParams &params);
};
#endif // __HWMODEL_FREM_HH__