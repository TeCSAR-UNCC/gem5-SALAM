#include <stdio.h>

void isr(void)
{
#if 0
    //Timer BaseAddress
    int *timer1 = (int*)0x10011000;
    
    //Clear Timer Interrupt Flag
    timer1[3] = 0xff;

//	int *gicInt = (int*)0x2c001000;
    printf("***************** INTERRUPT!!! *************************\n");
//	printf("Gic Int\n");

#else
	uint32_t *rtc = (uint32_t *)0x1c170000; // RTC Base Address
	rtc[7] = 1;								//Clear Interrupt
	printf("Interrupt cleared\r\n\r");
#endif
}
