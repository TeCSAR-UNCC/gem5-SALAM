#include "br.hh"

//AUTO-GENERATED FILE

Br::Br(const BrParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
