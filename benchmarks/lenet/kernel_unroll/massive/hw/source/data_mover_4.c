#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_t[conv2OutDim][conv2OutDim][conv2OutChan];

void compute(array3d_t in, array3d_t out) {

    // HWC Implementation for Convolution
    int h,w,c;
    // Input X
    #pragma nounroll
    for (h = 0; h < conv2OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < conv2OutDim; w++) {
            #pragma nounroll
            for(c = 0; c < conv2OutChan; c++) {
               out[h][w][c] = in[h][w][c];
            }
        }
    }
}

void top(){
    void* input = (void*)conv2Output;
    void* output = (void*)fc0Input;

	compute(input,output);
}