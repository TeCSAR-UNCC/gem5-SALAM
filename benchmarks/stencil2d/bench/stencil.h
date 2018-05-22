#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include "../defines.h"

//Define input sizes
#define col_size COL
#define row_size ROW
#define f_size FIL

void stencil( TYPE orig[row_size * col_size],
        TYPE sol[row_size * col_size],
        TYPE filter[f_size] );

////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t {
    TYPE orig[row_size*col_size];
    TYPE sol[row_size*col_size];
    TYPE filter[f_size];
};
