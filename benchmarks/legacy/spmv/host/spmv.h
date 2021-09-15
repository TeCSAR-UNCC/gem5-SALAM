#include "../defines.h"
#include "data.h"

#define common_val   *(unsigned *)0x80b00000
#define acc          *(char *)0x2f000000
#define loc_val      *(int *)0x2f000001
#define loc_cols     *(int *)0x2f000009
#define loc_rows     *(int *)0x2f000011
#define loc_vec      *(int *)0x2f000019
#define loc_out      *(int *)0x2f000021

#define EPSILON 1.0e-6

typedef struct {
    TYPE * val;
    int * cols;
    int * rowDelimiters;
    TYPE * vec;
    TYPE * out;
    TYPE * check;
} spmv_struct;

int checkData(spmv_struct * sps) {
    int i;
    for (i = 0; i < N; i++) {
        if(((sps->out[i]-sps->check[i]) > EPSILON) || ((sps->out[i]-sps->check[i]) < -EPSILON)) {
            printf("Check Failed\n");
            return 0;
        }
    }
    printf("Check Passed\n");
    return 1;
}

void genData(spmv_struct * sps) {
    int i;
    for (i = 0; i < NNZ; i++) {
        sps->val[i] = mat[i];
        sps->cols[i] = col[i];
    }
    for (i = 0; i <= N; i++) {
        sps->rowDelimiters[i] = row[i];
    }
    for (i = 0; i < N; i++) {
        sps->vec[i] = vect[i];
        sps->check[i] = chk[i];
    }
}
