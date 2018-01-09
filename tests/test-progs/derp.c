#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define BASE 0x100000000
uint64_t volatile * const base = (uint64_t *) BASE;

int
main()
{
    printf("Doing something stupid!");
    *base = 1;
}
