#include "fdiv.hh"

//AUTO-GENERATED FILE

Fdiv::Fdiv(const FdivParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
