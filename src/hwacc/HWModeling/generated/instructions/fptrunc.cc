#include "fptrunc.hh"

//AUTO-GENERATED FILE

Fptrunc::Fptrunc(const FptruncParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
