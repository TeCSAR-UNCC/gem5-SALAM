//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"
#include "../gemm_clstr_hw_defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//Specify row/column sizes
#define row_size 	ROW
#define col_size 	COL
#define mat_size	row_size*col_size*sizeof(TYPE)