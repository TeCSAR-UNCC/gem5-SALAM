#include "../lenet5_hw_defines.h"

#define InputIdx3D(i,j,k) (conv0InDim*conv0InDim*(k) + conv0InDim*(j) + i)
#define KIdx3D(i,j,k) (conv0KSize*conv0KSize*(k) + conv0KSize*(i) + j)
#define OutIdx3D(i,j,k) (conv0OutDim*conv0OutDim*(k) + conv0OutDim*(j-conv0KSize/2) + i-conv0KSize/2)

void conv0() {
    uint8_t* convInput = (uint8_t*)Conv0Input;
    uint8_t* kernel = (uint8_t*)Conv0Weights;
    uint8_t* convOut = (uint8_t*)Conv0Output;
    int i, j, k, l, m, n;
    for (n = 0; n < conv0OutChan; n++){
        for (k = 0; k < conv0InChan; k++){
            for ( j = 0; j < conv0InDim; j++) {
                for ( i = 0; i < conv0InDim; i++) {
                    if(!(i-conv0KSize/2<0 || j-conv0KSize/2<0
                        || i+conv0KSize/2>=conv0InDim
                        || j+conv0KSize/2 >= conv0InDim)){
                        int sum = 0;
                        for ( m = -conv0KSize/2; m < conv0KSize/2; m++) {
                            for ( l = -conv0KSize/2; l < conv0KSize/2; l++) {
                                sum += convInput[InputIdx3D(i+l, j+m, k)]
                                * kernel[KIdx3D(l+conv0KSize/2, m+conv0KSize/2, n)];
                            }
                        }
                        convOut[OutIdx3D(i,j,n)] += sum;
                    }
                }
            }
        }
    }
}