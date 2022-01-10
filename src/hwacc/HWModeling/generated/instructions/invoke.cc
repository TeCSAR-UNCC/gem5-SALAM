#include "invoke.hh"

//AUTO-GENERATED FILE

Invoke::Invoke(const InvokeParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
