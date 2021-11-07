#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_buf[conv2KSize][conv2InDim][conv2InChan];
typedef uint32_t array3d_win[conv2KSize][conv2KSize][conv2InChan];

void compute(uint32_t* strIn, array3d_win convWindow, array3d_buf convLineBuff) {
    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<conv2KSize; h++){
        #pragma nounroll
        for(w=0; w<conv2InDim; w++){
            #pragma nounroll
            for(c=0; c<conv2InChan; c++){
                convLineBuff[h][w][c] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<conv2OutDim; h++){
        // Once the first row is read data movement resumes
        if (h>=1) {
            #pragma nounroll
            for(w=0; w<conv2InDim; w++){
                #pragma nounroll
                for(c=0; c<conv2InChan; c++){
                    convLineBuff[(h-1)%5][w][c] = *strIn;
                }
            }
        }
        #pragma nounroll
        for (w=0; w<conv2OutDim; w++){
            #pragma nounroll
            for(cc=0; cc<conv2OutChan; cc++){
                #pragma unroll
                for(x=0; x<conv2KSize; x++) {
                    #pragma unroll
                    for(y=0; y<conv2KSize; y++){
                        #pragma unroll
                        for(c=0; c<conv2InChan; c++){
                            convWindow[x][y][c] = convLineBuff[(x + h%5)%5][y][c];
                        }
                    }
                }
            }
        }
    }
}

void top() {
    void* strIn = (void*)Pool1Out;
    void* convWindow = (void*)Conv2Window;
    void* convLineBuff = (void*)Conv2LineBuff;

	compute(strIn,convWindow,convLineBuff);

	return;
}