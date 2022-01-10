#include "sdiv.hh"

//AUTO-GENERATED FILE

Sdiv::Sdiv(const SdivParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
