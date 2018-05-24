#include "../defines.h"

#define common_val  *(unsigned *)0x80b00000
#define acc         *(char *)0x2f000000
#define loc_C       *(int *)0x2f000001
#define loc_inp     *(int *)0x2f000009
#define loc_sol     *(int *)0x2f000011

#define EPSILON 1.0e-6

typedef struct {
    TYPE * C;
    TYPE * inp;
    TYPE * sol;
    TYPE * check;
//    int row_size;
//    int col_size;
//    int height_size;
//    int C_size;
} stencil_struct;

int checkData( stencil_struct * sts ) {
    int i, j, k;
    
    for( i=0; i < SIZE; i++ ) {
        if (sts->sol[i]!= sts->check[i]) {
            printf("Check Failed\n");
            return 0;
        }
    }
    printf("Check Passed\n");
    
    return 1;
}

void genData(stencil_struct * sts) {

    int i, j, k, sum0, sum1, mul0, mul1;    
    
    for( i=0; i < SIZE; i++ ) {
        sts->inp[i] = i;
    }
    
    for( i=0; i < C_SIZE; i++ ) {
        sts->C[i] = i+2;
    }
    
    for(j=0; j < COL; j++) {
        for(k=0; k < ROW; k++) {
            sts->check[INDX(ROW, COL, k, j, 0)] = sts->inp[INDX(ROW, COL, k, j, 0)];
            sts->check[INDX(ROW, COL, k, j, HYT-1)] = sts->inp[INDX(ROW, COL, k, j, HYT-1)];
        }
    }
    for(i=1; i < HYT-1; i++) {
        for(k=0; k < ROW; k++) {
            sts->check[INDX(ROW, COL, k, 0, i)] = sts->inp[INDX(ROW, COL, k, 0, i)];
            sts->check[INDX(ROW, COL, k, COL-1, i)] = sts->inp[INDX(ROW, COL, k, COL-1, i)];
        }
    }
    for(i=1; i < HYT-1; i++) {
        for(j=1; j < COL-1; j++) {
            sts->check[INDX(ROW, COL, 0, j, i)] = sts->inp[INDX(ROW, COL, 0, j, i)];
            sts->check[INDX(ROW, COL, ROW-1, j, i)] = sts->inp[INDX(ROW, COL, ROW-1, j, i)];
        }
    }


    // Stencil computation
    for(i = 1; i < HYT - 1; i++){
        for(j = 1; j < COL - 1; j++){
            for(k = 1; k < ROW - 1; k++){
                sum0 = sts->inp[INDX(ROW, COL, k, j, i)];
                sum1 = sts->inp[INDX(ROW, COL, k, j, i + 1)] +
                       sts->inp[INDX(ROW, COL, k, j, i - 1)] +
                       sts->inp[INDX(ROW, COL, k, j + 1, i)] +
                       sts->inp[INDX(ROW, COL, k, j - 1, i)] +
                       sts->inp[INDX(ROW, COL, k + 1, j, i)] +
                       sts->inp[INDX(ROW, COL, k - 1, j, i)];
                mul0 = sum0 * sts->C[0];
                mul1 = sum1 * sts->C[1];
                sts->check[INDX(ROW, COL, k, j, i)] = mul0 + mul1;
            }
        }
    }
}

