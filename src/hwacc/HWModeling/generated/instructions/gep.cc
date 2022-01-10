#include "gep.hh"

//AUTO-GENERATED FILE

Gep::Gep(const GepParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
