//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//Specify row/column sizes

// Device MMR addresses
#define TOP			0x2f000000
#define SORT		0x2f000019
#define DMA			0x2ff00000

// Specify the scratchpad addresses for variables
#define SPM			0x2f100000
#define ARRAY		SPM
#define TEMP		SPM + SIZE
