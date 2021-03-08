#include "../lenet5_hw_defines.h"

#define InputIdx3D(i,j,k) (fc0InDim*fc0InDim*(k) + fc0InDim*(j) + i)
#define KIdx3D(i,j,k) (fc0KSize*fc0KSize*(k) + fc0KSize*(i) + j)
#define OutIdx3D(i,j,k) (fc0OutDim*fc0OutDim*(k) + fc0OutDim*(j-fc0KSize/2) + i-fc0KSize/2)

void fc0() {
    uint8_t* fcInput = (uint8_t*)fc0Input;
    uint8_t* kernel = (uint8_t*)fc0Weights;
    uint8_t* fcOut = (uint8_t*)fc0Output;
    uint8_t* fcLUT = (uint8_t*)fc0LUT;

    int i, j, k, l, m, n;
    #pragma clang loop unroll(disable)
    for (n = 0; n < fc0OutChan; n++){
        #pragma clang loop unroll(disable)
        for (k = 0; k < fc0InChan; k++){
            #pragma clang loop unroll(disable)
            for ( j = 0; j < fc0InDim; j++) {
                #pragma clang loop unroll(disable)
                for ( i = 0; i < fc0InDim; i++) {
                    if(!(i-fc0KSize/2<0 || j-fc0KSize/2<0
                        || i+fc0KSize/2>=fc0InDim
                        || j+fc0KSize/2 >= fc0InDim)){
                        int sum = 0;
                        #pragma clang loop unroll(disable)
                        for ( m = -fc0KSize/2; m < fc0KSize/2; m++) {
                            #pragma clang loop unroll(disable)
                            for ( l = -fc0KSize/2; l < fc0KSize/2; l++) {
                                sum += fcInput[InputIdx3D(i+l, j+m, k)]
                                * kernel[KIdx3D(l+fc0KSize/2, m+fc0KSize/2, n)];
                            }
                        }
                        fcOut[OutIdx3D(i,j,n)] += sum;
                    }
                }
            }
        }
    }
    // Apply the activation function
    #pragma clang loop unroll(disable)
    for (n = 0; n < fc0OutChan; n++){
        #pragma clang loop unroll(disable)
        for ( j = 0; j < fc0InDim; j++) {
            #pragma clang loop unroll(disable)
            for ( i = 0; i < fc0InDim; i++) {
                fcOut[OutIdx3D(i,j,n)] *= fcLUT[0];
            }
        }
    }
}