#include "gemm.h"

#ifdef DMA_MODE
#include "gem5/dma_interface.h"
#endif

// void gemm( TYPE m1[N], TYPE m2[N], TYPE prod[N] ){
//     int i, j, k;
//     int k_col, i_col;
//     TYPE mult;
//     //#pragma clang loop unroll_count(8)
//     for(i=0;i<row_size;i++) {
//         //#pragma clang loop unroll(full)
//         for(j=0;j<col_size;j++) {
//             i_col = i * col_size;
//             TYPE sum = 0;
//             //#pragma clang loop unroll_count(8)
//             #pragma clang loop unroll(full)
//             for(k=0;k<row_size;k++) {
//                 k_col = k * col_size;
//                 mult = m1[i_col + k] * m2[k_col + j];
//                 sum += mult;
//             }
//             prod[i_col + j]  = sum;
//         }
//     }
// }


void gemm(TYPE m1[N], TYPE m2[N], TYPE prod[N] ){
    int i, j, k;
    int k_col, i_col;
    TYPE mult;
    #pragma clang loop unroll_count(8)
    for(i=0;i<N;i++) {
            prod[i]  = m1[i] + m2[i];
        
    }
}
