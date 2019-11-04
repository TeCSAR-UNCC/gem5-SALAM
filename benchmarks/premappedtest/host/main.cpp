#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "main.h"
#include "../../common/m5ops.h"
//#include "../../common/dma.h"

vadd_struct vas;

extern "C" int dmacpy(void*, void*, int);
extern "C" int pollDma();
extern "C" int resetDma();

int main(void) {
	uint64_t base = 0x80c00000;
    uint64_t spm_base = 0x2f100000;
    uint8_t * acc = (uint8_t *)0x2f000000;
    //int junk = 0;

	TYPE *a = (TYPE *)(base+0);
	TYPE *b = (TYPE *)(base+sizeof(TYPE)*LENGTH);
	TYPE *c = (TYPE *)(base+2*sizeof(TYPE)*LENGTH);
	TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*LENGTH);
	TYPE *spm_a = (TYPE *)(spm_base+0);
	TYPE *spm_b = (TYPE *)(spm_base+sizeof(TYPE)*LENGTH);
	TYPE *spm_c = (TYPE *)(spm_base+2*sizeof(TYPE)*LENGTH);

    vas.a = a;
    vas.b = b;
    vas.c = c;
    vas.check = check;
    vas.length = LENGTH;

    printf("Generating data\n");
    genData(&vas);
    printf("Data generated\n");

    dma_signal = 0;
    printf("Starting DMA Xfer\n");
    dmacpy(spm_a, a, sizeof(TYPE)*LENGTH);
    dmacpy(spm_b, b, sizeof(TYPE)*LENGTH);
    while(dma_signal==0) {  }

    dev_signal = 0;
    printf("Launching Accelerator\n");
    *acc = 0x01;
    printf("%d\n", *acc);

	while(dev_signal==0) {  }

    printf("Reading data from device\n");
    dma_signal = 0;
    dmacpy(c, spm_c, sizeof(TYPE)*LENGTH);
    while(dma_signal==0) {  }

	if(!checkData(&vas)) {
        int i;
	    for (i = 0; i < LENGTH; i++) {
	        printf("C[%2d]=%f\n", i, vas.c[i]);
	    }
	}
	m5_exit();
}
