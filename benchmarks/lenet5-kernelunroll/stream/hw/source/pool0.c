#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_win[pool0KSize][pool0InDim][pool0InChan];

void data_mover(uint32_t* strIn, array3d_win poolWin) {
    int h,w,c;
    // Shift Window
    #pragma nounroll
    for(h = 0; h < pool0InDim; h++){
        #pragma nounroll
        for (w = 0; w < pool0InDim; w++) {
            #pragma nounroll
            for(c = 0; c < pool0InChan; c++) {
                poolWin[h%2][w][c] = *strIn;
            }
        }
    }
}

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
    void* strIn = (void*)Conv0Out;
    void* poolWin = (void*)Pool0Window;
    void* strOut = (void*)Pool0Out;

    data_mover(strIn,poolWin);
	compute(poolWin,strOut);

	return;
}