#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv0KSize*conv0KSize*conv0InChan + h *conv0KSize*conv0InChan + w * conv0InChan + c))
#define OutIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c))

void conv0() {
    uint32_t* strIn = (uint32_t*)STREAMDMA_Stream;
    uint32_t* convInput = (uint32_t*)Conv0Window;
    uint32_t* kernel = (uint32_t*)Conv0Weights;
    uint32_t* strOut = (uint32_t*)Conv0Out;
    uint32_t* convLUT = (uint32_t*)Conv0LUT;

    int h,w,c,cc,x,y;
    // Warmup Loop
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv0KSize; h++) {
        #pragma clang loop unroll(disable)
        for (w = 0; w < conv0InDim; w++) {
            #pragma clang loop unroll(disable)
            for(c = 0; c < conv0InChan; c++) {
                convInput[InputIdx3D(h%conv0KSize, w, c)] = *strIn;
            }
        }
    }

    // HWC Implementation for Convolution
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv0InDim; h++) {
        if (h > conv0KSize-1) {
            // Shift Window
            #pragma clang loop unroll(disable)
            for (w = 0; w < conv0InDim; w++) {
                #pragma clang loop unroll(disable)
                for(c = 0; c < conv0InChan; c++) {
                    convInput[InputIdx3D(h%conv0KSize, w, c)] = *strIn;
                }
            }
        }
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < conv0InDim; w++) {
            // Check that the window is valid
            if(!(w+conv0KSize>conv0InDim || h+conv0KSize>conv0InDim)) {
                // Output Channels
                #pragma clang loop unroll(disable)
                for(cc = 0; cc < conv0OutChan; cc++) {
                    // Kernel X
                    int sum = 0;
                    #pragma clang loop unroll(full)
                    for (x = 0; x < conv0KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(full)
                        for (y = 0; y < conv0KSize; y++) {
                            // Input Channels
                            #pragma clang loop unroll(full)
                            for(c = 0; c < conv0InChan; c++) {
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