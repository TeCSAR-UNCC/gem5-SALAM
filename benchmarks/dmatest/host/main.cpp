#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "vadd.h"
#include "../../common/dma.h"

vadd_struct vas;

int main(void) {
	uint64_t base = 0x80c00000;
    uint64_t spm_base = 0x2f100000;

	TYPE *a = (TYPE *)(base+0);
	TYPE *b = (TYPE *)(base+sizeof(TYPE)*LENGTH);
	TYPE *c = (TYPE *)(base+2*sizeof(TYPE)*LENGTH);
	TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*LENGTH);
	TYPE *spm_a = (TYPE *)(spm_base+0);
	TYPE *spm_b = (TYPE *)(spm_base+sizeof(TYPE)*LENGTH);
	TYPE *spm_c = (TYPE *)(spm_base+2*sizeof(TYPE)*LENGTH);

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

    dmacpy(spm_a, a, sizeof(TYPE)*LENGTH);
    while(!pollDma());
    resetDma();
    dmacpy(spm_b, b, sizeof(TYPE)*LENGTH);
    while(!pollDma());
    resetDma();
#endif
    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    dmacpy(c, spm_c, sizeof(TYPE)*LENGTH);
    while(!pollDma());
#endif
    acc = 0x00;
	if(!checkData(&vas)) {
	    for (i = 0; i < LENGTH; i++) {
	        printf("C[%2d]=%f\n", i, vas.c[i]);
	    }
	}
	printf("%d", acc);
	*(char *)0x2FFFFFFF = 1;
}
