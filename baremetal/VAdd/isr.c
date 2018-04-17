#include <stdio.h>
#include "vadd.h"

void isr(void)
{
	printf("Interrupt\n\r");
	common_val = 1;
}
