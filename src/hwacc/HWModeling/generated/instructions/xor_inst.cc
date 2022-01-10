#include "xor_inst.hh"

//AUTO-GENERATED FILE

XorInst::XorInst(const XorInstParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
