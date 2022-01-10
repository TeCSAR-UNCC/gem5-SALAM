#ifndef __HWMODEL_ICMP_HH__
#define __HWMODEL_ICMP_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Icmp.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Icmp: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Icmp();
		Icmp(const IcmpParams &params);
};
#endif // __HWMODEL_ICMP_HH__