#include "alloca.hh"

//AUTO-GENERATED FILE

Alloca::Alloca(const AllocaParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
