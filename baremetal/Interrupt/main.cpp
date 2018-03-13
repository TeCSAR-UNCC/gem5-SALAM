/*
 * Copyright (c) 2015, University of Kaiserslautern
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Matthias Jung
            Frederik Lauer
 */

#include <cstdio>

int main(void)
{
    /* This example configures a timer instance to trigger a
     * Interrupt every few seconds.
     * The Interrupt release the isr function in the isr.c
     * file.
     */
#if 0
    unsigned int delay = 0;
    unsigned int counter = 0;

//    int *timer1 = (int*)0x10011000; // Timer Base Address
//	unsigned int *timer1 = (unsigned int*)0x2B040000; // system Timer Base Address
//    *timer1 = 0x1000; // Timer StartValue
	unsigned int *hdlcd = (unsigned int *)0x2B000000; // Base Address
	

	printf("Version is %u", *hdlcd);
//    timer1[10] = 0x1000; // Timer StartValue

    printf("##################################################\n");
    printf("#################      Boot     ##################\n");
    printf("##################################################\n");
    printf("                                                  \n");

    while (1) {
        if (counter > 300000) {
            counter = 0;
            delay++;
            if (delay == 5) {
                printf("Start Timer Interrupts\n");
//                timer1[2] = 0xE0;   // Timer Start
//                timer1[11] = 0b01;   // Timer Start
            }
            printf("Delay: %u\n",delay);
        } else {
            counter++;
        }
    }

	uint8_t *kmio = (uint8_t *)0x1c060000; // Base Address
	
	*kmio = 0x1c;
	
	while(1)
	{
		if((kmio[4] & 0x08) == 0x08)
			while((kmio[4] & 0x08) == 0x08);
			printf("Data read %u\r\n", kmio[8]);
	}

	uint8_t *uart0 = (uint8_t *)0x1c090000; // Base Address
	
	uart0[0x30] = 0x81;
	
	while(1) {
		}
#else		
//	uint8_t *rtc = (uint8_t *)0x1c170000; // Base Address
	uint32_t counter = 0, reg_Val;
	uint32_t *rtc = (uint32_t *)0x1c170000; // Base Address
/*	
	rtc[0x0c] = 0x01;
	printf("Raw Interrupt status = %d\r\n", rtc[0x14]);
	printf("Masked Interrupt status = %d\r\n", rtc[0x18]);
	rtc[0x10] = 0x01;

	printf("Raw Interrupt status = %d\r\n", rtc[0x14]);
	printf("Masked Interrupt status = %d\r\n", rtc[0x18]);
*/
	rtc[3] = 0x01;			//RTC Enable
	rtc[4] = 0x01;			//RTC INT Enable
//	*(rtc + 3) = 0x01;
	//printf("Raw Interrupt status = %d\r\n", rtc[5]);
	reg_Val = rtc[5];
	printf("Counter Value is %d\r\n", reg_Val);

	while(1) {
//		printf("Masked Interrupt status = %d\r\n", rtc[0x18]);
//	printf("Raw Interrupt status = %d\r\n", rtc[5]);
	//printf("Masked Interrupt status = %d\r\n", rtc[6]);
	//for(counter = 0; counter<10; counter++)
	//reg_Val = rtc[6];
	rtc[0] = 321;
	printf("Data Register: %d\r\n", rtc[6]);
		}
#endif
}
