//------------------------------------------//
#include "hwacc/acc_cluster.hh"
//------------------------------------------//

AccCluster::AccCluster(const AccClusterParams &p)
    : Platform(p), system(p.system), gic(nullptr)
{}

void
AccCluster::postConsoleInt()
{
    warn_once("Don't know what interrupt to post for console.\n");
    //panic("Need implementation\n");
}

void
AccCluster::clearConsoleInt()
{
    warn_once("Don't know what interrupt to clear for console.\n");
    //panic("Need implementation\n");
}

// AccCluster *
// AccClusterParams::create()
// {
//     return new AccCluster(this);
// }
