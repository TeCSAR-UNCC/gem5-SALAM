#include "../defines.h"

#define rcIndex     (r*ROW + c)

typedef struct {
    TYPE * a;
    TYPE * b;
    TYPE * c;
    int row_size;
    int col_size;
} gemm_struct;

typedef struct {
    int * in;
    int * coefficient;
    int * previous;
    int * temp;
    int * check;
} fir_struct;

int checkFirData(fir_struct * fir) {
    int i, total = 0;

    for (i = 0; i < INPUTSIZE; i++) {
        total += fir->temp[i];
        printf("Temp[%2d] %5d\n", i, fir->temp[i]);
    }

    printf("Total %5d, Check %5d\n", total, fir->check[0]);
    if(total != fir->check[0]) {
        printf("Check Failed\n");
        return 0;
    }
    printf("Check Passed\n");
    return 1;
}

void genGemmData(gemm_struct * ges) {
    int r, c, k, mult, sum;

    for( r=0; r < ges->row_size; r++ ) {
        for( c=0; c < ges->col_size; c++ ) {
            ges->a[rcIndex] = rcIndex;
            ges->b[rcIndex] = (ges->row_size * ges->col_size) - 1 - rcIndex;
        }
    }
}

void genFirData(fir_struct * fir) {
    int i;

    for (i = 0; i < INPUTSIZE; i++) {
        fir->in[i] = i+1;
    }

    for (i = 0; i < TAPS; i++) {
        fir->previous[i] = 0;
        fir->coefficient[i] = 10;
        printf("C[%2d] = %5d\n", i, fir->coefficient[i]);
    }
    fir->check[0] = EXPECTED_TOTAL;
}
