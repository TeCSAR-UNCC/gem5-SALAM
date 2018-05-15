#include <stdio.h>
#include "stencil.h"

void isr(void)
{
	printf("Interrupt\n\r");
	common_val = 1;
}
