#include <stdio.h>

void isr(void)
{

	uint32_t *rtc = (uint32_t *)0x1c170000; // RTC Base Address
	rtc[7] = 1;								//Clear Interrupt
	printf("Interrupt\n\r");
}
