#include "store.hh"

//AUTO-GENERATED FILE

Store::Store(const StoreParams &params) :
	SimObject(params),
	InstConfigBase( params.functional_unit,
						params.functional_unit_limit,
						params.opcode_num,
						params.runtime_cycles) { }
