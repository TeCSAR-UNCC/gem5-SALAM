#include <stdio.h>
//#include "main.h"

extern char* FLAGS;
extern uint8_t* acc;
extern int dma_signal;
extern int dev_signal;
extern void hostStep(int val);
extern int launchCpy();


void dev_isr(void)
{
    printf("Dev Interrupt\n");
    uint8_t * ACC = (uint8_t *)0x2f000000;
    *ACC &= 0xFB;
	dev_signal = 1;
}

void dma_isr(void)
{
    printf("DMA Interrupt\n");
    *FLAGS &= 0xFB;
    int ret_val = launchCpy();
    dma_signal = ret_val;
}
