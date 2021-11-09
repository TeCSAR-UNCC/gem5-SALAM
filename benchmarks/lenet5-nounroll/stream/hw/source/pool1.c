#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_win[pool1KSize][pool1InDim][pool1InChan];

void compute(array3d_win poolWin, uint32_t* strOut) {
    int h,w,c,cc,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < pool1InDim; h+=pool1KSize) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < pool1InDim; w+=pool1KSize) {
            #pragma nounroll
            for(c = 0; c < pool1InChan; c++) {
                int sum = 0;
                // Kernel X
                #pragma unroll
                for (x = 0; x < pool1KSize; x++) {
                    // Kernel Y
                    #pragma unroll
                    for (y = 0; y < pool1KSize; y++) {
                        // Input Channels
                        sum += poolWin[x][w+y][c];
                    }
                }
                *strOut = sum/(pool1KSize*pool1KSize);
            }
        }
    }
}

void top() {
    void* poolWin = (void*)Pool1Window;
    void* strOut = (void*)Pool1Out;

	compute(poolWin,strOut);

	return;
}