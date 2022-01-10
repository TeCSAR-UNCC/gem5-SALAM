#ifndef __HWMODEL_RESUME_HH__
#define __HWMODEL_RESUME_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/Resume.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class Resume: public SimObject, public InstConfigBase
{
	private:
	protected:
	public:
		Resume();
		Resume(const ResumeParams &params);
};
#endif // __HWMODEL_RESUME_HH__