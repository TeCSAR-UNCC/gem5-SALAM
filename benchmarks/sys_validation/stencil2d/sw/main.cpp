#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

#define INP_OFFSET      0
#define SOL_OFFSET      sizeof(TYPE)*ROW*COL
#define FIL_OFFSET      2*sizeof(TYPE)*ROW*COL
#define CHK_OFFSET      2*sizeof(TYPE)*ROW*COL  + sizeof(TYPE)*FIL

stencil_struct sts;

volatile uint8_t  * top        = (uint8_t  *)TOP;
volatile uint32_t * loc_inp    = (uint32_t *)(TOP + 0x1);
volatile uint32_t * loc_sol    = (uint32_t *)(TOP + 0x9);
volatile uint32_t * loc_filter = (uint32_t *)(TOP + 0x11);

int main(void) {
    m5_reset_stats();
	uint32_t base = 0x80c00000;
	TYPE *inp     = (TYPE *)(base+INP_OFFSET);
	TYPE *sol     = (TYPE *)(base+SOL_OFFSET);
	TYPE *filter  = (TYPE *)(base+FIL_OFFSET);
	TYPE *check   = (TYPE *)(base+CHK_OFFSET);
    int i;
    volatile int count = 0;
	stage = 0;

    sts.inp = inp;
    sts.sol = sol;
    sts.filter = filter;
    sts.check = check;

    printf("Generating data\n");
    genData(&sts);
    printf("Data generated\n");

    *loc_inp    = (uint32_t)(void *)inp;
    *loc_sol    = (uint32_t)(void *)sol;
    *loc_filter = (uint32_t)(void *)filter;
    *top = 0x01;
    while (stage < 1) count++;

    printf("Job complete\n");

#ifdef CHECK
    bool fail = false;
	if(!checkData(&sts)) {
	    for (i = 0; i < ROW*COL; i++) {
	        if(sts.sol[i] != sts.check[i])
	            printf("out[%2d]=%d expected[%d]=%d\n", i, sts.sol[i], i, sts.check[i]);
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
