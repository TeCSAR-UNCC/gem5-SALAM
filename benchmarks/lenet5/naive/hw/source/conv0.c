#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv0KSize*conv0KSize*conv0InChan + h *conv0KSize*conv0InChan + w * conv0InChan + c))
#define OutIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c))

void conv0() {
    uint32_t* convInput = (uint32_t*)Conv0Input;
    uint32_t* kernel = (uint32_t*)Conv0Weights;
    uint32_t* convOut = (uint32_t*)Conv0Output;
    uint32_t* convLUT = (uint32_t*)Conv0LUT;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma nounroll
    for (h = 0; h < conv0InDim; h++) {
        // Input Y
        #pragma nounroll
        for (w = 0; w < conv0InDim; w++) {
            // Check that the window is valid
            if(!(w+conv0KSize>conv0InDim || h+conv0KSize>conv0InDim)) {
                // Output Channels
                #pragma nounroll
                for(cc = 0; cc < conv0OutChan; cc++) {
                    // Kernel X
                    int sum = 0;
                    #pragma nounroll
                    for (x = 0; x < conv0KSize; x++) {
                        // Kernel Y
                        #pragma nounroll
                        for (y = 0; y < conv0KSize; y++) {
                            // Input Channels
                            #pragma nounroll
                            for(c = 0; c < conv0InChan; c++) {
                                sum += convInput[InputIdx3D(h+x, w+y, c)]
                                * kernel[KIdx4D(x,y,c,cc)];
                            }
                        }
                    }
                    sum *= convLUT[0];
                    convOut[OutIdx3D(h,w,cc)] = sum;
                }
            }
        }
    }
}