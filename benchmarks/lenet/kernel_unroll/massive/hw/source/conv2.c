#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_in[conv2InDim][conv2InDim][conv2InChan];
typedef uint32_t array4d_t[conv2KSize][conv2KSize][conv2InChan][conv2OutChan];
typedef uint32_t array3d_out[conv2OutDim][conv2OutDim][conv2OutChan];

void compute(array3d_in convInput, array4d_t kernel, array3d_out convOut) {
    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < conv2OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < conv2OutDim; w++) {
            // Output Channels
            #pragma nounroll
            for(cc = 0; cc < conv2OutChan; cc++) {
                // Kernel X
                int sum = 0;
                #pragma unroll
                for (x = 0; x < conv2KSize; x++) {
                    // Kernel Y
                    #pragma unroll
                    for (y = 0; y < conv2KSize; y++) {
                        // Input Channels
                        #pragma unroll
                        for(c = 0; c < conv2InChan; c++) {
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
    void* convInput = (void*)conv2Input;
    void* kernel = (void*)conv2Weights;
    void* convOut = (void*)conv2Output;

	compute(convInput,kernel,convOut);

	return;
}