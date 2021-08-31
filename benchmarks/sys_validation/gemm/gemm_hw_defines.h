//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//Specify row/column sizes
#define row_size 	ROW
#define col_size 	COL
#define mat_size	row_size*col_size*sizeof(TYPE)

//BEGIN GENERATED CODE
//Cluster: GEMM
//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//Accelerator: TOP
#define TOP 0x10020015
//Accelerator: GEMM
#define GEMM 0x1002002e
#define MATRIX1 0x1002002f
#define MATRIX2 0x1002802f
#define MATRIX3 0x1003002f
//END GENERATED CODE