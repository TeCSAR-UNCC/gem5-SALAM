#include "bitcast.hh"

//AUTO-GENERATED FILE

Bitcast::Bitcast(const BitcastParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
