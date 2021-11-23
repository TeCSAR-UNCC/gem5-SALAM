#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../defines.h"

void md( int32_t n_points[blockSide][blockSide][blockSide],
         dvector_t force[blockSide][blockSide][blockSide][densityFactor],
         dvector_t position[blockSide][blockSide][blockSide][densityFactor]
       );
////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
  int32_t n_points[blockSide][blockSide][blockSide];
  dvector_t force[blockSide][blockSide][blockSide][densityFactor];
  dvector_t position[blockSide][blockSide][blockSide][densityFactor];
};

#define DEV_INIT	0x01
#define DEV_INTR	0x04

#define points_size nBlocks*sizeof(int32_t)
#define grid_size   nBlocks*densityFactor*sizeof(dvector_t)