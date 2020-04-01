//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//Specify row/column sizes
#define row_size 	ROW
#define col_size 	COL
#define mat_size	row_size*col_size*sizeof(TYPE)

// Device MMR addresses
#define TOP			0x2f000000
#define GEMM		0x2f000019
#define DMA			0x2ff00000

// Specify the scratchpad addresses for variables
#define SPM			0x2f100000
#define M1ADDR		SPM + (mat_size * 0)
#define M2ADDR		SPM + (mat_size * 1)
#define M3ADDR		SPM + (mat_size * 2)