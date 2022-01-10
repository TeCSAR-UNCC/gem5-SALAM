#include "fmul.hh"

//AUTO-GENERATED FILE

Fmul::Fmul(const FmulParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
