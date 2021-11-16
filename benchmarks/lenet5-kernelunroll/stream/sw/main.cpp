#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../../common/m5ops.h"
#include "bench.h"
#include "../lenet5_clstr_hw_defines.h"

volatile uint8_t  * top   = (uint8_t  *)TOP;

int main(void) {
	m5_reset_stats();
    volatile int count = 0;
	stage = 0;

    // uint32_t base = 0x80c00000;
    // TYPE *mainMem = (TYPE *)base;
    // *mainMem = (uint32_t)(void *)mainMem;

    *top = 0x01;
    while (stage < 1) count++;

    // printf("Job complete\n");
	m5_dump_stats();
	m5_exit();
}
