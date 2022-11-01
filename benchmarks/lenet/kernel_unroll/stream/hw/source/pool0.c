#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_win[pool0KSize][pool0InDim][pool0InChan];

void compute(array3d_win poolWin, uint32_t* strOut) {
    int h,w,c,cc,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < pool0InDim; h+=pool0KSize) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < pool0InDim; w+=pool0KSize) {
            #pragma nounroll
            for(c = 0; c < pool0InChan; c++) {
                int sum = 0;
                // Kernel X
                #pragma unroll
                for (x = 0; x < pool0KSize; x++) {
                    // Kernel Y
                    #pragma unroll
                    for (y = 0; y < pool0KSize; y++) {
                        // Input Channels
                        sum += poolWin[x][w+y][c];
                    }
                }
                *strOut = sum/(pool0KSize*pool0KSize);
            }
        }
    }
}

void top() {
    void* poolWin = (void*)Pool0Window;
    void* strOut = (void*)Pool0Out;

	compute(poolWin,strOut);

	return;
}