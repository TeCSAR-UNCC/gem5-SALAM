#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_win[pool0KSize][pool0InDim][pool0InChan];

void data_mover(uint32_t* strIn, array3d_win poolWin) {
    int h,w,c;
    // Shift Window
    #pragma nounroll
    for(h = 0; h < pool0InDim; h++){
        #pragma nounroll
        for (w = 0; w < pool0InDim; w++) {
            #pragma nounroll
            for(c = 0; c < pool0InChan; c++) {
                poolWin[h%2][w][c] = *strIn;
            }
        }
    }
}

void top() {
    void* strIn = (void*)Conv0Out;
    void* poolWin = (void*)Pool0Window;

    data_mover(strIn,poolWin);

	return;
}