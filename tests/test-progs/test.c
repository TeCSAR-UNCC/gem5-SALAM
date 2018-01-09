#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>

#define BASE 0x400000FF
uint32_t volatile * const base = (uint32_t *) BASE;

int
main()
{
    int value = 1;
    
    printf("Write to memmapped IO");
    *base = value;
    printf("Read from memmapped IO");
    value = *base;
}
