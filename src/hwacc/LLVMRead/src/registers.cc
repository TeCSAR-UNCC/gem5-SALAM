#include "registers.hh"

void 
Register::setSize(const std::string& Data_Type) {
    // --- Store data type as register parameters ---------------------//
    _Data_Type = Data_Type;
    //  --- Determine needed register size based around data type -----//
    //  --- Pointer Types ---------------------------------------------//
    if (_Data_Type.compare("pointer") == COMPAREFOUND) _Size = POINTERSIZE;
    else if (_Data_Type.back() == '*') _Size = POINTERSIZE;
    //  --- Integer Types ---------------------------------------------//
    else if (_Data_Type.front() == 'i') _Size = BYTESIZE(std::stoi(_Data_Type.substr(SKIPFIRST)));
    //  --- Floating Point Types --------------------------------------//
    else if (_Data_Type.compare("float") == COMPAREFOUND) _Size = FLOATSIZE;
    else if (_Data_Type.compare("double") == COMPAREFOUND) _Size = DOUBLESIZE;
    //  --- Void Type -------------------------------------------------//
    else if (_Data_Type.compare("void") == COMPAREFOUND) _Size = VOIDSIZE;
    //  --- LLVM Label Type -------------------------------------------//
    else if (_Data_Type.compare("label") == COMPAREFOUND) _Size = LABELSIZE;
    //  --- Unspecified or Unknown Data Type --------------------------//
    else _Size = DEFAULTSIZE;
}   //  --- End Function ----------------------------------------------//

void
Register::setValue(void *Data) { // memcpy shortcut method
    memcpy(&_Value, Data, _Size);
}   //  --- End Function ----------------------------------------------//

void
RegisterList::printRegNames() { // Prints name of all current registers
    for (auto it=_RegList->begin(); it!=_RegList->end(); ++it) {
        std::cout << (*it)->getName() << "\n";
    }
}   //  --- End Function ----------------------------------------------//

void
RegisterList::totalAccess(Reg_Usage* regUsage) { // Prints name of all current registers
    for (auto it=_RegList->begin(); it!=_RegList->end(); ++it) {
        regUsage->reads += (*it)->_Reg_Usage.reads;
        regUsage->writes += (*it)->_Reg_Usage.writes;
    }
}   //  --- End Function ----------------------------------------------//


Register *
RegisterList::findRegister(std::string Name) {
    for (auto it = _RegList->begin(); it != _RegList->end(); ++it) {
        if ((*it)->getName() == Name) return *it;
    }
    return NULL;
}