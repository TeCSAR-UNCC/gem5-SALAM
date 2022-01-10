#include "srem.hh"

//AUTO-GENERATED FILE

Srem::Srem(const SremParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
