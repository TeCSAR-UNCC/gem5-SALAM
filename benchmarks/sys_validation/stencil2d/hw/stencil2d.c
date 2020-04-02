#include "hw_defines.h"

void stencil (){
    uint8_t * origbase   = (uint8_t *)ORIGADDR;
    uint8_t * solbase    = (uint8_t *)SOLADDR;
    uint8_t * filterbase = (uint8_t *)FILTERADDR;
    TYPE    * orig       = (TYPE    *)origbase;
    TYPE    * sol        = (TYPE    *)solbase;
    TYPE    * filter     = (TYPE    *)filterbase;
    int r, c, k1, k2;
    TYPE temp, mul;

    stencil_label1:
    #pragma clang loop unroll(disable)
    for (c=0; c<col_size-2; c++) {
        stencil_label2:
        #pragma clang loop unroll_count(2)
        for (r=0; r<row_size-2; r++) {
            temp = (TYPE)0;
            stencil_label3:
            #pragma clang loop unroll(disable)
            for (k1=0;k1<3;k1++){
                stencil_label4:
                #pragma clang loop unroll(full)
                for (k2=0;k2<3;k2++){
                    mul = filter[k1*3 + k2] * orig[(r+k1)*col_size + c+k2];
                    temp += mul;
                }
            }
            sol[(r*col_size) + c] = temp;
        }
    }
}
