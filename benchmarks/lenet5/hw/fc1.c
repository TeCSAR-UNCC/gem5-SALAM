#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * fc1InDim*fc1InChan + w * fc1InChan + c)*sizeof(TYPE))
#define KIdx4D(h,w,c,n) ((n * fc1KSize*fc1KSize*fc1InChan + h *fc1KSize*fc1InChan + w * fc1InChan + c)*sizeof(TYPE))
#define OutIdx3D(h,w,c) ((h * fc1InDim*fc1InChan + w * fc1InChan + c)*sizeof(TYPE))

void fc1() {
    uint8_t* fcInput = (uint8_t*)fc1Input;
    uint8_t* kernel = (uint8_t*)fc1Weights;
    uint8_t* fcOut = (uint8_t*)fc1Output;
    uint8_t* fcLUT = (uint8_t*)fc1LUT;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
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
                    #pragma clang loop unroll(disable)
                    for (x = 0; x < fc1KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(disable)
                        for (y = 0; y < fc1KSize; y++) {
                            // Input Channels
                            int sum = 0;
                            #pragma clang loop unroll(disable)
                            for(c = 0; c < fc1InChan; c++) {
                                sum += fcInput[InputIdx3D(h+x, w+y, c)]
                                * kernel[KIdx4D(x,y,c,cc)];
                            }
                            fcOut[OutIdx3D(h,w,cc)] += sum;
                        }
                    }
                }
            }
        }
    }

    // Apply the activation function
    for (h = 0; h < fc1OutDim; h++){
        for ( w = 0; w < fc1OutDim; w++) {
            #pragma clang loop unroll(disable)
            for ( c = 0; c < fc1OutChan; c++) {
                fcOut[OutIdx3D(h,w,c)] *= fcLUT[0];
            }
        }
    }
}