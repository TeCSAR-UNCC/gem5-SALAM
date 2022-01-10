#include "indirectbr.hh"

//AUTO-GENERATED FILE

Indirectbr::Indirectbr(const IndirectbrParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
