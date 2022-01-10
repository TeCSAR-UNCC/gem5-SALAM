#include "switch_inst.hh"

//AUTO-GENERATED FILE

SwitchInst::SwitchInst(const SwitchInstParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
