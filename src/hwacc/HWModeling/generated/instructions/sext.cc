#include "sext.hh"

//AUTO-GENERATED FILE

Sext::Sext(const SextParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
