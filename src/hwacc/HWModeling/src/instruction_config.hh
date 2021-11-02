#ifndef __HWMODEL_INSTRUCTION_CONFIG_HH__
#define __HWMODEL_INSTRUCTION_CONFIG_HH__

#include "params/InstConfig.hh"
#include "sim/sim_object.hh"
// GENERATED HEADERS - DO NOT MODIFY
#include "../generated/instructions/add.hh"
#include "../generated/instructions/addrspacecast.hh"
#include "../generated/instructions/alloca.hh"
#include "../generated/instructions/and_inst.hh"
#include "../generated/instructions/ashr.hh"
#include "../generated/instructions/bitcast.hh"
#include "../generated/instructions/br.hh"
#include "../generated/instructions/call.hh"
#include "../generated/instructions/fadd.hh"
#include "../generated/instructions/fcmp.hh"
#include "../generated/instructions/fdiv.hh"
#include "../generated/instructions/fence.hh"
#include "../generated/instructions/fmul.hh"
#include "../generated/instructions/fpext.hh"
#include "../generated/instructions/fptosi.hh"
#include "../generated/instructions/fptoui.hh"
#include "../generated/instructions/fptrunc.hh"
#include "../generated/instructions/frem.hh"
#include "../generated/instructions/fsub.hh"
#include "../generated/instructions/gep.hh"
#include "../generated/instructions/icmp.hh"
#include "../generated/instructions/indirectbr.hh"
#include "../generated/instructions/inttoptr.hh"
#include "../generated/instructions/invoke.hh"
#include "../generated/instructions/landingpad.hh"
#include "../generated/instructions/load.hh"
#include "../generated/instructions/lshr.hh"
#include "../generated/instructions/mul.hh"
#include "../generated/instructions/or_inst.hh"
#include "../generated/instructions/phi.hh"
#include "../generated/instructions/ptrtoint.hh"
#include "../generated/instructions/resume.hh"
#include "../generated/instructions/ret.hh"
#include "../generated/instructions/sdiv.hh"
#include "../generated/instructions/select.hh"
#include "../generated/instructions/sext.hh"
#include "../generated/instructions/shl.hh"
#include "../generated/instructions/srem.hh"
#include "../generated/instructions/store.hh"
#include "../generated/instructions/sub.hh"
#include "../generated/instructions/switch_inst.hh"
#include "../generated/instructions/trunc.hh"
#include "../generated/instructions/udiv.hh"
#include "../generated/instructions/uitofp.hh"
#include "../generated/instructions/unreachable.hh"
#include "../generated/instructions/urem.hh"
#include "../generated/instructions/vaarg.hh"
#include "../generated/instructions/xor_inst.hh"
#include "../generated/instructions/zext.hh"
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace gem5;

class InstConfig : public SimObject
{
	private:
	protected:

	public:
		// GENERATED CLASS MEMBERS - DO NOT MODIFY
		Add* _add;
		Addrspacecast* _addrspacecast;
		Alloca* _alloca;
		AndInst* _and_inst;
		Ashr* _ashr;
		Bitcast* _bitcast;
		Br* _br;
		Call* _call;
		Fadd* _fadd;
		Fcmp* _fcmp;
		Fdiv* _fdiv;
		Fence* _fence;
		Fmul* _fmul;
		Fpext* _fpext;
		Fptosi* _fptosi;
		Fptoui* _fptoui;
		Fptrunc* _fptrunc;
		Frem* _frem;
		Fsub* _fsub;
		Gep* _gep;
		Icmp* _icmp;
		Indirectbr* _indirectbr;
		Inttoptr* _inttoptr;
		Invoke* _invoke;
		Landingpad* _landingpad;
		Load* _load;
		Lshr* _lshr;
		Mul* _mul;
		OrInst* _or_inst;
		Phi* _phi;
		Ptrtoint* _ptrtoint;
		Resume* _resume;
		Ret* _ret;
		Sdiv* _sdiv;
		Select* _select;
		Sext* _sext;
		Shl* _shl;
		Srem* _srem;
		Store* _store;
		Sub* _sub;
		SwitchInst* _switch_inst;
		Trunc* _trunc;
		Udiv* _udiv;
		Uitofp* _uitofp;
		Unreachable* _unreachable;
		Urem* _urem;
		Vaarg* _vaarg;
		XorInst* _xor_inst;
		Zext* _zext;
		InstConfig();
		// DEFAULT CONSTRUCTOR - DO NOT MODIFY
		InstConfig(const InstConfigParams &params);
		// END DEFAULT CONSTRUCTOR

        // Needs to be added to generator script
        std::vector<InstConfigBase*> inst_list;
};
#endif //__INSTRUCTION_CONFIG_HH__
