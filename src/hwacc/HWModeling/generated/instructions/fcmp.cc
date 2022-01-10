#include "fcmp.hh"

//AUTO-GENERATED FILE

Fcmp::Fcmp(const FcmpParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
