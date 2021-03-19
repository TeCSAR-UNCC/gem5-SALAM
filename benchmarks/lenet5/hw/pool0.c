#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * pool0InDim*pool0InChan + w * pool0InChan + c)*sizeof(TYPE))
#define OutIdx3D(h,w,c) ((h * pool0InDim*pool0InChan + w * pool0InChan + c)*sizeof(TYPE))

void pool0() {
    uint8_t* convInput = (uint8_t*)pool0Input;
    uint8_t* convOut = (uint8_t*)pool0Output;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv0InDim; h+=pool0KSize) {
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < conv0InDim; w+=pool0KSize) {
            // Check that the window is valid
            if(!(w+conv0KSize>conv0InDim || h+conv0KSize>conv0InDim)) {
                // Kernel X
                #pragma clang loop unroll(disable)
                for (x = 0; x < conv0KSize; x++) {
                    // Kernel Y
                    #pragma clang loop unroll(disable)
                    for (y = 0; y < conv0KSize; y++) {
                        // Input Channels
                        int sum = 0;
                        #pragma clang loop unroll(disable)
                        for(c = 0; c < conv0InChan; c++) {
                            sum += convInput[InputIdx3D(h+x, w+y, c)];
                        }
                        convOut[OutIdx3D(h,w,cc)] += sum/(pool0KSize*pool0KSize);
                    }
                }
            }
        }
    }
}