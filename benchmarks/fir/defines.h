#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "stdint.h"

// #define SPM
#define CHECK
#define TEST

#ifdef TEST
#define INPUTSIZE 32
#define TAPS 16
#define EXPECTED_TOTAL 44880

#else
#define INPUTSIZE 32
#define TAPS 16
#define EXPECTED_TOTAL 44880

#endif
#endif
