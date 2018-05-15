#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int *)0x2f000001
#define val_b      *(int *)0x2f000009
#define val_c      *(int *)0x2f000011
#define SIZE (sts->row_size * sts->col_size * sts->height_size)
#define INDX(_row_size,_col_size,_i,_j,_k) ((_i)+_row_size*((_j)+_col_size*(_k)))

typedef struct {
    int * inp;
    int * sol;
    int * C;
    int * check;
    int row_size;
    int col_size;
    int height_size;
    int C_size;
} stencil_struct;

void checkData( stencil_struct * sts ) {
    int i, j, k;
    
    for( i=0; i < SIZE; i++ ) {
        if (sts->sol[INDX(sts->row_size, sts->col_size, k, j, i)]!= sts->check[INDX(sts->row_size, sts->col_size, k, j, i)]) {
            printf("Check Failed\n");
            return;
        }
    }
    
    printf("Check Passed\n");
}

//void genData(vadd_struct * vas) {
//    int i;
//    for (i = 0; i < vas->length; i++) {
//        vas->a[i] = i;
//        vas->b[i] = vas->length - 1- i;
//        vas->check[i] = vas->a[i] + vas->b[i];
//    }
//}

void genData(stencil_struct * sts) {

    int i, j, k, sum0, sum1, mul0, mul1;    
    
    for( i=0; i < SIZE; i++ ) {
        sts->inp[i] = i;
    }
    
    for( i=0; i < sts->C_size; i++ ) {
        sts->C[i] = i;
    }
    
    for(j=0; j < sts->col_size; j++) {
        for(k=0; k < sts->row_size; k++) {
            sts->check[INDX(sts->row_size, sts->col_size, k, j, 0)] = sts->inp[INDX(sts->row_size, sts->col_size, k, j, 0)];
            sts->check[INDX(sts->row_size, sts->col_size, k, j, sts->height_size-1)] = sts->inp[INDX(sts->row_size, sts->col_size, k, j, sts->height_size-1)];
        }
    }
    for(i=1; i < sts->height_size-1; i++) {
        for(k=0; k < sts->row_size; k++) {
            sts->check[INDX(sts->row_size, sts->col_size, k, 0, i)] = sts->inp[INDX(sts->row_size, sts->col_size, k, 0, i)];
            sts->check[INDX(sts->row_size, sts->col_size, k, sts->col_size-1, i)] = sts->inp[INDX(sts->row_size, sts->col_size, k, sts->col_size-1, i)];
        }
    }
    for(i=1; i < sts->height_size-1; i++) {
        for(j=1; j < sts->col_size-1; j++) {
            sts->check[INDX(sts->row_size, sts->col_size, 0, j, i)] = sts->inp[INDX(sts->row_size, sts->col_size, 0, j, i)];
            sts->check[INDX(sts->row_size, sts->col_size, sts->row_size-1, j, i)] = sts->inp[INDX(sts->row_size, sts->col_size, sts->row_size-1, j, i)];
        }
    }


    // Stencil computation
    for(i = 1; i < sts->height_size - 1; i++){
        for(j = 1; j < sts->col_size - 1; j++){
            for(k = 1; k < sts->row_size - 1; k++){
                sum0 = sts->inp[INDX(sts->row_size, sts->col_size, k, j, i)];
                sum1 = sts->inp[INDX(sts->row_size, sts->col_size, k, j, i + 1)] +
                       sts->inp[INDX(sts->row_size, sts->col_size, k, j, i - 1)] +
                       sts->inp[INDX(sts->row_size, sts->col_size, k, j + 1, i)] +
                       sts->inp[INDX(sts->row_size, sts->col_size, k, j - 1, i)] +
                       sts->inp[INDX(sts->row_size, sts->col_size, k + 1, j, i)] +
                       sts->inp[INDX(sts->row_size, sts->col_size, k - 1, j, i)];
                mul0 = sum0 * sts->C[0];
                mul1 = sum1 * sts->C[1];
                sts->check[INDX(sts->row_size, sts->col_size, k, j, i)] = mul0 + mul1;
            }
        }
    }
}

