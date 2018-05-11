#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int *)0x2f000001
#define val_b      *(int *)0x2f000009
#define val_c      *(int *)0x2f000011
#define rcIndex     (r*ges->row_size + c)

typedef struct {
    int * a;
    int * b;
    int * c;
    int * check;
    int row_size;
    int col_size;
} gemm_struct;

//void checkData(vadd_struct * vas) {
//    int i;
//    for (i = 0; i < vas->length; i++) {
//        if (vas->c[i] != vas->check[i]) {
//            printf("Check Failed\n");
//            return;
//        }
//    }
//    printf("Check Passed\n");
//}

void checkData( gemm_struct * ges ) {
    int r, c;
    
    for( r=0; r < ges->row_size; r++ ) {
        for( c=0; c < ges->col_size; c++ ) {
            if (ges->c[rcIndex]!= ges->check[rcIndex]) {
                printf("Check Failed\n");
                return;
            }
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

void genData(gemm_struct * ges) {
    int r, c, k, mult, sum;
    
    for( r=0; r < ges->row_size; r++ ) {
        for( c=0; c < ges->col_size; c++ ) {
            ges->a[rcIndex] = rcIndex;
            ges->b[rcIndex] = (ges->row_size * ges->col_size) - 1 - rcIndex;
        }
    }
        
    ges->check[0] = 0;
    for( r=0; r < ges->row_size; r++ ) {
        for( c=0; c < ges->col_size; c++ ) {
            sum = ges->check[rcIndex];
            for(k=0; k < ges->row_size; k++) {
                mult = ges->a[(r*ges->row_size + k)] * ges->b[(k*ges->col_size + c)];
                sum += mult;
            }
            ges->check[rcIndex] = sum;
        }
    }
}

