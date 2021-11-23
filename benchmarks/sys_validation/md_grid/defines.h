#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "stdint.h"
#include "md_grid_clstr_hw_defines.h"

//#define SPM
#define CHECK
//#define TEST

#ifdef TEST
#define TYPE double
// Problem Constants
#define nAtoms        64
#define domainEdge    20.0
#define blockSide     4
#define nBlocks       (blockSide*blockSide*blockSide)
#define blockEdge     (domainEdge/((TYPE)blockSide))
// Memory Bound
// This is an artifact of using statically-allocated arrays. We'll pretend that
// it doesn't exist and instead track the actual number of points.
#define densityFactor 1
// LJ coefficients
#define lj1           1.5
#define lj2           2.0

#else
#define TYPE double
// Problem Constants
#define nAtoms        256
#define domainEdge    20.0
#define blockSide     4
//#define blockSide     1
#define nBlocks       (blockSide*blockSide*blockSide)
#define blockEdge     (domainEdge/((TYPE)blockSide))
// Memory Bound
// This is an artifact of using statically-allocated arrays. We'll pretend that
// it doesn't exist and instead track the actual number of points.
#define densityFactor 10
// LJ coefficients
#define lj1           1.5
#define lj2           2.0


#endif

typedef struct {
  TYPE x, y, z;
} dvector_t;
typedef struct {
  int32_t x, y, z;
} ivector_t;

#define MIN(x,y) ( (x)<(y) ? (x) : (y) )
#define MAX(x,y) ( (x)>(y) ? (x) : (y) )

#endif
