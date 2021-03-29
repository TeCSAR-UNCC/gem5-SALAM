#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * fc1InDim*fc1InChan + w * fc1InChan + c))
#define KIdx4D(h,w,c,n) ((n * fc1KSize*fc1KSize*fc1InChan + h *fc1KSize*fc1InChan + w * fc1InChan + c))
#define OutIdx3D(h,w,c) ((h * fc1InDim*fc1InChan + w * fc1InChan + c))

void fc1() {
    uint32_t* strIn = (uint32_t*)fc0Out;
    uint32_t* convInput = (uint32_t*)fc1Window;
    uint32_t* kernel = (uint32_t*)fc1Weights;
    uint32_t* strOut = (uint32_t*)STREAMDMA_Stream;
    uint32_t* convLUT = (uint32_t*)fc1LUT;

    int h,w,c,cc,x,y;
    // Warmup Loop
    #pragma clang loop unroll(disable)
    for (h = 0; h < fc1KSize; h++) {
        #pragma clang loop unroll(disable)
        for (w = 0; w < fc1InDim; w++) {
            #pragma clang loop unroll(disable)
            for(c = 0; c < fc1InChan; c++) {
                convInput[InputIdx3D(h%fc1KSize, w, c)] = *strIn;
            }
        }
    }

    // HWC Implementation for Convolution
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < fc1InDim; h++) {
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < fc1InDim; w++) {
            // Check that the window is valid
            if(!(w+fc1KSize>fc1InDim || h+fc1KSize>fc1InDim)) {
                // Output Channels
                #pragma clang loop unroll(disable)
                for(cc = 0; cc < fc1OutChan; cc++) {
                    // Kernel X
                    int sum = 0;
                    #pragma clang loop unroll(full)
                    for (x = 0; x < fc1KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(full)
                        for (y = 0; y < fc1KSize; y++) {
                            // Input Channels
                            #pragma clang loop unroll(full)
                            for(c = 0; c < fc1InChan; c++) {
                                sum += convInput[InputIdx3D(x, y, c)]
                                * kernel[KIdx4D(x,y,c,cc)];
                            }
                        }
                    }
                    *strOut = sum;
                }
            }
        }
    }
}