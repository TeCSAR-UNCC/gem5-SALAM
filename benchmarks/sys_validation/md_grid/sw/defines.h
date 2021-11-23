#include "../defines.h"

volatile int stage;

void runCPUBench( int n_points[blockSide][blockSide][blockSide],
         dvector_t force[blockSide][blockSide][blockSide][densityFactor],
         dvector_t position[blockSide][blockSide][blockSide][densityFactor] );

int checkResults(dvector_t force[blockSide][blockSide][blockSide][densityFactor],
                  dvector_t check[blockSide][blockSide][blockSide][densityFactor]);

void makeCheck(void * npoints, void * force, void * position);
int runCheck(void * force, void * check);