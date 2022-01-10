#include "udiv.hh"

//AUTO-GENERATED FILE

Udiv::Udiv(const UdivParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
