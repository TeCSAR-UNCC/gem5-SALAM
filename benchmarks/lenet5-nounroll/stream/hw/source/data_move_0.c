#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_buf[conv0KSize][conv0InDim][conv0InChan];
typedef uint32_t array3d_win[conv0KSize][conv0KSize][conv0InChan];

void compute(uint32_t* strIn, array3d_win convWindow, array3d_buf convLineBuff) {
    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<conv0KSize; h++){
        #pragma nounroll
        for(w=0; w<conv0InDim; w++){
            #pragma nounroll
            for(c=0; c<conv0InChan; c++){
                convLineBuff[h][w][c] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<conv0OutDim; h++){
        // Once the first row is read data movement resumes
        if (h>=1) {
            #pragma nounroll
            for(w=0; w<conv0InDim; w++){
                #pragma nounroll
                for(c=0; c<conv0InChan; c++){
                    convLineBuff[(h-1)%5][w][c] = *strIn;
                }
            }
        }
        #pragma nounroll
        for (w=0; w<conv0OutDim; w++){
            #pragma nounroll
            for(cc=0; cc<conv0OutChan; cc++){
                #pragma unroll
                for(x=0; x<conv0KSize; x++) {
                    #pragma unroll
                    for(y=0; y<conv0KSize; y++){
                        #pragma unroll
                        for(c=0; c<conv0InChan; c++){
                            convWindow[x][y][c] = convLineBuff[(x + h%5)%5][y][c];
                        }
                    }
                }
            }
        }
    }
}

void top() {
    void* strIn = (void*)STREAMDMA_Stream;
    void* convWindow = (void*)Conv0Window;
    void* convLineBuff = (void*)Conv0LineBuff;

	compute(strIn,convWindow,convLineBuff);

	return;
}