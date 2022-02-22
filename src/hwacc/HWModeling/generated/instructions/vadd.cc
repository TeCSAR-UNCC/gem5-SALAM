#include "vadd.hh"

//AUTO-GENERATED FILE

Vadd::Vadd(const VaddParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
