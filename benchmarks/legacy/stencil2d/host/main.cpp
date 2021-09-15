#include <cstdio>
#include <cstring>
#include "stencil.h"

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define INP_OFFSET      0
#define SOL_OFFSET      sizeof(TYPE)*ROW*COL
#define FIL_OFFSET      2*sizeof(TYPE)*ROW*COL
#define CHK_OFFSET      2*sizeof(TYPE)*ROW*COL  + sizeof(TYPE)*FIL

stencil_struct sts;

int main(void) {
	//uint64_t base =     0x80c00000;
	TYPE *inp          = (TYPE *)(BASE+INP_OFFSET);
	TYPE *sol          = (TYPE *)(BASE+SOL_OFFSET);
	TYPE *filter       = (TYPE *)(BASE+FIL_OFFSET);
	TYPE *check        = (TYPE *)(BASE+CHK_OFFSET);
    int i;
	//int *check;
//	int row_size = ROW;
//    int col_size = COL;
//    int f_size = FIL;


	common_val = 0;

    sts.inp = inp;
    sts.sol = sol;
    sts.filter = filter;
    sts.check = check;

    printf("Generating data\n");
    genData(&sts);
    printf("Data generated\n");
    
#ifndef SPM
    loc_inp     = (uint64_t)(BASE+INP_OFFSET);
    loc_sol     = (uint64_t)(BASE+SOL_OFFSET);
    loc_filter  = (uint64_t)(BASE+FIL_OFFSET);
#else
    loc_inp     = (uint64_t)(SPM_BASE+INP_OFFSET);
    loc_sol     = (uint64_t)(SPM_BASE+SOL_OFFSET);
    loc_filter  = (uint64_t)(SPM_BASE+FIL_OFFSET);

    std::memcpy((void *)(SPM_BASE+INP_OFFSET), (void *)inp,       sizeof(TYPE)*ROW*COL);
//    std::memcpy((void *)(SPM_BASE+SOL_OFFSET), (void *)sol,     sizeof(TYPE)*ROW*COL);
    std::memcpy((void *)(SPM_BASE+FIL_OFFSET), (void *)filter,    sizeof(TYPE)*FIL);
#endif

    printf("%d\n", acc);
    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
	
#ifdef SPM
    std::memcpy((void *)sol, (void *)(SPM_BASE+SOL_OFFSET), sizeof(TYPE)*ROW*COL);
#endif
    acc = 0x00;
#ifdef CHECK
	if(!checkData(&sts)) {
	    for (i = 0; i < ROW*COL; i++) {
	        if(sts.sol[i] != sts.check[i])
	            printf("out[%2d]=%d expected[%d]=%d\n", i, sts.sol[i], i, sts.check[i]);
	    }
	}
#endif
	*(char *)0x7fffffff = 1; //Kill the simulation
}
