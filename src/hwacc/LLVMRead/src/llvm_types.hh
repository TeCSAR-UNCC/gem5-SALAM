#ifndef __LLVM_TYPES_HH__

#define __LLVM_TYPES_HH__

#include <string>
#include <list>
#include "debugFlags.hh"

class LLVMType {
friend class TypeList;
  private: 
    int size;
    std::string name;

  public: 
    LLVMType(int size, std::string name);
    std::string getName() {return name;}
    int getSize() {return size;}
};

class TypeList {
  private: 
    std::list<LLVMType*> *typeList;

  public:
    TypeList() { typeList = new std::list<LLVMType*>(); }
    void addType(LLVMType *type) { typeList->push_back(type); }
    LLVMType *findType(std::string name); 
};
#endif
