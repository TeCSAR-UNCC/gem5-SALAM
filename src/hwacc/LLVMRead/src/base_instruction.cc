//------------------------------------------//
#include "base_instruction.hh"
//------------------------------------------//

void
InstructionBase::setResult(void *Data) {
    // memcpy shortcut method
    memcpy(&_FinalResult, Data, _ReturnRegister->getSize());
}   //  --- End Function ----------------------------------------------//


bool
InstructionBase::commit() {
	// If cycle count is = max cycle count, commit register value to memory
	if(_ReturnRegister != NULL) {
		if (_CurrCycle == _MaxCycle) { 
			_ReturnRegister->setValue(&_FinalResult);
			_ReturnRegister->commit();
            _ReturnRegister->write();
			signalChildren();
			if (_debug) {
                DPRINTF(LLVMOp, "Operation Committed to Memory \n");
            }
			return true;
		} else if (_debug) {
            DPRINTF(LLVMOp, "Operation Will Commit in %d Cycle(s) \n", (_MaxCycle-_CurrCycle));
        }
       // _CurrCycle++;
    } else {
        signalChildren();
    }
	return false;
}

void
InstructionBase::configFU(uint8_t HardwareUnit, int32_t pipelined) {
    
    if (pipelined) {
        switch(HardwareUnit) {
            case COUNTER: _MaxStages = COUNTER_STAGES; break;
            case INTADDER: _MaxStages = INTADDER_STAGES; break;
            case INTMULTI: _MaxStages = INTMULTI_STAGES; break;
            case INTSHIFTER: _MaxStages = INTSHIFTER_STAGES; break;
            case INTBITWISE: _MaxStages = INTBITWISE_STAGES; break;
            case FPSPADDER: _MaxStages = FPSPADDER_STAGES; break;
            case FPDPADDER: _MaxStages = FPDPADDER_STAGES; break;
            case FPSPMULTI: _MaxStages = FPSPMULTI_STAGES; break;
            case FPSPDIVID: _MaxStages = FPSPDIVID_STAGES; break;
            case FPDPMULTI: _MaxStages = FPDPMULTI_STAGES; break;
            case FPDPDIVID:  _MaxStages = FPDPDIVID_STAGES; break;
            case COMPARE: _MaxStages = COMPARE_STAGES; break;
            case GETELEMENTPTR: _MaxStages = GETELEMENTPTR_STAGES; break;
            case CONVERSION: _MaxStages = CONVERSION_STAGES; break;
            case REGISTER: _MaxStages = REGISTER_STAGES; break;
            default: _MaxStages = OTHER_STAGES; break;
        }
    } else _MaxStages = 1;
    if (_MaxStages > 1) _Multistaged = true;
    int remainingStages;
    int remainingCycles = _MaxCycle;
    for (int i = 0; i < (_MaxStages); i++) {
        remainingStages = _MaxStages-i;
        _StageCycle.push_back(remainingCycles / remainingStages); 
        remainingCycles-=(remainingCycles / remainingStages);
    } 
}

std::vector<Register*>
InstructionBase::runtimeDependencies(std::string PrevBB) {
	return _Dependencies;
}


// Used by parents to signal children to fetch associated register after commit
void
InstructionBase::fetchDependency(Register* reg) {
    assert(_Ops.size()==_Dependencies.size());
    if (reg != _ReturnRegister) {
        for (int i = 0; i < _Dependencies.size(); i++) {
            if (reg == _Dependencies.at(i)) {
                _Ops.at(i) = _Dependencies.at(i)->getValue();
                if(!(reg->updated_this_cycle)) {
                    reg->read();
                    reg->update();
                }
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
    if(_ReturnRegister) {
        for (int i = 0; i < _Children.size(); i++) {
            _Children.at(i)->fetchDependency(_ReturnRegister);
        }
    } else {
        for (int i = 0; i < _Children.size(); i++) {
            _Children.at(i)->_ActiveParents--;
        }
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
