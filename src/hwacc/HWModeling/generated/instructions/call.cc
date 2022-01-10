#include "call.hh"

//AUTO-GENERATED FILE

Call::Call(const CallParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
