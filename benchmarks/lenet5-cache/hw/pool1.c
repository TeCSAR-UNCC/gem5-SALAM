#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * pool1InDim*pool1InChan + w * pool1InChan + c))

void pool1() {
    uint32_t* strIn = (uint32_t*)Conv1Out;
    uint32_t* convInput = (uint32_t*)Pool1Window;
    uint32_t* strOut = (uint32_t*)Pool1Out;

    int h,w,c,cc,hh,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < pool1InDim; h+=pool1KSize) {
        // Shift Window
        #pragma clang loop unroll(disable)
        for(hh = 0; hh < pool1KSize; hh++){
            #pragma clang loop unroll(disable)
            for (w = 0; w < pool1InDim; w++) {
                #pragma clang loop unroll(disable)
                for(c = 0; c < pool1InChan; c++) {
                    convInput[InputIdx3D(hh, w, c)] = *strIn;
                }
            }
        }
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < pool1InDim; w+=pool1KSize) {
            // Check that the window is valid
            if(!(w+pool1KSize>pool1InDim || h+pool1KSize>pool1InDim)) {
                #pragma clang loop unroll(disable)
                for(c = 0; c < pool1InChan; c++) {
                    int sum = 0;
                    // Kernel X
                    #pragma clang loop unroll(disable)
                    for (x = 0; x < pool1KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(disable)
                        for (y = 0; y < pool1KSize; y++) {
                            // Input Channels
                            sum += convInput[InputIdx3D(h+x, w+y, c)];
                        }
                    }
                    *strOut = sum/(pool1KSize*pool1KSize);
                }
            }
        }
    }
}