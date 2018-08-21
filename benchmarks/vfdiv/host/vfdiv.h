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
} vfdiv_struct;

int checkData(vfdiv_struct * vas) {
    int i;
    for (i = 0; i < vas->length; i++) {
	    printf("c[i] = %p\n", (void *) &vas->c[i]);
		printf("%.9g = %.9g\n",vas->c[i], vas->check[i]);
        if (vas->c[i] != vas->check[i]) {
            printf("Check Failed\n");
        }
    }
    printf("Check Passed\n");
    return 1;
}

void genData(vfdiv_struct * vas) {
    int i;
    for (i = 0; i < vas->length; i++) {
        vas->a[i] = (TYPE)i*FACTOR;
        if(i <= 1) vas->b[i] = (TYPE)(vas->length - i)*FACTOR;
		else vas->b[i] = vas->check[i-1];        
		
		if(DIVIDE) vas->check[i] = vas->a[i]/vas->b[i];
        if(MULTIPLY) vas->check[i] = vas->a[i]*vas->b[i];
        if(ADD) vas->check[i] = vas->a[i]+vas->b[i];
        if(SUBTRACT) vas->check[i] = vas->a[i]-vas->b[i];
    }
}
