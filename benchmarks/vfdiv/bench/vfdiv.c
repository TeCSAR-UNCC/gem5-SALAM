#include "../defines.h"

void vfdiv(TYPE* a, TYPE* b, TYPE* c) {
    int i;
    #pragma clang loop unroll(disable)
    for (i = 0; i < LENGTH; i++) {
       if(DIVIDE) c[i] = a[i] / b[i];
	   if(MULTIPLY) c[i] = a[i] * b[i];
	   if(ADD) c[i] = a[i] + b[i];
	   if(SUBTRACT) c[i] = a[i] - b[i];
    }
}
