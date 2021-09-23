#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "mdknn_clstr_hw_defines.h"

#define CHECK

#define TYPE double
// Problem Constants
#define nAtoms        256
#define maxNeighbors  16
// LJ coefficients
#define lj1           1.5
#define lj2           2.0

#define DEV_INIT      0x01
#define DEV_INTR      0x04

#define ARRAY_SIZE    nAtoms * sizeof(TYPE)
#define NL_SIZE       nAtoms * maxNeighbors * sizeof(int32_t)

#endif