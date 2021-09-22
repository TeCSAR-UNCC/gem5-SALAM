#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * pool0InDim*pool0InChan + w * pool0InChan + c))
#define OutIdx3D(h,w,c) ((h * pool0InDim*pool0InChan + w * pool0InChan + c))

void pool0() {
    uint32_t* convInput = (uint32_t*)pool0Input;
    uint32_t* convOut = (uint32_t*)pool0Output;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < pool0InDim; h+=pool0KSize) {
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < pool0InDim; w+=pool0KSize) {
            // Check that the window is valid
            if(!(w+pool0KSize>pool0InDim || h+pool0KSize>pool0InDim)) {
                int sum = 0;
                // Kernel X
                #pragma clang loop unroll(disable)
                for (x = 0; x < pool0KSize; x++) {
                    // Kernel Y
                    #pragma clang loop unroll(full)
                    for (y = 0; y < pool0KSize; y++) {
                        // Input Channels
                        #pragma clang loop unroll(full)
                        for(c = 0; c < pool0InChan; c++) {
                            sum += convInput[InputIdx3D(h+x, w+y, c)];
                        }
                    }
                }
                convOut[OutIdx3D(h,w,cc)] = sum/(pool0KSize*pool0KSize);
            }
        }
    }
}