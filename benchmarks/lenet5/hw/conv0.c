#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c)*sizeof(TYPE))
#define KIdx4D(h,w,c,n) ((n * conv0KSize*conv0KSize*conv0InChan + h *conv0KSize*conv0InChan + w * conv0InChan + c)*sizeof(TYPE))
#define OutIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c)*sizeof(TYPE))

void conv0() {
    uint8_t* convInput = (uint8_t*)Conv0Input;
    uint8_t* kernel = (uint8_t*)Conv0Weights;
    uint8_t* convOut = (uint8_t*)Conv0Output;
    uint8_t* convLUT = (uint8_t*)Conv0LUT;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv0InDim; h++) {
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < conv0InDim; w++) {
            // Check that the window is valid
            if(!(w+conv0KSize>conv0InDim || h+conv0KSize>conv0InDim)) {
                // Output Channels
                #pragma clang loop unroll(full)
                for(cc = 0; cc < conv0OutChan; cc++) {
                    // Kernel X
                    #pragma clang loop unroll(full)
                    for (x = 0; x < conv0KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(full)
                        for (y = 0; y < conv0KSize; y++) {
                            // Input Channels
                            int sum = 0;
                            #pragma clang loop unroll(full)
                            for(c = 0; c < conv0InChan; c++) {
                                sum += convInput[InputIdx3D(h+x, w+y, c)]
                                * kernel[KIdx4D(x,y,c,cc)];
                            }
                            convOut[OutIdx3D(h,w,cc)] += sum;
                        }
                    }
                }
            }
        }
    }

    // Apply the activation function
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv0OutDim; h++){
        #pragma clang loop unroll(disable)
        for ( w = 0; w < conv0OutDim; w++) {
            #pragma clang loop unroll(full)
            for ( c = 0; c < conv0OutChan; c++) {
                convOut[OutIdx3D(h,w,c)] *= convLUT[0];
            }
        }
    }
}