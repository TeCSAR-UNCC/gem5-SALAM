#include "hw_defines.h"

// void gemm(){
//     TYPE    * m1     = (TYPE    *)MATRIX1;
//     TYPE    * m2     = (TYPE    *)MATRIX2;
//     TYPE    * m3     = (TYPE    *)MATRIX3;
//     int k_col, i_col;
//     TYPE mult, sum;
//     for(int i=0;i<row_size;i++) {
//         for(int j=0;j<col_size;j++) {
//             i_col = i * col_size;
//             sum = 0;
//             #pragma unroll
//             for(int k=0;k<row_size;k++) {
//                 k_col = k * col_size;
//                 mult = m1[i_col + k] * m2[k_col + j];
//                 sum += mult;
//             }
//             m3[i_col + j]  = sum;
//         }
//     }
// }

void gemm(){
    TYPE    * m1     = (TYPE    *)MATRIX1;
    TYPE    * m2     = (TYPE    *)MATRIX2;
    TYPE    * m3     = (TYPE    *)MATRIX3;
    int k_col, i_col;
    TYPE mult, sum;
    for(int ij=0; ij<row_size*col_size; ij++) {
        // Column index
        int i = ij % row_size;
        // Row index
        int j = ij / row_size;

        i_col = i * col_size;
        sum = 0;
        #pragma unroll
        for(int k=0;k<row_size;k++) {
            k_col = k * col_size;
            mult = m1[i_col + k] * m2[k_col + j];
            sum += mult;
        }
        m3[i_col + j]  = sum;
    }
}