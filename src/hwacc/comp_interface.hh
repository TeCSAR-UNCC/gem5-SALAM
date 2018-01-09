#ifndef __HWACC_COMP_INTERFACE_HH__

#define __HWACC_COMP_INTERFACE_HH__

#include "params/CompInterface.hh"
#include "sim/sim_object.hh"
#include "hwacc/io_acc.hh"

class CompInterface : public SimObject {
  private:
    IOAcc *acc;

  protected:
  
  public:
    CompInterface(CompInterfaceParams *p);
    void memRead(int varID, size_t offset, size_t length);
    uint8_t* readSync();
    void memWrite(int varID, size_t offset, size_t length, uint8_t data[]);
    void writeSync();
    void finish();
    void runApp();
    uint8_t* configRead(size_t offset);
    void configWrite(size_t offset);

};

#endif //__HWACC_COMP_INTERFACE_HH__
