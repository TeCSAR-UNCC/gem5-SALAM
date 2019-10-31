#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "stdint.h"

#define SPM
//#define CHECK
//#define TEST

#ifdef TEST
#define TYPE double
// Problem Constants
#define nAtoms        256
#define maxNeighbors  16
// LJ coefficients
#define lj1           1.5
#define lj2           2.0

#else
#define TYPE double
// Problem Constants
#define nAtoms        256
#define maxNeighbors  16
// LJ coefficients
#define lj1           1.5
#define lj2           2.0
#endif
#endif
