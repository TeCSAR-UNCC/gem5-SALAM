#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv0KSize*conv0KSize*conv0InChan + h *conv0KSize*conv0InChan + w * conv0InChan + c))

void conv0() {
    volatile uint32_t* strIn = (uint32_t*)STREAMDMA_Stream;
    uint32_t* convInput = (uint32_t*)Conv0Window;
    uint32_t* kernel = (uint32_t*)Conv0Weights;
    volatile uint32_t* strOut = (uint32_t*)STREAMDMA_Stream;
    // uint32_t* strOut = (uint32_t*)Conv0Out;
    uint32_t* convLUT = (uint32_t*)Conv0LUT;

    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup data movement
    #pragma nounroll
    for (x=0; x<conv0KSize; x++){
        #pragma nounroll
        for(y=0;y<conv0KSize; y++){
            #pragma nounroll
            for(c=0; c<conv0InChan; c++){
                convInput[InputIdx3D(x, y, c)] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<conv0InDim; h++){
        // convInput[InputIdx3D(x, y, c)] = *strIn;
        #pragma nounroll
        for (w=0; w<conv0InDim; w++){
            if(!(h+conv0KSize > conv0InDim || w+conv0KSize > conv0InDim)) {
                sum = 0;
                #pragma nounroll
                for(cc=0; cc<conv0OutChan; cc++){
                    for(x=0; x<conv0KSize; x++) {
                        #pragma nounroll
                        for(y=0; y<conv0KSize; y++){
                            #pragma nounroll
                            for(c=0; c<conv0InChan; c++){
                                sum += convInput[InputIdx3D(x, y, c)] * kernel[KIdx4D(x,y,c,cc)];
                            }
                        }
                    }
                // ADD TANH ACTIVATION HERE
                *strOut = sum;
                }
            }
        }
    }
}