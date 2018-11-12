#include "../defines.h"

int dma_signal;
int dev_signal;

typedef struct {
    TYPE * a;
    TYPE * b;
    TYPE * c;
    TYPE * check;
    int length;
} vadd_struct;

void hostStep(int test);

int checkData(vadd_struct * vas) {
    int i;
    for (i = 0; i < vas->length; i++) {
        if (vas->c[i] != vas->check[i]) {
            printf("Check Failed\n");
            return 0;
        }
    }
    printf("Check Passed\n");
    return 1;
}

void genData(vadd_struct * vas) {
    int i;
    for (i = 0; i < vas->length; i++) {
        vas->a[i] = (TYPE)i;
        vas->b[i] = (TYPE)(vas->length - i);
        vas->check[i] = vas->a[i]+vas->b[i];
    }
}
