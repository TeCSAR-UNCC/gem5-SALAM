#include <stdio.h>
#include "host.h"

void head_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(head_top);
	printf("Head Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}

void body_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(body_top);
	printf("Body Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}

void tail_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(tail_top);
	printf("Tail Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}

void class_isr(void)
{
	volatile uint8_t  * MMR  = (uint8_t  *)(class_top);
	printf("Classifier Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}