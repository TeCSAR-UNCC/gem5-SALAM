#include "ashr.hh"

//AUTO-GENERATED FILE

Ashr::Ashr(const AshrParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
