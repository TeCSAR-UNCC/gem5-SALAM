#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array4d_t[conv2KSize][conv2KSize][conv2InChan][conv2OutChan];
typedef uint32_t array3d_buf[conv2KSize][conv2InDim][conv2InChan];

void compute(uint32_t* strIn, array3d_buf convWin, array4d_t kernel, uint32_t* strOut) {

    int h,w,c,cc,x,y;
    uint32_t sum;

    #pragma nounroll
    for(h=0; h<conv2KSize; h++){
        #pragma nounroll
        for(w=0; w<conv2InDim; w++){
            #pragma nounroll
            for(c=0; c<conv2InChan; c++){
                convWin[h][w][c] = *strIn;
            }
        }
    }
    sum = 0;
    #pragma nounroll
    for(cc=0; cc<conv2OutChan; cc++){
        #pragma unroll
        for(x=0; x<conv2KSize; x++) {
            #pragma unroll
            for(y=0; y<conv2KSize; y++){
                #pragma unroll
                for(c=0; c<conv2InChan; c++){
                    sum += convWin[x][y][c] * kernel[x][y][c][cc];
                }
            }
        }
    *strOut = sum;
    }
}

void top() {
    void* strIn = (void*)Pool1Out;
    void* buffer = (void*)Conv2LineBuff;
    void* kernel = (void*)Conv2Weights;
    void* strOut = (void*)Conv2Out;

	compute(strIn,buffer,kernel,strOut);

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