#ifndef __HWMODEL_BIT_REGISTER_HH__
#define __HWMODEL_BIT_REGISTER_HH__

// GENERATED FILE - DO NOT MODIFY

#include "params/BitRegister.hh"
#include "sim/sim_object.hh"
#include "base.hh"

using namespace gem5;

class BitRegister: public SimObject, public FunctionalUnitBase
{
	private:
	protected:
	public:
		BitRegister();
		BitRegister(const BitRegisterParams &params);
};
#endif // __HWMODEL_BIT_REGISTER_HH__