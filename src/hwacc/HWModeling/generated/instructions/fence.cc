#include "fence.hh"

//AUTO-GENERATED FILE

Fence::Fence(const FenceParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
