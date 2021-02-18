#include "../lenet5_hw_defines.h"

void pool1() {
    uint8_t* convWindow = (uint8_t*)pool1Window;
    uint8_t* kernel = (uint8_t*)pool1Weights;
    uint8_t* convOut = (uint8_t*)pool1Output;
    // find center position of kernel (half of kernel size)
    int kCenter = POOL1KSIZE / 2;
    int i,j,k,c,m,n;
    for(k = 0; k < POOL1Channels; k++) {
        for (i = 1; i < POOL1InputDim-1; i+=POOL1KSIZE) {
            // #pragma clang loop unroll_count(8)
            for (j = 1; j < POOL1InputDim-1; j+=POOL1KSIZE) {
                uint8_t sum = 0;
                // TODO: Fix memory accesses and sizes to properly write channels out
                for(c = 0; c < POOL1Channels; c++){
                    for (m = 0; m < POOL1KSIZE; m++) {
                        int mm = POOL1KSIZE - 1 - m;
                        for (n = 0; n < POOL1KSIZE; n++) {
                            int nn = POOL1KSIZE - 1 - n;
                            // index of input signal, used for checking boundary
                            int ii = i + (m - kCenter);
                            int jj = j + (n - kCenter);
                            // ignore input samples which are out of bound
                            if (ii >= 0 && ii < POOL1InputDim && jj >= 0 && jj < POOL1InputDim)
                            sum += convWindow[ii*POOL1InputDim + jj] * kernel[mm*POOL1KSIZE + nn];
                        }
                    }
                }
                convOut[i/POOL1KSIZE*POOL1InputDim + j/POOL1KSIZE]=sum;
            }
        }
    }
}