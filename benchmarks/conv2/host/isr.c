#include <stdio.h>
#include "conv.h"
#include "relu.h"
#include "max_pool.h"
#include "combined.h"

void isr(void)
{
    uint8_t * hwacc = (uint8_t *)0x2f000000;
    hwacc[0] = 0x0;
    printf("Interrupt:\n\r");

}
