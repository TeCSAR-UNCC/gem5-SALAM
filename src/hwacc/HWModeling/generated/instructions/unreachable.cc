#include "unreachable.hh"

//AUTO-GENERATED FILE

Unreachable::Unreachable(const UnreachableParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
