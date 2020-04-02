/*
Implementation based on algorithm described in:
"Stencil computation optimization and auto-tuning on state-of-the-art multicore architectures"
K. Datta, M. Murphy, V. Volkov, S. Williams, J. Carter, L. Oliker, D. Patterson, J. Shalf, K. Yelick
SC 2008
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//Define input sizes
#define height_size HYT
#define col_size 	COL
#define row_size 	ROW

// Device MMR addresses
#define TOP			0x2f000000
#define STENCIL		0x2f000019
#define DMA			0x2ff00000

// Specify the scratchpad addresses for variables
#define SPM			0x2f100000
#define CADDR		SPM
#define ORIGADDR	SPM + (C_SIZE * sizeof(TYPE))
#define SOLADDR		SPM + (C_SIZE * sizeof(TYPE)) + (SIZE * sizeof(TYPE))