#include "phi.hh"

//AUTO-GENERATED FILE

Phi::Phi(const PhiParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
