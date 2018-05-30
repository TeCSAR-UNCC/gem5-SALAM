#ifndef __REGISTER_HH__

#define __REGISTER_HH__

#include "debug.hh"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <list>

class Register {
friend class RegisterList;

private:
  std::string dataType;
  std::string name;
  bool hot = false;

public:
  uint64_t value;
  uint64_t size; // byte

  Register(std::string id);
  Register(std::string id, uint64_t val);
  ~Register();

  bool getStatus() { return hot; }
  std::string getName() { return name; }
  std::string getType() { return dataType; }
  uint64_t getValue() { return value; }
  void getValue(void *data) { memcpy(data, &value, size); }
  void commit() { hot = false; }
  void reset() { hot = true; }
  void setType(std::string type) { dataType = type; }
  void setSize();
  void setSize(std::string type);
  void setValue();
  void setValue(void *data);
};

class RegisterList{
private:
  std::list<Register *> *regList;

public:
  RegisterList() { regList = new std::list<Register *>(); }
  ~RegisterList();
  void addRegister(Register *reg) { regList->push_back(reg); }
  size_t size() { return regList->size(); }
  Register *findRegister(std::string name);
  void printRegNames();

protected:
};

#endif //__REGISTER_HH__
