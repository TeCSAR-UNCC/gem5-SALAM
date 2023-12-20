#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_buf[conv1KSize][conv1InDim][conv1InChan];
typedef uint32_t array3d_win[conv1KSize][conv1KSize][conv1InChan];

void compute(uint32_t* strIn, array3d_win convWindow, array3d_buf convLineBuff) {
    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<conv1KSize; h++){
        #pragma nounroll
        for(w=0; w<conv1InDim; w++){
            #pragma nounroll
            for(c=0; c<conv1InChan; c++){
                convLineBuff[h][w][c] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<conv1OutDim; h++){
        // Once the first row is read data movement resumes
        if (h>=1) {
            #pragma nounroll
            for(w=0; w<conv1InDim; w++){
                #pragma nounroll
                for(c=0; c<conv1InChan; c++){
                    convLineBuff[(h-1)%5][w][c] = *strIn;
                }
            }
        }
        #pragma nounroll
        for (w=0; w<conv1OutDim; w++){
            #pragma nounroll
            for(cc=0; cc<conv1OutChan; cc++){
                #pragma unroll
                for(x=0; x<conv1KSize; x++) {
                    #pragma unroll
                    for(y=0; y<conv1KSize; y++){
                        #pragma unroll
                        for(c=0; c<conv1InChan; c++){
                            convWindow[x][y][c] = convLineBuff[(x + h%5)%5][y][c];
                        }
                    }
                }
            }
        }
    }
}

void top() {
    void* strIn = (void*)Pool0Out;
    void* convWindow = (void*)Conv1Window;
    void* convLineBuff = (void*)Conv1LineBuff;

	compute(strIn,convWindow,convLineBuff);

	return;
}