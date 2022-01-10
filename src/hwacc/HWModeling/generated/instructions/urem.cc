#include "urem.hh"

//AUTO-GENERATED FILE

Urem::Urem(const UremParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
