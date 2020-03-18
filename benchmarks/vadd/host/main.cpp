#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "vadd.h"
#include "../../common/dma.h"
#include "../../common/m5ops.h"

vadd_struct vas;

int main(void) {
	m5_reset_stats();

	TYPE *a = (TYPE *)(0x2f100001);
	TYPE *b = (TYPE *)(0x2f100041);
	TYPE *c = (TYPE *)(0x2f100081);
	TYPE *check = (TYPE *)(0x2f110001);
	
	TYPE *spma = (TYPE *)(0x2f100001);
	TYPE *spmb = (TYPE *)(0x2f100041);
	TYPE *spmc = (TYPE *)(0x2f100081);

	common_val = 0;
    vas.a = a;
    vas.b = b;
    vas.c = c;
    vas.check = check;
    vas.length = LENGTH;

    printf("Generating data\n");
    genData(&vas);
    printf("Data generated\n");

    val_a = (uint64_t)(0x2f100001);
    val_b = (uint64_t)(0x2f100041);
    val_c = (uint64_t)(0x2f100081);
    
    
    dmacpy(spma, a, sizeof(TYPE));
    while(!pollDma());
    resetDma();
    dmacpy(spmb, b, sizeof(TYPE));
    while(!pollDma());
    resetDma();
    dmacpy(spmc, c, sizeof(TYPE)*16);
    while(!pollDma());
    resetDma();  
    

    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}

    dmacpy(b, spmb, sizeof(TYPE));
    while(!pollDma());

    acc = 0x00;
	if(!checkData(&vas)) {
	    for (i = 0; i < LENGTH; i++) {
	        printf("C[%2d]=%f\n", i, vas.c[i]);
	    }
	}
	m5_dump_stats();	
	m5_exit();
}
