#include "../../lenet5_clstr_hw_defines.h"

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv1KSize*conv1KSize*conv1InChan + h *conv1KSize*conv1InChan + w * conv1InChan + c))

void conv1() {
    volatile uint32_t* convWindowBuff = (uint32_t*)Conv1WindowBuff;
    uint32_t* kernel = (uint32_t*)Conv1Weights;
    uint32_t* strOut = (uint32_t*)STREAMDMA_Stream;
    // uint32_t* strOut = (uint32_t*)Conv1Out;


    int h,w,c,cc,x,y;
    uint32_t sum;

    #pragma nounroll
    for (h=0; h<conv1OutDim; h++){
        #pragma nounroll
        for (w=0; w<conv1OutDim; w++){
            sum = 0;
            #pragma nounroll
            for(cc=0; cc<conv1OutChan; cc++){
                for(x=0; x<conv1KSize; x++) {
                    #pragma nounroll
                    for(y=0; y<conv1KSize; y++){
                        #pragma nounroll
                        for(c=0; c<conv1InChan; c++){
                            // sum += convWindowBuff[InputIdx3D(x, y, c)] * kernel[KIdx4D(x,y,c,cc)];
                            sum += *convWindowBuff * kernel[KIdx4D(x,y,c,cc)];
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