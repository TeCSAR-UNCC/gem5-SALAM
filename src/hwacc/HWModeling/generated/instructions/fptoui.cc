#include "fptoui.hh"

//AUTO-GENERATED FILE

Fptoui::Fptoui(const FptouiParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
