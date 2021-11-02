#ifndef __HWMODEL_FUNCTIONAL_UNITS_HH__
#define __HWMODEL_FUNCTIONAL_UNITS_HH__

#include "params/FunctionalUnits.hh"
#include "sim/sim_object.hh"
// GENERATED HEADERS - DO NOT MODIFY
#include "../generated/functionalunits/base.hh"
#include "../generated/functionalunits/float_divider.hh"
#include "../generated/functionalunits/float_multiplier.hh"
#include "../generated/functionalunits/double_multiplier.hh"
#include "../generated/functionalunits/float_adder.hh"
#include "../generated/functionalunits/bitwise_operations.hh"
#include "../generated/functionalunits/bit_shifter.hh"
#include "../generated/functionalunits/integer_adder.hh"
#include "../generated/functionalunits/bit_register.hh"
#include "../generated/functionalunits/integer_multiplier.hh"
#include "../generated/functionalunits/double_divider.hh"
#include "../generated/functionalunits/double_adder.hh"
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
		FloatDivider* _float_divider;
		FloatMultiplier* _float_multiplier;
		DoubleMultiplier* _double_multiplier;
		FloatAdder* _float_adder;
		BitwiseOperations* _bitwise_operations;
		BitShifter* _bit_shifter;
		IntegerAdder* _integer_adder;
		BitRegister* _bit_register;
		IntegerMultiplier* _integer_multiplier;
		DoubleDivider* _double_divider;
		DoubleAdder* _double_adder;
		FunctionalUnits();
		// DEFAULT CONSTRUCTOR - DO NOT MODIFY
		FunctionalUnits(const FunctionalUnitsParams &params);
		// END DEFAULT CONSTRUCTOR
		std::vector<FunctionalUnitBase*> functional_unit_list;
};
#endif //__HWMODEL_FUNCTIONAL_UNITS_HH__
