#include <stdio.h>
#include "host.h"
void isr(void)
{
	uint8_t  * MMR  = (uint8_t  *)(TOP);
	printf("Interrupt\n\r");
	*MMR = 0x00;
	stage += 1;
}