#include "../defines.h"

void vadd(TYPE* a, TYPE* b, TYPE* c) {
    int i;
    #pragma clang loop unroll(disable)
    for (i = 0; i < LENGTH; i++) {
        *c = *a + *b;
    }
}
