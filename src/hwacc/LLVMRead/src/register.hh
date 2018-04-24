#ifndef __REGISTER_HH__

#define __REGISTER_HH__

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
  bool hot = true;
  //Register * next;
public:
  uint64_t value;
  uint64_t size; // byte

  Register(std::string id);
  Register(std::string id, uint64_t val);
  ~Register();

  bool getStatus() { return hot; }
  std::string getName() { return name; }
  void commit() { hot = false; }
  void reset() { hot = true; }
  std::string getType() { return dataType; }
  void setType(std::string type) { dataType = type; }
  void setSize();
  void setSize(std::string type);
  void getValue(void *data) { memcpy(&data, &value, size); }
  uint64_t getStoredValue() { return value; }
  void setValue();
  void setValue(void *data) { memcpy(&value, data, size); }

protected:
};

class RegisterList{
private:
  std::list<Register *> *regList;

public:
  RegisterList() { regList = new std::list<Register *>(); }
  ~RegisterList();
  void addRegister(Register *reg) { regList->push_back(reg); }
  Register *findRegister(std::string name);

protected:
};

#endif //__REGISTER_HH__
