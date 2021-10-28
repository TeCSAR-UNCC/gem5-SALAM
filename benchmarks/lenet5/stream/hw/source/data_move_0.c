#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define BuffIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c))
#define WinIdx3D(h,w,c) ((h * conv0KSize*conv0InChan + w * conv0InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv0KSize*conv0KSize*conv0InChan + h *conv0KSize*conv0InChan + w * conv0InChan + c))

void dataMover() {
    volatile uint32_t* strIn = (uint32_t*)STREAMDMA_Stream;
    uint32_t* convWindow = (uint32_t*)Conv0Window;
    uint32_t* convLineBuff = (uint32_t*)Conv0LineBuff;

    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<conv0KSize; h++){
        #pragma nounroll
        for(w=0; w<conv0InDim; w++){
            #pragma nounroll
            for(c=0; c<conv0InChan; c++){
                convLineBuff[BuffIdx3D(h, w, c)] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<conv0OutDim; h++){
        // Once the first row is read data movement resumes
        if (h>=1) {
            #pragma nounroll
            for(w=0; w<conv0InDim; w++){
                #pragma nounroll
                for(c=0; c<conv0InChan; c++){
                    convLineBuff[BuffIdx3D((h-1)%5, w, c)] = *strIn;
                }
            }
        }
        #pragma nounroll
        for (w=0; w<conv0OutDim; w++){
            #pragma nounroll
            for(cc=0; cc<conv0OutChan; cc++){
                #pragma nounroll
                for(x=0; x<conv0KSize; x++) {
                    #pragma nounroll
                    for(y=0; y<conv0KSize; y++){
                        #pragma nounroll
                        for(c=0; c<conv0InChan; c++){
                            convWindow[WinIdx3D(x%5, y%5, c)] = convLineBuff[BuffIdx3D((x + h%5)%5, y, c)];
                        }
                    }
                }
            }
        }
    }
}