#include "inttoptr.hh"

//AUTO-GENERATED FILE

Inttoptr::Inttoptr(const InttoptrParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
