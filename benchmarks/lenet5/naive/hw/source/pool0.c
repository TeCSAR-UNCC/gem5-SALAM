#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_in[pool0InDim][pool0InDim][pool0InChan];
typedef uint32_t array3d_out[pool0OutDim][pool0OutDim][pool0InChan];

void compute(array3d_in poolIn, array3d_out poolOut) {
    // HWC Implementation for Convolution
    int h,w,c,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < pool0InDim; h+=pool0KSize) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < pool0InDim; w+=pool0KSize) {
            // Input Channels
            #pragma unroll
            for(c = 0; c < pool0InChan; c++) {
                int sum = 0;
                // Kernel X
                #pragma unroll
                for (x = 0; x < pool0KSize; x++) {
                    // Kernel Y
                    #pragma unroll
                    for (y = 0; y < pool0KSize; y++) {
                        sum += poolIn[h+x][w+y][c];
                    }
                }
                poolOut[h/pool0KSize][w/pool0KSize][c] = sum/(pool0KSize*pool0KSize);
            }
        }
    }
}

void top(){
    void* poolIn = (void*)pool0Input;
    void* poolOut = (void*)pool0Output;

	compute(poolIn,poolOut);
}