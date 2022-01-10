#include "frem.hh"

//AUTO-GENERATED FILE

Frem::Frem(const FremParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
