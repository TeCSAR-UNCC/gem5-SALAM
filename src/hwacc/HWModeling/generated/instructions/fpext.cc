#include "fpext.hh"

//AUTO-GENERATED FILE

Fpext::Fpext(const FpextParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
