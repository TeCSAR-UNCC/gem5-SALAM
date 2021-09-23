#include <stdio.h>
#include "host.h"

void head_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(head_top);
	volatile uint64_t * ARGS = (uint64_t *)(head_top+1);
	printf("Head Interrupt\n\r");
	*MMR = 0x00;
    ARGS[0] = 0;
    ARGS[1] = 0;
    ARGS[2] = 0;
    ARGS[3] = 0;
    ARGS[4] = 0;
    ARGS[5] = 0;
    ARGS[6] = 0;
    ARGS[7] = 0;
	stage += 1;
}

void body_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(body_top);
	volatile uint64_t * ARGS = (uint64_t *)(body_top+1);
	printf("Body Interrupt\n\r");
	*MMR = 0x00;
    ARGS[0] = 0;
    ARGS[1] = 0;
    ARGS[2] = 0;
    ARGS[3] = 0;
    ARGS[4] = 0;
    ARGS[5] = 0;
    ARGS[6] = 0;
    ARGS[7] = 0;
    ARGS[8] = 0;
    ARGS[9]  = 0;
	stage += 1;
}

void tail_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(tail_top);
	volatile uint64_t * ARGS = (uint64_t *)(tail_top+1);
	printf("Tail Interrupt\n\r");
	*MMR = 0x00;
    ARGS[0] = 0;
    ARGS[1] = 0;
    ARGS[2] = 0;
    ARGS[3] = 0;
	stage += 1;
}

void class_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(class_top);
	volatile uint64_t * ARGS = (uint64_t *)(class_top+1);
	printf("Classifier Interrupt\n\r");
	*MMR = 0x00;
    ARGS[0] = 0;
    ARGS[1] = 0;
    ARGS[2] = 0;
    ARGS[3] = 0;
	stage += 1;
}