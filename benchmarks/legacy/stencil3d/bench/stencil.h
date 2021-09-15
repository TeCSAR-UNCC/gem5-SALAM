/*
Implementation based on algorithm described in:
"Stencil computation optimization and auto-tuning on state-of-the-art multicore architectures"
K. Datta, M. Murphy, V. Volkov, S. Williams, J. Carter, L. Oliker, D. Patterson, J. Shalf, K. Yelick
SC 2008
*/

#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include "../defines.h"

//Define input sizes
#define height_size HYT
#define col_size COL
#define row_size ROW

void stencil3d( TYPE C[C_SIZE], TYPE orig[SIZE], TYPE sol[SIZE] );

////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
  TYPE C[C_SIZE];
  TYPE orig[SIZE];
  TYPE sol[SIZE];
};
