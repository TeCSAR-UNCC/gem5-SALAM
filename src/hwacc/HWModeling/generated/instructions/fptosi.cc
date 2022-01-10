#include "fptosi.hh"

//AUTO-GENERATED FILE

Fptosi::Fptosi(const FptosiParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
