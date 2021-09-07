#include "hw_defines.h"

void stencil (){
    TYPE    * orig       = (TYPE    *)ORIG;
    TYPE    * sol        = (TYPE    *)SOL;
    TYPE    * filter     = (TYPE    *)FILTER;
    int r, c, k1, k2;
    TYPE temp, mul;

    stencil_label1:
    #pragma nounroll
    for (c=0; c<col_size-2; c++) {
        stencil_label2:
        #pragma unroll 2
        for (r=0; r<row_size-2; r++) {
            temp = (TYPE)0;
            stencil_label3:
            #pragma nounroll
            for (k1=0;k1<3;k1++){
                stencil_label4:
                #pragma unroll
                for (k2=0;k2<3;k2++){
                    mul = filter[k1*3 + k2] * orig[(r+k1)*col_size + c+k2];
                    temp += mul;
                }
            }
            sol[(r*col_size) + c] = temp;
        }
    }
}
