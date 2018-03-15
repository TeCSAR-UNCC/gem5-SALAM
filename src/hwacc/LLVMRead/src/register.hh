#ifndef __REGISTER_HH__

#define __REGISTER_HH__

#include <cstdint>
#include <string>
#include <vector>
#include <list>
template <typename T>

class Register {
  friend class RegisterList;
  private:
    T value;
    std::string name;
    bool hot = true;
    //Register * next;
  public:
    Resigers() {value = 0;}
    Register(std::string id);
    Register(std::string id, T val);
    ~Register();
    
    T getValue() {
      return value;
    }
//    Register *getNext() {
//      return next;
//    }
    std::string getName() {
      return name;
    }
    void setValue(T val) {
      value = val;
    }
    void commit(){
      hot = false;
    }
    void reset(){
      hot = true;
    }
  protected:
  
};

template <typename T>
class RegisterList {
  private:
//    Register *head, *tail;
//    unsigned numRegisters;
    std::list<Register *> *regList;
  public:
    RegisterList() { regList = new std::list<Register *>(); }
    ~RegisterList();
    void addRegister(Register *reg) { regList->push_back(reg); }
    Register *findRegister(std::string name);
  protected:
  
};

#endif //__REGISTER_HH__
