#include "../lenet5_hw_defines.h"

#define InputIdx3D(i,j,k) (pool1InDim*pool1InDim*(k) + pool1InDim*(j) + i)
#define KIdx3D(i,j,k) (pool1KSize*pool1KSize*(k) + pool1KSize*(i) + j)
#define OutIdx3D(i,j,k) (pool1OutDim*pool1OutDim*(k) + pool1OutDim*(j/pool1KSize) + i/pool1KSize)

void pool1() {
    uint8_t* convInput = (uint8_t*)pool1Input;
    uint8_t* convOut = (uint8_t*)pool1Output;

    int i, j, k, l, m;
    for (k = 0; k < pool1InChan; k++){
        for ( j = 0; j < pool1InDim; j+=2) {
            for ( i = 0; i < pool1InDim; i+=2) {
                int sum = 0;
                for (m = 0; m < pool1KSize; m++) {
                    for ( l = 0; l < pool1KSize; l++) {
                        sum += convInput[InputIdx3D(i+l, j+m, k)];
                    }
                }
                convOut[OutIdx3D(i,j,k)] += sum/(pool1KSize*pool1KSize);
            }
        }
    }
}