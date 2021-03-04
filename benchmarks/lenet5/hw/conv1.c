#include "../lenet5_hw_defines.h"

#define InputIdx3D(i,j,k) (conv1InDim*conv1InDim*(k) + conv1InDim*(j) + i)
#define KIdx3D(i,j,k) (conv1KSize*conv1KSize*(k) + conv1KSize*(i) + j)
#define OutIdx3D(i,j,k) (conv1OutDim*conv1OutDim*(k) + conv1OutDim*(j-conv1KSize/2) + i-conv1KSize/2)

void conv1() {
    uint8_t* convInput = (uint8_t*)Conv1Input;
    uint8_t* kernel = (uint8_t*)Conv1Weights;
    uint8_t* convOut = (uint8_t*)Conv1Output;
    int i, j, k, l, m, n;
    for (n = 0; n < conv1OutChan; n++){
        for (k = 0; k < conv1InChan; k++){
            for ( j = 0; j < conv1InDim; j++) {
                for ( i = 0; i < conv1InDim; i++) {
                    if(!(i-conv1KSize/2<0 || j-conv1KSize/2<0
                        || i+conv1KSize/2>=conv1InDim
                        || j+conv1KSize/2 >= conv1InDim)){
                        int sum = 0;
                        for ( m = -conv1KSize/2; m < conv1KSize/2; m++) {
                            for ( l = -conv1KSize/2; l < conv1KSize/2; l++) {
                                sum += convInput[InputIdx3D(i+l, j+m, k)]
                                * kernel[KIdx3D(l+conv1KSize/2, m+conv1KSize/2, n)];
                            }
                        }
                        convOut[OutIdx3D(i,j,n)] += sum;
                    }
                }
            }
        }
    }
}