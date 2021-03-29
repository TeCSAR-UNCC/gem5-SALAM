#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * fc0InDim*fc0InChan + w * fc0InChan + c))
#define KIdx4D(h,w,c,n) ((n * fc0KSize*fc0KSize*fc0InChan + h *fc0KSize*fc0InChan + w * fc0InChan + c))
#define OutIdx3D(h,w,c) ((h * fc0InDim*fc0InChan + w * fc0InChan + c))

void fc0() {
    uint32_t* fcInput = (uint32_t*)fc0Input;
    uint32_t* kernel = (uint32_t*)fc0Weights;
    uint32_t* fcOut = (uint32_t*)fc0Output;
    uint32_t* fcLUT = (uint32_t*)fc0LUT;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < fc0InDim; h++) {
        // Input Y
        for (w = 0; w < fc0InDim; w++) {
            // Check that the window is valid
            if(!(w+fc0KSize>fc0InDim || h+fc0KSize>fc0InDim)) {
                // Output Channels
                #pragma clang loop unroll(disable)
                for(cc = 0; cc < fc0OutChan; cc++) {
                    // Kernel X
                    int sum = 0;
                    #pragma clang loop unroll(full)
                    for (x = 0; x < fc0KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(full)
                        for (y = 0; y < fc0KSize; y++) {

                            #pragma clang loop unroll(full)
                            // Input Channels
                            for(c = 0; c < fc0InChan; c++) {
                                sum += fcInput[InputIdx3D(h+x, w+y, c)]
                                * kernel[KIdx4D(x,y,c,cc)];
                            }
                        }
                    }
                    sum *= fcLUT[0];
                    fcOut[OutIdx3D(h,w,cc)] = sum;
                }
            }
        }
    }
}