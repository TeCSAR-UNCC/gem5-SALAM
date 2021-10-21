#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv0InDim*conv0InChan + w * conv0InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv0KSize*conv0KSize*conv0InChan + h *conv0KSize*conv0InChan + w * conv0InChan + c))

void conv0() {
    uint32_t* convWindowBuff = (uint32_t*)Conv0WindowBuff;
    uint32_t* kernel = (uint32_t*)Conv0Weights;
    uint32_t* strOut = (uint32_t*)Conv0Out;

    int h,w,c,cc,x,y;
    uint32_t sum;

    #pragma nounroll
    for (h=0; h<conv0OutDim; h++){
        #pragma nounroll
        for (w=0; w<conv0OutDim; w++){
            sum = 0;
            #pragma nounroll
            for(cc=0; cc<conv0OutChan; cc++){
                for(x=0; x<conv0KSize; x++) {
                    #pragma nounroll
                    for(y=0; y<conv0KSize; y++){
                        #pragma nounroll
                        for(c=0; c<conv0InChan; c++){
                            sum += convWindowBuff[InputIdx3D(x, y, c)] * kernel[KIdx4D(x,y,c,cc)];
                        }
                    }
                }
                
            if(sum >= 2){
                sum = sum*0.964027580076;
            } else if (sum < 2 && sum >= 1){
                sum = sum*0.761594155956;
            } else if (sum < 1 && sum >= .5){
                sum = sum*0.46211715726;
            } else if (sum < .5 && sum >= .25){
                sum = sum*.244918662404;
            } else if (sum < .25 && sum >= 0){
                sum = sum*0;
            } else if (sum < 0 && sum >= -.25){
                sum = sum*-.244918662404;
            } else if (sum < -.25 && sum >= -.5){
                sum = sum*-0.46211715726;
            } else if (sum < -.5 && sum >= -1){
                sum = sum*-0.761594155956;
            } else if (sum > -1){
                sum = sum*-0.964027580076;
            }
            *strOut = sum;
            }
        }
    }
}