#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int *)0x2f000001
#define val_b      *(int *)0x2f000009
#define val_c      *(int *)0x2f000011
#define rcIndex     (r*sts->row_size + c)

typedef struct {
    int * inp;
    int * sol;
    int * filter;
    int * check;
    int row_size;
    int col_size;
    int f_size;
} stencil_struct;

void checkData( stencil_struct * sts ) {
    int r, c;
    
    for( r=0; r < sts->row_size; r++ ) {
        for( c=0; c < sts->col_size; c++ ) {
            if (sts->sol[rcIndex]!= sts->check[rcIndex]) {
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

void genData(stencil_struct * sts) {
    int r, c, k1, k2, temp, mul, sol;

    for( r=0; r < sts->row_size; r++ ) {
        for( c=0; c < sts->col_size; c++ ) {
            sts->inp[rcIndex] = rcIndex;
        }
    }

    for(k1=0; k1 < sts->f_size; k1++) {
        for (k2=0; k2 < sts->f_size; k2++){    
            sts->filter[k1*sts->f_size + k2] = k1*sts->f_size + k2;
        }
    }
    
    for( r=0; r < sts->row_size; r++ ) {
        for( c=0; c < sts->col_size; c++ ) {
            temp = 0;
            
            for(k1=0; k1 < sts->f_size; k1++) {
                for (k2=0; k2 < sts->f_size; k2++){                    
                    mul = sts->filter[k1*sts->f_size + k2] * sts->inp[(r+k1)*sts->col_size + c + k2];
                    temp += mul;
                }
            }
            sts->check[rcIndex] = temp;
        }
    }
}

