#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_in[pool1InDim][pool1InDim][pool1InChan];
typedef uint32_t array3d_out[pool1OutDim][pool1OutDim][pool1InChan];

void compute(array3d_in poolIn, array3d_out poolOut) {
    // HWC Implementation for Convolution
    int h,w,c,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < pool1InDim; h+=pool1KSize) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < pool1InDim; w+=pool1KSize) {
            // Input Channels
            #pragma nounroll
            for(c = 0; c < pool1InChan; c++) {
                int sum = 0;
                // Kernel X
                #pragma unroll
                for (x = 0; x < pool1KSize; x++) {
                    // Kernel Y
                    #pragma unroll
                    for (y = 0; y < pool1KSize; y++) {
                        sum += poolIn[h+x][w+y][c];
                    }
                }
                poolOut[h/pool1KSize][w/pool1KSize][c] = sum/(pool1KSize*pool1KSize);
            }
        }
    }
}

void top(){
    void* poolIn = (void*)pool1Input;
    void* poolOut = (void*)pool1Output;

	compute(poolIn,poolOut);
}