#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "spmv_clstr_hw_defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//#define SPM
#define CHECK
//#define TEST

#ifndef TEST
// These constants valid for the IEEE 494 bus interconnect matrix
#define NNZ 1666
#define N 494
#define TYPE double

#else
#define NNZ 19
#define N 7
#define TYPE double
#endif

#endif
