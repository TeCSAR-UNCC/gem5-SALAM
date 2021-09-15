#ifndef __HWACC_ACC_CLUSTER_HH__
#define __HWACC_ACC_CLUSTER_HH__
//------------------------------------------//
#include "hwacc/LLVMRead/src/debug_flags.hh"
#include "dev/platform.hh"
#include "params/AccCluster.hh"
//------------------------------------------//
using namespace gem5;

class BaseGic;
class IdeController;
class System;

class AccCluster : public Platform
{
  public:
    /** Pointer to the system */
    gem5::System *system;

    BaseGic *gic;

  public:
    // typedef AccClusterParams Params;
    // const Params *
    // params() const {
    //     return dynamic_cast<const Params *>(_params);
    // }
    /**
     * Constructor for the Tsunami Class.
     * @param name name of the object
     * @param s system the object belongs to
     * @param intctrl pointer to the interrupt controller
     */
    PARAMS(AccCluster);
    AccCluster(const AccClusterParams &p);

    /** Give platform a pointer to interrupt controller */
    void setGic(BaseGic *_gic) { gic = _gic; }

  public: // Public Platform interfaces
    void postConsoleInt() override;
    void clearConsoleInt() override;
};

#endif
