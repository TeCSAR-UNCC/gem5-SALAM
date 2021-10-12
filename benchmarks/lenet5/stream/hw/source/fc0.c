#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * fc0InDim*fc0InChan + w * fc0InChan + c))
#define KIdx4D(h,w,c,n) ((n * fc0KSize*fc0KSize*fc0InChan + h *fc0KSize*fc0InChan + w * fc0InChan + c))
#define OutIdx3D(h,w,c) ((h * fc0InDim*fc0InChan + w * fc0InChan + c))

void fc0() {
    uint32_t* strIn = (uint32_t*)Pool1Out;
    uint32_t* convInput = (uint32_t*)fc0Window;
    uint32_t* kernel = (uint32_t*)fc0Weights;
    uint32_t* strOut = (uint32_t*)fc0Out;
    uint32_t* convLUT = (uint32_t*)fc0LUT;

    int h,w,c,cc,x,y;
    // Warmup Loop
    #pragma clang loop unroll(disable)
    for (h = 0; h < fc0KSize; h++) {
        #pragma clang loop unroll(disable)
        for (w = 0; w < fc0InDim; w++) {
            #pragma clang loop unroll(disable)
            for(c = 0; c < fc0InChan; c++) {
                convInput[InputIdx3D(h%fc0KSize, w, c)] = *strIn;
            }
        }
    }

    // HWC Implementation for Convolution
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < fc0InDim; h++) {
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < fc0InDim; w++) {
            // Check that the window is valid
            if(!(w+fc0KSize>fc0InDim || h+fc0KSize>fc0InDim)) {
                // Output Channels
                #pragma clang loop unroll(disable)
                for(cc = 0; cc < fc0OutChan; cc++) {
                    // Kernel X
                    int sum = 0;
                    #pragma clang loop unroll(disable)
                    for (x = 0; x < fc0KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(disable)
                        for (y = 0; y < fc0KSize; y++) {
                            // Input Channels
                            #pragma clang loop unroll(disable)
                            for(c = 0; c < fc0InChan; c++) {
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