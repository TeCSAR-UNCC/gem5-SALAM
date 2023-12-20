#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_in[conv1InDim][conv1InDim][conv1InChan];
typedef uint32_t array4d_t[conv1KSize][conv1KSize][conv1InChan][conv1OutChan];
typedef uint32_t array3d_out[conv1OutDim][conv1OutDim][conv1OutChan];

void compute(array3d_in convInput, array4d_t kernel, array3d_out convOut) {
    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < conv1OutDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < conv1OutDim; w++) {
            #ifdef NO_UNROLL
            #pragma nounroll
            #elif INPUT_UNROLL
            #pragma nounroll
            #elif OUTPUT_UNROLL
            #pragma unroll
            #endif
            // Output Channels
            for(cc = 0; cc < conv1OutChan; cc++) {
                int sum = 0;
                #ifdef NO_UNROLL
                #pragma nounroll
                #elif INPUT_UNROLL
                #pragma unroll
                #elif OUTPUT_UNROLL
                #pragma unroll
                #endif
                // Kernel X
                for (x = 0; x < conv1KSize; x++) {
                    #ifdef NO_UNROLL
                    #pragma nounroll
                    #elif INPUT_UNROLL
                    #pragma unroll
                    #elif OUTPUT_UNROLL
                    #pragma unroll
                    #endif
                    // Kernel Y
                    for (y = 0; y < conv1KSize; y++) {
                        #ifdef NO_UNROLL
                        #pragma nounroll
                        #elif INPUT_UNROLL
                        #pragma unroll
                        #elif OUTPUT_UNROLL
                        #pragma unroll
                        #endif
                        // Input Channels
                        for(c = 0; c < conv1InChan; c++) {
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
    void* convInput = (void*)Conv1Input;
    void* kernel = (void*)Conv1Weights;
    void* convOut = (void*)Conv1Output;

	compute(convInput,kernel,convOut);

	return;
}