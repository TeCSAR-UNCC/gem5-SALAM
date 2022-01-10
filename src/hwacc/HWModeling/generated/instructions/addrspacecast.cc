#include "addrspacecast.hh"

//AUTO-GENERATED FILE

Addrspacecast::Addrspacecast(const AddrspacecastParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
