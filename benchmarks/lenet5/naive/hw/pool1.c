#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * pool1InDim*pool1InChan + w * pool1InChan + c))
#define OutIdx3D(h,w,c) ((h * pool1InDim*pool1InChan + w * pool1InChan + c))

void pool1() {
    uint32_t* convInput = (uint32_t*)pool1Input;
    uint32_t* convOut = (uint32_t*)pool1Output;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < pool1InDim; h+=pool1KSize) {
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < pool1InDim; w+=pool1KSize) {
            // Check that the window is valid
            if(!(w+pool1KSize>pool1InDim || h+pool1KSize>pool1InDim)) {
                int sum = 0;
                // Kernel X
                #pragma clang loop unroll(disable)
                for (x = 0; x < pool1KSize; x++) {
                    // Kernel Y
                    #pragma clang loop unroll(full)
                    for (y = 0; y < pool1KSize; y++) {
                        // Input Channels
                        #pragma clang loop unroll(full)
                        for(c = 0; c < pool1InChan; c++) {
                            sum += convInput[InputIdx3D(h+x, w+y, c)];
                        }
                    }
                }
                convOut[OutIdx3D(h,w,cc)] = sum/(pool1KSize*pool1KSize);
            }
        }
    }
}