#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_t[pool0OutDim][pool0OutDim][pool0OutChan];

void compute(array3d_t in, array3d_t out) {

    // HWC Implementation for Convolution
    int h,w,c;
    // Input X
    #pragma nounroll
    for (h = 0; h < pool0OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < pool0OutDim; w++) {
            #pragma unroll
            for(c = 0; c < pool0OutChan; c++) {
               out[h][w][c] = in[h][w][c];
            }
        }
    }
}

void top(){
    void* input = (void*)pool0Output;
    void* output = (void*)Conv1Input;

	compute(input,output);
}