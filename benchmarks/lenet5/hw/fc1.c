#include "../lenet5_hw_defines.h"

#define InputIdx3D(i,j,k) (fc1InDim*fc1InDim*(k) + fc1InDim*(j) + i)
#define KIdx3D(i,j,k) (fc1KSize*fc1KSize*(k) + fc1KSize*(i) + j)
#define OutIdx3D(i,j,k) (fc1OutDim*fc1OutDim*(k) + fc1OutDim*(j-fc1KSize/2) + i-fc1KSize/2)

void fc1() {
    uint8_t* fcInput = (uint8_t*)fc1Input;
    uint8_t* kernel = (uint8_t*)fc1Weights;
    uint8_t* fcOut = (uint8_t*)fc1Output;
    uint8_t* fcLUT = (uint8_t*)fc1LUT;

    int i, j, k, l, m, n;
    #pragma clang loop unroll(disable)
    for (n = 0; n < fc1OutChan; n++){
        #pragma clang loop unroll(disable)
        for (k = 0; k < fc1InChan; k++){
            #pragma clang loop unroll(disable)
            for ( j = 0; j < fc1InDim; j++) {
                #pragma clang loop unroll(disable)
                for ( i = 0; i < fc1InDim; i++) {
                    if(!(i-fc1KSize/2<0 || j-fc1KSize/2<0
                        || i+fc1KSize/2>=fc1InDim
                        || j+fc1KSize/2 >= fc1InDim)){
                        int sum = 0;
                        #pragma clang loop unroll(disable)
                        for ( m = -fc1KSize/2; m < fc1KSize/2; m++) {
                            #pragma clang loop unroll(disable)
                            for ( l = -fc1KSize/2; l < fc1KSize/2; l++) {
                                sum += fcInput[InputIdx3D(i+l, j+m, k)]
                                * kernel[KIdx3D(l+fc1KSize/2, m+fc1KSize/2, n)];
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
    for (n = 0; n < fc1OutChan; n++){
        #pragma clang loop unroll(disable)
        for ( j = 0; j < fc1InDim; j++) {
            #pragma clang loop unroll(disable)
            for ( i = 0; i < fc1InDim; i++) {
                fcOut[OutIdx3D(i,j,n)] *= fcLUT[0];
            }
        }
    }
}