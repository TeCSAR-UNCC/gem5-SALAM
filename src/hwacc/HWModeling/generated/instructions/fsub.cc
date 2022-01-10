#include "fsub.hh"

//AUTO-GENERATED FILE

Fsub::Fsub(const FsubParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
