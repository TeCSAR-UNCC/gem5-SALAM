#include "landingpad.hh"

//AUTO-GENERATED FILE

Landingpad::Landingpad(const LandingpadParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
