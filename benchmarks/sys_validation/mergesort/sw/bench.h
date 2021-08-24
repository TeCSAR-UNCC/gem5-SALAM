#include "../defines.h"

#define rcIndex     (r*ROW + c)

volatile int stage;

// void genData(gemm_struct * ges) {
//     int r, c, k, mult, sum;

//     for( r=0; r < ges->row_size; r++ ) {
//         for( c=0; c < ges->col_size; c++ ) {
//             ges->a[rcIndex] = rcIndex;
//             ges->b[rcIndex] = (ges->row_size * ges->col_size) - 1 - rcIndex;
//         }
//     }
// }