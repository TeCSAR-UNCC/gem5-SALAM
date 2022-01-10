#include "or_inst.hh"

//AUTO-GENERATED FILE

OrInst::OrInst(const OrInstParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
