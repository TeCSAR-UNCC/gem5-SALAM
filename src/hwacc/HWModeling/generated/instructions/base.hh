#ifndef __HWMODEL_INST_CONFIG_BASE_HH__
#define __HWMODEL_INST_CONFIG_BASE_HH__

#include <map>
#include <iostream>
#include <cstdlib>
#include <vector>

class InstConfigBase
{
	private:
	protected:
		uint32_t _functional_unit;
		uint32_t _functional_unit_limit;
		uint32_t _opcode_num;
		uint32_t _runtime_cycles;
	public:
		InstConfigBase();
		InstConfigBase( uint32_t functional_unit,
							uint32_t functional_unit_limit,
							uint32_t opcode_num,
							uint32_t runtime_cycles) :
							_functional_unit(functional_unit),
							_functional_unit_limit(functional_unit_limit),
							_opcode_num(opcode_num),
							_runtime_cycles(runtime_cycles) { }
		uint32_t get_functional_unit() { return _functional_unit; }
		uint32_t get_functional_unit_limit() { return _functional_unit_limit; }
		uint32_t get_opcode_num() { return _opcode_num; }
		uint32_t get_runtime_cycles() { return _runtime_cycles; }
};
#endif // __HWMODEL_INST_CONFIG_BASE_HH__