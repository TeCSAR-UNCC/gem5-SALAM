#include <stdio.h>
#include "host.h"

void head_isr(void)
{
	uint8_t  * MMR  = (uint8_t  *)(TOP);
	printf("Head Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}

void body_isr(void)
{
	uint8_t  * MMR  = (uint8_t  *)(TOP);
	printf("Body Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}

void tail_isr(void)
{
	uint8_t  * MMR  = (uint8_t  *)(TOP);
	printf("Tail Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}

void class_isr(void)
{
	uint8_t  * MMR  = (uint8_t  *)(TOP);
	printf("Classifier Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}