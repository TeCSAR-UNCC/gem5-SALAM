#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SPM
//#define CHECK
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
