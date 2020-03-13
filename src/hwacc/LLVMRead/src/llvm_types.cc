//------------------------------------------//
#include "llvm_types.hh"
//------------------------------------------//

LLVMType::LLVMType(int size, std::string name) {
    this->size = size;
    this->name = name;
}

LLVMType *
TypeList::findType(std::string name) {
    for (auto it = typeList->begin(); it != typeList->end(); ++it) {
        if ((*it)->getName().compare(name) == 0) return *it;
    }
    return NULL;
}