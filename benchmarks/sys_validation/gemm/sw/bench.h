#include "../defines.h"

#define rcIndex     (r*ROW + c)

volatile int stage;

typedef struct {
    TYPE * a;
    TYPE * b;
    TYPE * c;
    int row_size;
    int col_size;
} gemm_struct;

void genData(gemm_struct * ges) {
    int r, c, k, mult, sum;

    for( r=0; r < ges->row_size; r++ ) {
        for( c=0; c < ges->col_size; c++ ) {
            ges->a[rcIndex] = rcIndex;
            ges->b[rcIndex] = (ges->row_size * ges->col_size) - 1 - rcIndex;
        }
    }
}