#ifndef __HWMODEL_FUNCTIONAL_UNIT_BASE_HH__
#define __HWMODEL_FUNCTIONAL_UNIT_BASE_HH__

#include "../../src/salam_power_model.hh"

#include <map>
#include <iostream>
#include <cstdlib>
#include <vector>

class FunctionalUnitBase
{
	private:
	protected:
		std::string _alias;
		uint32_t _stages;
		uint32_t _cycles;
		uint32_t _enum_value;
		std::string _int_size;
		std::string _int_sign;
		bool _int_apmode;
		std::string _fp_size;
		std::string _fp_sign;
		bool _fp_apmode;
		std::string _ptr_size;
		std::string _ptr_sign;
		bool _ptr_apmode;
		uint32_t _limit;
		std::string _power_units;
		std::string _energy_units;
		std::string _time_units;
		std::string _area_units;
		uint32_t _fu_latency;
		double _internal_power;
		double _switch_power;
		double _dynamic_power;
		double _dynamic_energy;
		double _leakage_power;
		double _area;
		double _path_delay;

		uint64_t _available;

		uint64_t _in_use = 0;

	public:
		FunctionalUnitBase();
		FunctionalUnitBase( std::string alias,
							uint32_t stages,
							uint32_t cycles,
							uint32_t enum_value,
							std::string int_size,
							std::string int_sign,
							bool int_apmode,
							std::string fp_size,
							std::string fp_sign,
							bool fp_apmode,
							std::string ptr_size,
							std::string ptr_sign,
							bool ptr_apmode,
							uint32_t limit,
							std::string power_units,
							std::string energy_units,
							std::string time_units,
							std::string area_units,
							uint32_t fu_latency,
							double internal_power,
							double switch_power,
							double dynamic_power,
							double dynamic_energy,
							double leakage_power,
							double area,
							double path_delay) :
							_alias(alias),
							_stages(stages),
							_cycles(cycles),
							_enum_value(enum_value),
							_int_size(int_size),
							_int_sign(int_sign),
							_int_apmode(int_apmode),
							_fp_size(fp_size),
							_fp_sign(fp_sign),
							_fp_apmode(fp_apmode),
							_ptr_size(ptr_size),
							_ptr_sign(ptr_sign),
							_ptr_apmode(ptr_apmode),
							_limit(limit),
							_power_units(power_units),
							_energy_units(energy_units),
							_time_units(time_units),
							_area_units(area_units),
							_fu_latency(fu_latency),
							_internal_power(internal_power),
							_switch_power(switch_power),
							_dynamic_power(dynamic_power),
							_dynamic_energy(dynamic_energy),
							_leakage_power(leakage_power),
							_area(area),
							_path_delay(path_delay) { }
		std::string get_alias() { return _alias; }
		uint32_t get_stages() { return _stages; }
		uint32_t get_cycles() { return _cycles; }
		uint64_t get_in_use() { return _in_use; }
		uint32_t get_enum_value() { return _enum_value; }
		std::string get_int_size() { return _int_size; }
		std::string get_int_sign() { return _int_sign; }
		bool get_int_apmode() { return _int_apmode; }
		std::string get_fp_size() { return _fp_size; }
		std::string get_fp_sign() { return _fp_sign; }
		bool get_fp_apmode() { return _fp_apmode; }
		std::string get_ptr_size() { return _ptr_size; }
		std::string get_ptr_sign() { return _ptr_sign; }
		bool get_ptr_apmode() { return _ptr_apmode; }
		uint32_t get_limit() { return _limit; }
		std::string get_power_units() { return _power_units; }
		std::string get_energy_units() { return _energy_units; }
		std::string get_time_units() { return _time_units; }
		std::string get_area_units() { return _area_units; }
		uint32_t get_fu_latency() { return _fu_latency; }
		double get_internal_power() { return _internal_power; }
		double get_switch_power() { return _switch_power; }
		double get_dynamic_power() { return _dynamic_power; }
		double get_dynamic_energy() { return _dynamic_energy; }
		double get_leakage_power() { return _leakage_power; }
		double get_area() { return _area; }
		double get_path_delay() { return _path_delay; }
		bool is_available() { return (_in_use >= _available); }
		void use_functional_unit() { _in_use++; }
		void clear_functional_unit() { _in_use--; }
		void set_functional_unit_limit(uint64_t available) { _available = available; }
		void inc_functional_unit_limit() { _available++; }
		uint64_t get_functional_unit_limit() { return _available; }

};
#endif // __HWMODEL_FUNCTIONAL_UNIT_BASE_HH__