#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_t[conv0OutDim][conv0OutDim][conv0OutChan];

void compute(array3d_t in, array3d_t out) {

    // HWC Implementation for Convolution
    int h,w,c;
    // Input X
    #pragma nounroll
    for (h = 0; h < conv0OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < conv0OutDim; w++) {
            #pragma unroll
            for(c = 0; c < conv0OutChan; c++) {
               out[h][w][c] = in[h][w][c];
            }
        }
    }
}

void top(){
    void* input = (void*)Conv0Output;
    void* output = (void*)pool0Input;

	compute(input,output);
}