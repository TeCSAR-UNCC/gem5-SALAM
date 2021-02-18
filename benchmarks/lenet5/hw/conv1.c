#include "../lenet5_hw_defines.h"

void conv1() {
    uint8_t* convWindow = (uint8_t*)Conv1Window;
    uint8_t* kernel = (uint8_t*)Conv1Weights;
    uint8_t* convOut = (uint8_t*)Conv1Output;
    // find center position of kernel (half of kernel size)
    int kCenter = conv1KSIZE / 2;
    int i,j,k,c,m,n;
    for(k = 0; k < conv1InputChannels; k++) {
        for (i = 1; i < INPUTDIMENSION-1; i++) {
            // #pragma clang loop unroll_count(8)
            for (j = 1; j < INPUTDIMENSION-1; j++) {
                uint8_t sum = 0;
                // TODO: Fix memory accesses and sizes to properly write channels out
                for(c = 0; c < conv1OutputChannels; c++){
                    for (m = 0; m < conv1KSIZE; m++) {
                        int mm = conv1KSIZE - 1 - m;
                        for (n = 0; n < conv1KSIZE; n++) {
                            int nn = conv1KSIZE - 1 - n;
                            // index of input signal, used for checking boundary
                            int ii = i + (m - kCenter);
                            int jj = j + (n - kCenter);
                            // ignore input samples which are out of bound
                            if (ii >= 0 && ii < INPUTDIMENSION && jj >= 0 && jj < INPUTDIMENSION)
                            sum += convWindow[ii*INPUTDIMENSION + jj] * kernel[mm*conv1KSIZE + nn];
                        }
                    }
                }
                convOut[i*INPUTDIMENSION + j]=sum;
            }
        }
    }
}