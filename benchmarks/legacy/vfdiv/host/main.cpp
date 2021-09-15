#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "vfdiv.h"

vfdiv_struct vas;

int main(void) {
	uint64_t base = 0x80c00000;
    uint64_t spm_base = 0x2f100000;

	TYPE *a = (TYPE *)(base+0);
	TYPE *b = (TYPE *)(base+sizeof(TYPE)*LENGTH);
	TYPE *c = (TYPE *)(base+2*sizeof(TYPE)*LENGTH);
	TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*LENGTH);

	common_val = 0;
    vas.a = a;
    vas.b = b;
    vas.c = c;
    vas.check = check;
    vas.length = LENGTH;

    printf("Generating data\n");
    genData(&vas);
    printf("Data generated\n");
#ifndef SPM
    val_a = (uint64_t)base;
    val_b = (uint64_t)(base+sizeof(TYPE)*LENGTH);
    val_c = (uint64_t)(base+2*sizeof(TYPE)*LENGTH);
#else
    val_a = (uint64_t)spm_base;
    val_b = (uint64_t)(spm_base+sizeof(TYPE)*LENGTH);
    val_c = (uint64_t)(spm_base+2*sizeof(TYPE)*LENGTH);

    std::memcpy((void *)spm_base, (void *)a, sizeof(TYPE)*LENGTH);
    std::memcpy((void *)(spm_base+sizeof(TYPE)*LENGTH), (void *)b, sizeof(TYPE)*LENGTH);
#endif
    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void *)c, (void *)(spm_base+2*sizeof(TYPE)*LENGTH), sizeof(TYPE)*LENGTH);
#endif
    acc = 0x00;
	if(!checkData(&vas)) {
	    for (i = 0; i < LENGTH; i++) {
	        printf("C[%2d]=%f\n", i, vas.c[i]);
	    }
	}
	*(char *)0x2FFFFFFF = 1;
}
