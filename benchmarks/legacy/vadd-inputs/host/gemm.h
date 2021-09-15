#include "../defines.h"

#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int *)0x2f000001
#define val_b      *(int *)0x2f000009
#define val_c      *(int *)0x2f000011
#define rcIndex     (r*ROW + c)

typedef struct {
    TYPE * a;
    TYPE * b;
    TYPE * c;
    int row_size;
    int col_size;
} gemm_struct;

void genData(gemm_struct * ges) {
    // int r, c, k, mult, sum;

    // for( r=0; r < ges->row_size; r++ ) {
    //     for( c=0; c < ges->col_size; c++ ) {
    //         ges->a[rcIndex] = rcIndex;
    //         ges->b[rcIndex] = (ges->row_size * ges->col_size) - 1 - rcIndex;
    //     }
    // }
}

