#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv1KSize*conv1KSize*conv1InChan + h *conv1KSize*conv1InChan + w * conv1InChan + c))

void dataMover() {
    volatile uint32_t* strIn = (uint32_t*)Pool0Out;
    uint32_t* convWindowBuff = (uint32_t*)Conv1WindowBuff;
    uint32_t* convWindow = (uint32_t*)Conv1Window;

    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<conv1KSize; h++){
        #pragma nounroll
        for(w=0; w<conv1InDim; w++){
            #pragma nounroll
            for(c=0; c<conv1InChan; c++){
                convWindow[InputIdx3D(h, w, c)] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<conv1OutDim; h++){
        // Once the first row is read data movement resumes
        if (h>=1) {
            #pragma nounroll
            for(w=0; w<conv1InDim; w++){
                #pragma nounroll
                for(c=0; c<conv1InChan; c++){
                    convWindow[InputIdx3D((h-1)%5, w, c)] = *strIn;
                }
            }
        }
        for (w=0; w<conv1OutDim; w++){
            #pragma nounroll
            for(cc=0; cc<conv1OutChan; cc++){
                for(x=0; x<conv1KSize; x++) {
                    #pragma nounroll
                    for(y=0; y<conv1KSize; y++){
                        #pragma nounroll
                        for(c=0; c<conv1InChan; c++){
                            // convWindowBuff[InputIdx3D(x, y, c)] = convWindow[InputIdx3D((x + h%5)%5, y, c)];
                            *convWindowBuff = convWindow[InputIdx3D((x + h%5)%5, y, c)];
                        }
                    }
                }
            }
        }
    }
}