#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//Define input sizes
#define col_size COL
#define row_size ROW
#define f_size FIL

// Device MMR addresses
#define TOP			0x2f000000
#define STENCIL		0x2f000019
#define DMA			0x2ff00000

// Specify the scratchpad addresses for variables
#define SPM			0x2f100000
#define ORIGADDR	SPM
#define SOLADDR		SPM + (row_size * col_size * sizeof(TYPE))
#define FILTERADDR	SPM + (row_size * col_size * sizeof(TYPE)) * 2