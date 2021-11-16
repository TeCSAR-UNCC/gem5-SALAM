#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_in[fc0InDim][fc0InDim][fc0InChan];
typedef uint32_t array4d_t[fc0KSize][fc0KSize][fc0InChan][fc0OutChan];
typedef uint32_t array3d_out[fc0OutDim][fc0OutDim][fc0OutChan];

void compute(array3d_in convInput, array4d_t kernel, array3d_out convOut) {
    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < fc0OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < fc0OutDim; w++) {
            // Output Channels
            #pragma nounroll
            for(cc = 0; cc < fc0OutChan; cc++) {
                // Kernel X
                int sum = 0;
                #pragma unroll
                for (x = 0; x < fc0KSize; x++) {
                    // Kernel Y
                    #pragma unroll
                    for (y = 0; y < fc0KSize; y++) {
                        // Input Channels
                        #pragma unroll
                        for(c = 0; c < fc0InChan; c++) {
                            sum += convInput[h+x][w+y][c]
                            * kernel[x][y][c][cc];
                        }
                    }
                }
                // if(sum >= 2){
                //     sum = sum*0.964027580076;
                // } else if (sum < 2 && sum >= 1){
                //     sum = sum*0.761594155956;
                // } else if (sum < 1 && sum >= .5){
                //     sum = sum*0.46211715726;
                // } else if (sum < .5 && sum >= .25){
                //     sum = sum*.244918662404;
                // } else if (sum < .25 && sum >= 0){
                //     sum = sum*0;
                // } else if (sum < 0 && sum >= -.25){
                //     sum = sum*-.244918662404;
                // } else if (sum < -.25 && sum >= -.5){
                //     sum = sum*-0.46211715726;
                // } else if (sum < -.5 && sum >= -1){
                //     sum = sum*-0.761594155956;
                // } else if (sum > -1){
                //     sum = sum*-0.964027580076;
                // }
                convOut[h][w][cc] = sum;
            } 
        }
    }
}

void top() {
    void* convInput = (void*)fc0Input;
    void* kernel = (void*)fc0Weights;
    void* convOut = (void*)fc0Output;

	compute(convInput,kernel,convOut);

	return;
}