#include <stdio.h>
#include "host.h"

void head_isr(void)
{
	printf("Interrupt\n\r");
	stage += 1;
}

void body_isr(void)
{
	printf("Interrupt\n\r");
	stage += 1;
}

void tail_isr(void)
{
	printf("Interrupt\n\r");
	stage += 1;
}

void class_isr(void)
{
	printf("Interrupt\n\r");
	stage += 1;
}