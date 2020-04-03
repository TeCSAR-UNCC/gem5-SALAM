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
#define NW				0x2f000021
#define DMA				0x2ff00000

// Specify the scratchpad addresses for variables
#define SPM				0x2f100000
#define SEQAADDR		SPM + 0
#define SEQBADDR		SPM + ALEN
#define ALIGNEDAADDR	SPM + 1 * (ALEN + BLEN)
#define ALIGNEDBADDR	SPM + 2 * (ALEN + BLEN)
#define MADDR			SPM + 3 * (ALEN + BLEN)
#define PTRADDR			SPM + 3 * (ALEN + BLEN) + 4*((ALEN + 1) * (BLEN + 1))