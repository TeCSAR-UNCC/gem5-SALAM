/*
Implemenataion based on:
A. Danalis, G. Marin, C. McCurdy, J. S. Meredith, P. C. Roth, K. Spafford, V. Tipparaju, and J. S. Vetter.
The scalable heterogeneous computing (shoc) benchmark suite.
In Proceedings of the 3rd Workshop on General-Purpose Computation on Graphics Processing Units, 2010.
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define DEV_INIT      0x01
#define DEV_INTR      0x04

#define ARRAY_SIZE    nAtoms * sizeof(TYPE)
#define NL_SIZE       nAtoms * maxNeighbors * sizeof(int32_t)

// Device MMR addresses
#define TOP           0x2f000000
#define MD            0x2f000039
#define DMA           0x2ff00000

// Specify the scratchpad addresses for variables
#define SPM           0x2f100000
#define FORCEXADDR    SPM + (0 * ARRAY_SIZE)
#define FORCEYADDR    SPM + (1 * ARRAY_SIZE)
#define FORCEZADDR    SPM + (2 * ARRAY_SIZE)
#define POSITIONXADDR SPM + (3 * ARRAY_SIZE)
#define POSITIONYADDR SPM + (4 * ARRAY_SIZE)
#define POSITIONZADDR SPM + (5 * ARRAY_SIZE)
#define NLADDR        SPM + (6 * ARRAY_SIZE)
#define END           SPM + (6 * ARRAY_SIZE) + (NL_SIZE)

// TYPE force_x[nAtoms]
// TYPE force_y[nAtoms]
// TYPE force_z[nAtoms]
// TYPE position_x[nAtoms]
// TYPE position_y[nAtoms]
// TYPE position_z[nAtoms]
// int32_t NL[nAtoms*maxNeighbors]