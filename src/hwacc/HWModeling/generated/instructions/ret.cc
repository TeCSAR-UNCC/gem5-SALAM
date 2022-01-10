#include "ret.hh"

//AUTO-GENERATED FILE

Ret::Ret(const RetParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
