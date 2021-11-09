#ifndef __HWMODEL_FUNCTIONAL_UNITS_HH__
#define __HWMODEL_FUNCTIONAL_UNITS_HH__

#include "params/FunctionalUnits.hh"
#include "sim/sim_object.hh"
// GENERATED HEADERS - DO NOT MODIFY
#include "../generated/functionalunits/base.hh"
#include "../generated/functionalunits/double_multiplier.hh"
#include "../generated/functionalunits/bit_register.hh"
#include "../generated/functionalunits/bitwise_operations.hh"
#include "../generated/functionalunits/double_adder.hh"
#include "../generated/functionalunits/float_divider.hh"
#include "../generated/functionalunits/bit_shifter.hh"
#include "../generated/functionalunits/integer_multiplier.hh"
#include "../generated/functionalunits/integer_adder.hh"
#include "../generated/functionalunits/double_divider.hh"
#include "../generated/functionalunits/float_adder.hh"
#include "../generated/functionalunits/float_multiplier.hh"
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace gem5;

class FunctionalUnitBase;

class FunctionalUnits : public SimObject
{
	private:
	protected:

	public:
		// GENERATED CLASS MEMBERS - DO NOT MODIFY
		DoubleMultiplier* _double_multiplier;
		BitRegister* _bit_register;
		BitwiseOperations* _bitwise_operations;
		DoubleAdder* _double_adder;
		FloatDivider* _float_divider;
		BitShifter* _bit_shifter;
		IntegerMultiplier* _integer_multiplier;
		IntegerAdder* _integer_adder;
		DoubleDivider* _double_divider;
		FloatAdder* _float_adder;
		FloatMultiplier* _float_multiplier;
		FunctionalUnits();
		// DEFAULT CONSTRUCTOR - DO NOT MODIFY
		FunctionalUnits(const FunctionalUnitsParams &params);
		// END DEFAULT CONSTRUCTOR
		std::vector<FunctionalUnitBase*> functional_unit_list;
};
#endif //__HWMODEL_FUNCTIONAL_UNITS_HH__
