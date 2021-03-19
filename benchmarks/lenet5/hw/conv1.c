#include "../lenet5_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c)*sizeof(TYPE))
#define KIdx4D(h,w,c,n) ((n * conv1KSize*conv1KSize*conv1InChan + h *conv1KSize*conv1InChan + w * conv1InChan + c)*sizeof(TYPE))
#define OutIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c)*sizeof(TYPE))

// CHW Memory Accesses
// #define InputIdx3D(h,w,c) (c * conv1InDim*conv1InDim + h * conv1InDim + w)
// #define KIdx4D(h,w,c,n) (n * conv1InChan*conv1InDim*conv1InDim + c * conv1InDim*conv1InDim + h * conv1InDim + w)
// #define OutIdx3D(h,w,c) (c * conv1OutDim*conv1OutDim + h * conv1OutDim + w)

void conv1() {
    uint8_t* convInput = (uint8_t*)Conv1Input;
    uint8_t* kernel = (uint8_t*)Conv1Weights;
    uint8_t* convOut = (uint8_t*)Conv1Output;
    uint8_t* convLUT = (uint8_t*)Conv1LUT;

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
                    #pragma clang loop unroll(disable)
                    for (x = 0; x < conv1KSize; x++) {
                        // Kernel Y
                        #pragma clang loop unroll(disable)
                        for (y = 0; y < conv1KSize; y++) {
                            // Input Channels
                            int sum = 0;
                            #pragma clang loop unroll(disable)
                            for(c = 0; c < conv1InChan; c++) {
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

    // Apply the activation function
    for (h = 0; h < conv1OutDim; h++){
        for ( w = 0; w < conv1OutDim; w++) {
            #pragma clang loop unroll(disable)
            for ( c = 0; c < conv1OutChan; c++) {
                convOut[OutIdx3D(h,w,c)] *= convLUT[0];
            }
        }
    }
}