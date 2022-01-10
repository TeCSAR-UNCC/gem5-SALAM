#include "double_divider.hh"

//AUTO-GENERATED FILE

DoubleDivider::DoubleDivider(const DoubleDividerParams &params) :
	SimObject(params),
	FunctionalUnitBase( params.alias,
						params.stages,
						params.cycles,
						params.enum_value,
						params.int_size,
						params.int_sign,
						params.int_apmode,
						params.fp_size,
						params.fp_sign,
						params.fp_apmode,
						params.ptr_size,
						params.ptr_sign,
						params.ptr_apmode,
						params.limit,
						params.power_units,
						params.energy_units,
						params.time_units,
						params.area_units,
						params.fu_latency,
						params.internal_power,
						params.switch_power,
						params.dynamic_power,
						params.dynamic_energy,
						params.leakage_power,
						params.area,
						params.path_delay) { }
