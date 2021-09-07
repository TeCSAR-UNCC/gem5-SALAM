#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

#define C_OFFSET        0
#define INP_OFFSET      sizeof(TYPE)*C_SIZE
#define SOL_OFFSET      sizeof(TYPE)*C_SIZE + sizeof(TYPE)*SIZE
#define CHK_OFFSET      sizeof(TYPE)*C_SIZE + 2*sizeof(TYPE)*SIZE

stencil_struct sts;

volatile uint8_t  * top     = (uint8_t  *)(TOP + 0x00);
volatile uint32_t * loc_C   = (uint32_t *)(TOP + 0x01);
volatile uint32_t * loc_inp = (uint32_t *)(TOP + 0x09);
volatile uint32_t * loc_sol = (uint32_t *)(TOP + 0x11);

int main(void) {
    m5_reset_stats();
    uint32_t BASE = 0x80c00000;
    TYPE *C       = (TYPE *)(BASE+C_OFFSET);
	TYPE *inp     = (TYPE *)(BASE+INP_OFFSET);
	TYPE *sol     = (TYPE *)(BASE+SOL_OFFSET);
	TYPE *check   = (TYPE *)(BASE+CHK_OFFSET);
	int i;
    volatile int count = 0;
	stage = 0;

    sts.C = C;
    sts.inp = inp;
    sts.sol = sol;
    sts.check = check;

    printf("Generating data\n");
    genData(&sts);
    printf("Data generated\n");

    *loc_C   = (uint32_t)(void *)C;
    *loc_inp = (uint32_t)(void *)inp;
    *loc_sol = (uint32_t)(void *)sol;
    *top = 0x01;
    while (stage < 1) count++;

    printf("Job complete\n");

#ifdef CHECK
    bool fail = false;
	if(!checkData(&sts)) {
	    for (i = 0; i < SIZE; i++) {
	        if(sts.sol[i] != sts.check[i]) {
	            printf("out[%2d]=%d expected[%d]=%d\n", i, sts.sol[i], i, sts.check[i]);
                fail = true;
            }
	    }
	}
    if(fail)
        printf("Check Failed\n");
    else
        printf("Check Passed\n");
#endif
	m5_dump_stats();
    m5_exit();
}
