#include "../lenet5_hw_defines.h"

void pool0() {
    uint8_t* convWindow = (uint8_t*)pool0Window;
    uint8_t* kernel = (uint8_t*)pool0Weights;
    uint8_t* convOut = (uint8_t*)pool0Output;
    // find center position of kernel (half of kernel size)
    int kCenter = POOL0KSIZE / 2;
    int i,j,k,c,m,n;
    for(k = 0; k < POOL0Channels; k++) {
        for (i = 1; i < POOL0InputDim-1; i+=POOL0KSIZE) {
            // #pragma clang loop unroll_count(8)
            for (j = 1; j < POOL0InputDim-1; j+=POOL0KSIZE) {
                uint8_t sum = 0;
                // TODO: Fix memory accesses and sizes to properly write channels out
                for(c = 0; c < POOL0Channels; c++){
                    for (m = 0; m < POOL0KSIZE; m++) {
                        int mm = POOL0KSIZE - 1 - m;
                        for (n = 0; n < POOL0KSIZE; n++) {
                            int nn = POOL0KSIZE - 1 - n;
                            // index of input signal, used for checking boundary
                            int ii = i + (m - kCenter);
                            int jj = j + (n - kCenter);
                            // ignore input samples which are out of bound
                            if (ii >= 0 && ii < POOL0InputDim && jj >= 0 && jj < POOL0InputDim)
                            sum += convWindow[ii*POOL0InputDim + jj] * kernel[mm*POOL0KSIZE + nn];
                        }
                    }
                }
                convOut[i/POOL0KSIZE*POOL0InputDim + j/POOL0KSIZE]=sum;
            }
        }
    }
}