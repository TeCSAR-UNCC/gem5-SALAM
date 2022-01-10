#include "fadd.hh"

//AUTO-GENERATED FILE

Fadd::Fadd(const FaddParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
