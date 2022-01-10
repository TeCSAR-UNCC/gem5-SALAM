#include "zext.hh"

//AUTO-GENERATED FILE

Zext::Zext(const ZextParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
