#include "lshr.hh"

//AUTO-GENERATED FILE

Lshr::Lshr(const LshrParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
