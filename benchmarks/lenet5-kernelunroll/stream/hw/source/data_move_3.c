#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_buf[fc0KSize][fc0InDim][fc0InChan];
typedef uint32_t array3d_win[fc0KSize][fc0KSize][fc0InChan];

void compute(uint32_t* strIn, array3d_win convWindow, array3d_buf convLineBuff) {
    int h,w,c,cc,x,y;
    uint32_t sum;

    // Warmup loop
    #pragma nounroll
    for(h=0; h<fc0KSize; h++){
        #pragma nounroll
        for(w=0; w<fc0InDim; w++){
            #pragma nounroll
            for(c=0; c<fc0InChan; c++){
                convLineBuff[h][w][c] = *strIn;
            }
        }
    }

    #pragma nounroll
    for (h=0; h<fc0OutDim; h++){
        #pragma nounroll
        for (w=0; w<fc0OutDim; w++){
            #pragma nounroll
            for(cc=0; cc<fc0OutChan; cc++){
                #pragma unroll
                for(x=0; x<fc0KSize; x++) {
                    #pragma unroll
                    for(y=0; y<fc0KSize; y++){
                        #pragma unroll
                        for(c=0; c<fc0InChan; c++){
                            convWindow[x][y][c] = convLineBuff[(x + h%5)%5][y][c];
                        }
                    }
                }
            }
        }
    }
}

void top() {
    void* strIn = (void*)Conv2Out;
    void* convWindow = (void*)FC0Window;
    void* convLineBuff = (void*)FC0LineBuff;

	compute(strIn,convWindow,convLineBuff);

	return;
}