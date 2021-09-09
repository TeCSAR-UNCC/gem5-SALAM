#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv1KSize*conv1KSize*conv1InChan + h *conv1KSize*conv1InChan + w * conv1InChan + c))
#define OutIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c))

// CHW Memory Accesses
// #define InputIdx3D(h,w,c) (c * conv1InDim*conv1InDim + h * conv1InDim + w)
// #define KIdx4D(h,w,c,n) (n * conv1InChan*conv1InDim*conv1InDim + c * conv1InDim*conv1InDim + h * conv1InDim + w)
// #define OutIdx3D(h,w,c) (c * conv1OutDim*conv1OutDim + h * conv1OutDim + w)

void conv1() {
    uint32_t* convInput = (uint32_t*)Conv1Input;
    uint32_t* kernel = (uint32_t*)Conv1Weights;
    uint32_t* convOut = (uint32_t*)Conv1Output;
    uint32_t* convLUT = (uint32_t*)Conv1LUT;

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    #pragma clang loop unroll(disable)
    for (h = 0; h < conv1InDim; h++) {
        // Input Y
        #pragma clang loop unroll(disable)
        for (w = 0; w < conv1InDim; w++) {
            // Check that the window is valid
            if(!(w+conv1KSize>conv1InDim || h+conv1KSize>conv1InDim)) {
                // Output Channels
                #pragma clang loop unroll(disable)
                for(cc = 0; cc < conv1OutChan; cc++) {
                    // Kernel X
                    int sum = 0;
                    #pragma clang loop unroll(disable)
                    for (x = 0; x < conv1KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(full)
                        for (y = 0; y < conv1KSize; y++) {
                            // Input Channels
                            #pragma clang loop unroll(full)
                            for(c = 0; c < conv1InChan; c++) {
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

    // // CHW Implementation for Convolution
    // int h,w,c,cc,x,y;
    // // Output Channels
    // for(cc = 0; cc < conv1OutChan; cc++) {
    //     // Input Channels
    //     for(c = 0; c < conv1InChan; c++) {
    //         // Input X
    //         for (h = 0; h < conv1InDim; h++) {
    //             // Input Y
    //             #pragma clang loop unroll(full)
    //             for (w = 0; w < conv1InDim; w++) {
    //                 // Check that the window is valid
    //                 if(!(w+conv1KSize>conv1InDim || h+conv1KSize>conv1InDim)) {
    //                     // Kernel X
    //                     #pragma clang loop unroll(full)
    //                     for (x = 0; x < conv1KSize; x++) {
    //                         // Kernel Y
    //                         #pragma clang loop unroll(full)
    //                         for (y = 0; y < conv1KSize; y++) {
    //                             convOut[OutIdx3D(h,w,cc)] += convInput[InputIdx3D(h+x, w+y, c)]
    //                             * kernel[KIdx4D(x,y,c,cc)];
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
}