#include "icmp.hh"

//AUTO-GENERATED FILE

Icmp::Icmp(const IcmpParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
