#include <stdio.h>
#include "host.h"

void head_isr(void)
{
	uint8_t  * MMR  = (uint8_t  *)(head_top);
	printf("Head Interrupt\n\r");
	*MMR = 0x00;
	printf("Head Interrupt Finished\n");
	stage += 1;
}

void body_isr(void)
{
	uint8_t  * MMR  = (uint8_t  *)(body_top);
	printf("Body Interrupt\n\r");
	*MMR = 0x00;
	printf("Body Interrupt finished\n");
	stage += 1;
}

void tail_isr(void)
{
	printf("Interrupt\n\r");
	stage += 1;
}

void class_isr(void)
{
	printf("Interrupt\n\r");
	stage += 1;
}