#include "vaarg.hh"

//AUTO-GENERATED FILE

Vaarg::Vaarg(const VaargParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
