#include "ptrtoint.hh"

//AUTO-GENERATED FILE

Ptrtoint::Ptrtoint(const PtrtointParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
