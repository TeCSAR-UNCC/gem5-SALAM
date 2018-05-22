#include <stdio.h>
#include "md.h"

void isr(void)
{
	printf("Interrupt\n\r");
	common_val = 1;
}
