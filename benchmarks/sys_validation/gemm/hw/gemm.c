#include "../gemm_hw_defines.h"

void gemm(){
    uint8_t * m1base = (uint8_t *)MATRIX1;
    uint8_t * m2base = (uint8_t *)MATRIX2;
    uint8_t * m3base = (uint8_t *)MATRIX3;
    TYPE    * m1     = (TYPE    *)m1base;
    TYPE    * m2     = (TYPE    *)m2base;
    TYPE    * m3     = (TYPE    *)m3base;
    int k_col, i_col;
    TYPE mult, sum;
    for(int i=0;i<row_size;i++) {
        for(int j=0;j<col_size;j++) {
            i_col = i * col_size;
            sum = 0;
            #pragma clang loop unroll(full)
            for(int k=0;k<row_size;k++) {
                k_col = k * col_size;
                mult = m1[i_col + k] * m2[k_col + j];
                sum += mult;
            }
            m3[i_col + j]  = sum;
        }
    }
}
