//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"

#define DEV_INIT		0x01
#define DEV_INTR		0x04

// Device MMR addresses
#define TOP				0x2f000000
#define FFT				0x2f000021
#define DMA				0x2ff00000

// Specify the scratchpad addresses for variables
#define SPM				0x2f100000
#define REALADDR		SPM + 0
#define IMGADDR			SPM + 1 * (FFT_SIZE * sizeof(double))
#define REALTWIDADDR	SPM + 2 * (FFT_SIZE * sizeof(double))
#define IMGTWIDADDR		SPM + 2 * (FFT_SIZE * sizeof(double)) + (FFT_HALF * sizeof(double))