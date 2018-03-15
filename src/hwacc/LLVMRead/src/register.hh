#ifndef __REGISTER_HH__

#define __REGISTER_HH__

#include <cstdint>
#include <string>
#include <vector>
#include <list>

class Register {
  friend class RegisterList;
  private:
    int value;
    std::string name;
    bool hot = true;
    //Register * next;
  public:
    Register(std::string id);
    Register(std::string id, int val);
    ~Register();
    
    int getValue() {
      return value;
    }
    bool getStatus(){
      return hot;
    }
//    Register *getNext() {
//      return next;
//    }
    std::string getName() {
      return name;
    }
    void setValue(int val) {
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
