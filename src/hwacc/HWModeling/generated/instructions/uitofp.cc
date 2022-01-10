#include "uitofp.hh"

//AUTO-GENERATED FILE

Uitofp::Uitofp(const UitofpParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
