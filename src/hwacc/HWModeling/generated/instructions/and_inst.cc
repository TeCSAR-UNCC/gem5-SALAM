#include "and_inst.hh"

//AUTO-GENERATED FILE

AndInst::AndInst(const AndInstParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
