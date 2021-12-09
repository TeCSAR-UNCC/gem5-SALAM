#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_t[pool1OutDim][pool1OutDim][pool1OutChan];

void compute(array3d_t in, array3d_t out) {

    // HWC Implementation for Convolution
    int h,w,c;
    // Input X
    #pragma nounroll
    for (h = 0; h < pool1OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < pool1OutDim; w++) {
            #pragma nounroll
            for(c = 0; c < pool1OutChan; c++) {
               out[h][w][c] = in[h][w][c];
            }
        }
    }
}

void top(){
    void* input = (void*)pool1Output;
    void* output = (void*)conv2Input;

	compute(input,output);
}