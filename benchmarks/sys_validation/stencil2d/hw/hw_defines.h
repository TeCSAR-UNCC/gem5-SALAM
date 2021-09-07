#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "../defines.h"
#include "../stencil2d_clstr_hw_defines.h"

#define DEV_INIT	0x01
#define DEV_INTR	0x04

//Define input sizes
#define col_size COL
#define row_size ROW
#define f_size FIL