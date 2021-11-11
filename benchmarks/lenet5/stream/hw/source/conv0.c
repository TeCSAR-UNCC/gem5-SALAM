#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_buf[conv0KSize][conv0InDim][conv0InChan];
typedef uint32_t array3d_win[conv0KSize][conv0KSize][conv0InChan];
typedef uint32_t array4d_t[conv0KSize][conv0KSize][conv0InChan][conv0OutChan];

void compute(uint32_t* strIn, array3d_buf convLineBuff, array4d_t kernel, uint32_t* strOut) {

    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<conv0KSize; h++){
        #pragma nounroll
        for(w=0; w<conv0InDim; w++){
            #pragma nounroll
            for(c=0; c<conv0InChan; c++){
                convLineBuff[h][w][c] = *strIn;
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
                    convLineBuff[(h-1)%5][w][c] = *strIn;
                }
            }
        }
        #pragma nounroll
        for (w=0; w<conv0OutDim; w++){
            sum = 0;
            #pragma nounroll
            for(cc=0; cc<conv0OutChan; cc++){
                #pragma unroll
                for(x=0; x<conv0KSize; x++) {
                    #pragma unroll
                    for(y=0; y<conv0KSize; y++){
                        #pragma unroll
                        for(c=0; c<conv0InChan; c++){
                            sum += convLineBuff[(x + h%5)%5][y + w][c] * kernel[x][y][c][cc];
                        }
                    }
                }
            *strOut = sum;
            }
        }
    }
}

void top() {
    void* strIn = (void*)STREAMDMA_Stream;
    void* convLineBuff = (void*)Conv0LineBuff;
    void* kernel = (void*)Conv0Weights;
    void* strOut = (void*)Conv0Out;

	compute(strIn,convLineBuff,kernel,strOut);

	return;
}

// Old activation code below
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