#include "trunc.hh"

//AUTO-GENERATED FILE

Trunc::Trunc(const TruncParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
