#include "../defines.h"

#define common_val *(unsigned *)0x80b00000
#define acc        *(char *)0x2f000000
#define val_a      *(int *)0x2f000001
#define val_b      *(int *)0x2f000009
#define val_c      *(int *)0x2f000011

typedef struct {
    TYPE * a;
    TYPE * b;
    TYPE * c;
    TYPE * check;
    int length;
} vadd_struct;

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
    vas->a[0] = 2;
    for (i = 0; i < vas->length; i++) {
        vas->c[i] = (TYPE)(vas->length - i);
        vas->check[i] = vas->a[0]+vas->c[i];
    }
}
