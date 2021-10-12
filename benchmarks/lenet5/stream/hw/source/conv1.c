#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv1KSize*conv1KSize*conv1InChan + h *conv1KSize*conv1InChan + w * conv1InChan + c))
#define OutIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c))

void conv1() {
    uint32_t* strIn = (uint32_t*)Pool0Out;
    uint32_t* convInput = (uint32_t*)Conv1Window;
    uint32_t* kernel = (uint32_t*)Conv1Weights;
    uint32_t* strOut = (uint32_t*)Conv1Out;
    uint32_t* convLUT = (uint32_t*)Conv1LUT;

    int h,w,c,cc,x,y;
    // Warmup Loop
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv1KSize; h++) {
        #pragma clang loop unroll(disable)
        for (w = 0; w < conv1InDim; w++) {
            #pragma clang loop unroll(disable)
            for(c = 0; c < conv1InChan; c++) {
                convInput[InputIdx3D(h%conv1KSize, w, c)] = *strIn;
            }
        }
    }

    // HWC Implementation for Convolution
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv1InDim; h++) {
        if (h > conv1KSize-1) {
            // Shift Window
            #pragma clang loop unroll(disable)
            for (w = 0; w < conv1InDim; w++) {
                #pragma clang loop unroll(disable)
                for(c = 0; c < conv1InChan; c++) {
                    convInput[InputIdx3D(h%conv1KSize, w, c)] = *strIn;
                }
            }
        }
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < conv1InDim; w++) {
            // Check that the window is valid
            if(!(w+conv1KSize>conv1InDim || h+conv1KSize>conv1InDim)) {
                // Output Channels
                #pragma clang loop unroll(disable)
                for(cc = 0; cc < conv1OutChan; cc++) {
                    // Kernel X
                    int sum = 0;
                    #pragma clang loop unroll(disable)
                    for (x = 0; x < conv1KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(disable)
                        for (y = 0; y < conv1KSize; y++) {
                            // Input Channels
                            #pragma clang loop unroll(disable)
                            for(c = 0; c < conv1InChan; c++) {
                                sum += convInput[InputIdx3D(x, y, c)]
                                * kernel[KIdx4D(x,y,c,cc)];
                            }
                        }
                    }
                    sum *= convLUT[0];
                    *strOut = sum;
                }
            }
        }
    }
}