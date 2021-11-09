#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_t[conv1OutDim][conv1OutDim][conv1OutChan];

void compute(array3d_t in, array3d_t out) {

    // HWC Implementation for Convolution
    int h,w,c;
    // Input X
    #pragma nounroll
    for (h = 0; h < conv1OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < conv1OutDim; w++) {
            #pragma nounroll
            for(c = 0; c < conv1OutChan; c++) {
               out[h][w][c] = in[h][w][c];
            }
        }
    }
}

void top(){
    void* input = (void*)Conv1Output;
    void* output = (void*)pool1Input;

	compute(input,output);
}