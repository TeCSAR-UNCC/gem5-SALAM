#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_win[pool1KSize][pool1InDim][pool1InChan];

void data_mover(uint32_t* strIn, array3d_win poolWin) {
    int h,w,c;
    // Shift Window
    #pragma nounroll
    for(h = 0; h < pool1InDim; h++){
        #pragma nounroll
        for (w = 0; w < pool1InDim; w++) {
            #pragma nounroll
            for(c = 0; c < pool1InChan; c++) {
                poolWin[h%2][w][c] = *strIn;
            }
        }
    }
}

void top() {
    void* strIn = (void*)Conv1Out;
    void* poolWin = (void*)Pool1Window;

    data_mover(strIn,poolWin);

	return;
}