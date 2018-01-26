#ifndef __HWACC_COMPUTE_UNIT_HH__

#define __HWACC_COMPUTE_UNIT_HH__

#include "params/ComputeUnit.hh"
#include "sim/sim_object.hh"
#include "hwacc/comm_interface.hh"

class ComputeUnit : public SimObject {
  private:

  protected:
    CommInterface *comm;
  public:
    ComputeUnit(ComputeUnitParams *p);
};

#endif //__HWACC_COMPUTE_UNIT_HH__
