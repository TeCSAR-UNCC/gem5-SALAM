#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * pool0InDim*pool0InChan + w * pool0InChan + c))

void pool0() {
    uint32_t* strIn = (uint32_t*)Conv0Out;
    uint32_t* convInput = (uint32_t*)Pool0Window;
    uint32_t* strOut = (uint32_t*)Pool0Out;

    int h,w,c,cc,hh,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < pool0InDim; h+=pool0KSize) {
        // Shift Window
        #pragma clang loop unroll(disable)
        for(hh = 0; hh < pool0KSize; hh++){
            #pragma clang loop unroll(disable)
            for (w = 0; w < pool0InDim; w++) {
                #pragma clang loop unroll(disable)
                for(c = 0; c < pool0InChan; c++) {
                    convInput[InputIdx3D(hh, w, c)] = *strIn;
                }
            }
        }
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < pool0InDim; w+=pool0KSize) {
            // Check that the window is valid
            if(!(w+pool0KSize>pool0InDim || h+pool0KSize>pool0InDim)) {
                for(c = 0; c < pool0InChan; c++) {
                    int sum = 0;
                    // Kernel X
                    #pragma clang loop unroll(disable)
                    for (x = 0; x < pool0KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(disable)
                        for (y = 0; y < pool0KSize; y++) {
                            // Input Channels
                            sum += convInput[InputIdx3D(h+x, w+y, c)];
                        }
                    }
                    *strOut = sum/(pool0KSize*pool0KSize);
                }
            }
        }
    }
}