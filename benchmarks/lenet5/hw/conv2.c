#include "../lenet5_hw_defines.h"

void conv2() {
    uint8_t* convWindow = (uint8_t*)Conv2Window;
    uint8_t* kernel = (uint8_t*)Conv2Weights;
    uint8_t* convOut = (uint8_t*)Conv2Output;
    // find center position of kernel (half of kernel size)
    int kCenter = conv2KSIZE / 2;
    int i,j,k,c,m,n;
    for(k = 0; k < conv2InputChannels; k++) {
        for (i = 1; i < INPUTDIMENSION-1; i++) {
            // #pragma clang loop unroll_count(8)
            for (j = 1; j < INPUTDIMENSION-1; j++) {
                uint8_t sum = 0;
                // TODO: Fix memory accesses and sizes to properly write channels out
                for(c = 0; c < conv2OutputChannels; c++){
                    for (m = 0; m < conv2KSIZE; m++) {
                        int mm = conv2KSIZE - 1 - m;
                        for (n = 0; n < conv2KSIZE; n++) {
                            int nn = conv2KSIZE - 1 - n;
                            // index of input signal, used for checking boundary
                            int ii = i + (m - kCenter);
                            int jj = j + (n - kCenter);
                            // ignore input samples which are out of bound
                            if (ii >= 0 && ii < INPUTDIMENSION && jj >= 0 && jj < INPUTDIMENSION)
                            sum += convWindow[ii*INPUTDIMENSION + jj] * kernel[mm*conv2KSIZE + nn];
                        }
                    }
                }
                // convOut[i*INPUTDIMENSION + j]=sum;
            }
        }
    }
}