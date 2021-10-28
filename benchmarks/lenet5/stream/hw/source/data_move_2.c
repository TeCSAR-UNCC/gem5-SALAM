#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define BuffIdx3D(h,w,c) ((h * conv2InDim*conv2InChan + w * conv2InChan + c))
#define WinIdx3D(h,w,c) ((h * conv2KSize*conv2InChan + w * conv2InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv2KSize*conv2KSize*conv2InChan + h *conv2KSize*conv2InChan + w * conv2InChan + c))

void dataMover() {
    volatile uint32_t* strIn = (uint32_t*)Pool1Out;
    uint32_t* convWindow = (uint32_t*)Conv2Window;
    uint32_t* convLineBuff = (uint32_t*)Conv2LineBuff;

    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<conv2KSize; h++){
        #pragma nounroll
        for(w=0; w<conv2InDim; w++){
            #pragma nounroll
            for(c=0; c<conv2InChan; c++){
                convLineBuff[BuffIdx3D(h, w, c)] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<conv2OutDim; h++){
        #pragma nounroll
        for (w=0; w<conv2OutDim; w++){
            #pragma nounroll
            for(cc=0; cc<conv2OutChan; cc++){
                for(x=0; x<conv2KSize; x++) {
                    #pragma nounroll
                    for(y=0; y<conv2KSize; y++){
                        #pragma nounroll
                        for(c=0; c<conv2InChan; c++){
                            convWindow[WinIdx3D(x%5, y%5, c)] = convLineBuff[BuffIdx3D((x + h%5)%5, y, c)];
                        }
                    }
                }
            }
        }
    }
}