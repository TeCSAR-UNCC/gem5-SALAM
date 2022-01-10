#include "resume.hh"

//AUTO-GENERATED FILE

Resume::Resume(const ResumeParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
