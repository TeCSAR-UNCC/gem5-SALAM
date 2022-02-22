#include "vsub.hh"

//AUTO-GENERATED FILE

Vsub::Vsub(const VsubParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
