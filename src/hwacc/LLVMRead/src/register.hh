#ifndef __REGISTER_HH__

#define __REGISTER_HH__

#include "debugFlags.hh"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <list>

struct RegisterTotals {
  int reads = 0;
  int writes = 0;
  int count = 0;
  int wordSize = 0;
};

class Register {
friend class RegisterList;

private:
  std::string dataType;
  std::string name;
  int writeCount;
  int readCount;
  int polledCount;
  bool hot = false;

public:
  uint64_t value;
  uint64_t size; // byte

  Register(std::string id);
  Register(std::string id, uint64_t val);
  ~Register();


  bool getStatus() { polled(); return hot; }
  std::string getName() { return name; }
  std::string getType() { return dataType; }
  uint64_t getValue() { return value; }
  int getSize() { return size; }
  void getValue(void *data) { memcpy(data, &value, size); }
  void commit() { hot = false; }
  void reset() { hot = true; }
  void setType(std::string type) { dataType = type; }
  void setSize();
  void setSize(std::string type);
  void setValue();
  void setValue(void *data);
  void accessedWrite() { writeCount++; }
  void accessedRead() { readCount++; }
  void polled() { polledCount++;}
  int getRead() { return readCount; }
  int getWrite() { return writeCount; }
  int getPoll() { return polledCount; }
};

class RegisterList{
private:
  std::list<Register *> *regList;
  RegisterTotals regTotals;

public:
  RegisterList() { regList = new std::list<Register *>(); }
  ~RegisterList();
  void addRegister(Register *reg) { regList->push_back(reg); }
  size_t size() { return regList->size(); }
  Register *findRegister(std::string name);
  void printRegNames();
  std::list<Register *>::iterator beginit() { auto it = regList->begin(); return it; }
  std::list<Register *>::iterator endit() {  auto it = regList->end(); return it; }
  void calculateTotals();
  int getReads() { return regTotals.reads; }
  int getWrites() { return regTotals.writes; }
  int getCount() { return regTotals.count; }
  int getWordSize() { return regTotals.wordSize; }
};

#endif //__REGISTER_HH__
