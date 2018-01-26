#ifndef __REGISTER_HH__

#define __REGISTER_HH__

#include <cstdint>
#include <string>
#include <vector>

class Register {
  friend class RegisterList;
  private:
    uint64_t value;
    std::string name;
    Register * next;
  public:
    Register(std::string id);
    Register(std::string id, uint64_t val);
    ~Register();
    
    uint64_t getValue() {
      return value;
    }
    Register *getNext() {
      return next;
    }
    std::string getName() {
      return name;
    }
    void setValue(uint64_t val) {
      value = val;
    }
  protected:
  
};

class RegisterList {
  private:
    Register *head, *tail;
    unsigned numRegisters;
  public:
    RegisterList();
    ~RegisterList();
    void addRegister(Register *reg);
    Register *findRegister(std::string name);
  protected:
  
};

#endif //__REGISTER_HH__
