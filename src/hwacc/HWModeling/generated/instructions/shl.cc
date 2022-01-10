#include "shl.hh"

//AUTO-GENERATED FILE

Shl::Shl(const ShlParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
