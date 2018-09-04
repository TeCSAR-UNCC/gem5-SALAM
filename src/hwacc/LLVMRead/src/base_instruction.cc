#include "base_instruction.hh"

void
InstructionBase::setResult(void *Data) { // memcpy shortcut method
    memcpy(&_FinalResult, Data, _ReturnRegister->getSize());
}   //  --- End Function ----------------------------------------------//


bool 
InstructionBase::commit() {
	// If cycle count is = max cycle count, commit register value to memory
	if (_ReturnRegister != NULL) {
		_CurrCycle++;
		if (_CurrCycle >= _MaxCycle) {
			_ReturnRegister->setValue(&_FinalResult);
			_ReturnRegister->commit();
			return true;
		} else DPRINTF(LLVMRegister, "Cycle Incomplete!\n\n");
	}
	return false;
}



bool 
InstructionBase::checkDependencies() {
    bool hot = false;
//	bool phiBranchDependent = false;
	if(_Dependencies.size() == 0) DPRINTF(LLVMRegister, "No Dependencies!\n");
	if(_OpCode == "phi"){
		/*
		for (int i = 0; i < MAXPHI; i++) {
			if (prevBB == instruction.other.phi.label[i]) {
				if(!instruction.other.phi.immVal[i]) {
					phiBranchDependent = true;
					if(instruction.other.phi.val[i]->getStatus()){
						DPRINTF(LLVMRegister, "Register (%s) is Hot:\n", instruction.other.phi.val[i]->getName());
						hot = true;
					} else {
						DPRINTF(LLVMRegister, "Register (%s) is Ready:\n", instruction.other.phi.val[i]->getName());
					}
				} else {
					DPRINTF(LLVMRegister, "Immediate Value (%d) Loaded!\n", instruction.other.phi.val[i]);
				}
			}
		}
		if(!phiBranchDependent) DPRINTF(LLVMRegister, "No Dependencies!\n");
		*/
	} else {
		for (int i = 0; i < _Dependencies.size(); i++) {
			// Increment counter for register dependency check
			if (_Dependencies.at(i)->getStatus()) {
				hot = true;
			} 
		}
	}
	return hot;
}

// Used by parents to signal children to fetch associated register after commit
void
InstructionBase::fetchDependency(Register* reg) {
    assert(_Ops.size()==_Dependencies.size());
    if (reg != _ReturnRegister) {
        for (int i = 0; i < _Dependencies.size(); i++) {
            if (reg == _Dependencies.at(i)) {
                _Ops.at(i) = _Dependencies.at(i)->getValue();
            }
        }
    }
    _ActiveParents--;
}

// Used during scheduling to fetch dependency register value when parent isn't
// found in any active compute or scheduling queue
void
InstructionBase::fetchDependency(int depidx) {
    _Ops.at(depidx) = _Dependencies.at(depidx)->getValue();
}

// Tell all children nodes to fetch the new value in _ReturnRegister
void
InstructionBase::signalChildren() {
    assert(_ReturnRegister);
    for (int i = 0; i < _Children.size(); i++) {
        _Children.at(i)->fetchDependency(_ReturnRegister);
    }
}

// Adds a child node to the current node
void
InstructionBase::registerChild(InstructionBase* child) {
    _Children.push_back(child);
}

// Adds a parent to the current node and increments number of active parents
void
InstructionBase::registerParent(InstructionBase* parent) {
    _Parents.push_back(parent);
    _ActiveParents++;
}
