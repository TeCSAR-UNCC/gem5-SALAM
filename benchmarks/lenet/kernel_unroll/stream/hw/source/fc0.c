#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_in[fc0KSize][fc0KSize][fc0InChan];
typedef uint32_t array4d_t[fc0KSize][fc0KSize][fc0InChan][fc0OutChan];

void compute(array3d_in convWin, array4d_t kernel, uint32_t* strOut) {

    int h,w,c,cc,x,y;
    uint32_t sum;

    #pragma nounroll
    for (h=0; h<fc0OutDim; h++){
        #pragma nounroll
        for (w=0; w<fc0OutDim; w++){
            sum = 0;
            #pragma nounroll
            for(cc=0; cc<fc0OutChan; cc++){
                #pragma unroll
                for(x=0; x<fc0KSize; x++) {
                    #pragma unroll
                    for(y=0; y<fc0KSize; y++){
                        #pragma unroll
                        for(c=0; c<fc0InChan; c++){
                            sum += convWin[x][y][c] * kernel[x][y][c][cc];
                        }
                    }
                }
                
            // if(sum >= 2){
            //     sum = sum*0.964027580076;
            // } else if (sum < 2 && sum >= 1){
            //     sum = sum*0.761594155956;
            // } else if (sum < 1 && sum >= .5){
            //     sum = sum*0.46211715726;
            // } else if (sum < .5 && sum >= .25){
            //     sum = sum*.244918662404;
            // } else if (sum < .25 && sum >= 0){
            //     sum = sum*0;
            // } else if (sum < 0 && sum >= -.25){
            //     sum = sum*-.244918662404;
            // } else if (sum < -.25 && sum >= -.5){
            //     sum = sum*-0.46211715726;
            // } else if (sum < -.5 && sum >= -1){
            //     sum = sum*-0.761594155956;
            // } else if (sum > -1){
            //     sum = sum*-0.964027580076;
            // }
            *strOut = sum;
            }
        }
    }
}

void top() {
    void* convWin = (void*)FC0Window;
    void* kernel = (void*)FC0Weights;
    void* strOut = (void*)STREAMDMA_Stream;

	compute(convWin,kernel,strOut);

	return;
}