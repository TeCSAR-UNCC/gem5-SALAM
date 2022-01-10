#include "mul.hh"

//AUTO-GENERATED FILE

Mul::Mul(const MulParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
