#include <stdio.h>

void isr(void)
{
    //Timer BaseAddress
    int *timer1 = (int*)0x10011000;
    //Clear Timer Interrupt Flag
    timer1[3] = 0xff;

    printf("***************** INTERRUPT!!! *************************\n");
}
