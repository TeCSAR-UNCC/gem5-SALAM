#include <cstdio>
#include <cstring>
#include "stencil.h"

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define C_OFFSET        0
#define INP_OFFSET      sizeof(TYPE)*C_SIZE
#define SOL_OFFSET      sizeof(TYPE)*C_SIZE + sizeof(TYPE)*SIZE
#define CHK_OFFSET      sizeof(TYPE)*C_SIZE + 2*sizeof(TYPE)*SIZE

stencil_struct sts;

int main(void) {
    TYPE *C         = (TYPE *)(BASE+C_OFFSET);
	TYPE *inp       = (TYPE *)(BASE+INP_OFFSET);
	TYPE *sol       = (TYPE *)(BASE+SOL_OFFSET);
	TYPE *check     = (TYPE *)(BASE+CHK_OFFSET);
	int i;
//	int row_size    = 16;
//    int col_size    = 32;
//    int height_size = 32;
//    int C_size = 2;

	common_val = 0;

    sts.C = C;
    sts.inp = inp;
    sts.sol = sol;
    sts.check = check;

    printf("Generating data\n");
    genData(&sts);
    printf("Data generated\n");
    
#ifndef SPM
    loc_C       = (uint64_t)(BASE+C_OFFSET);
    loc_inp     = (uint64_t)(BASE+INP_OFFSET);
    loc_sol     = (uint64_t)(BASE+SOL_OFFSET);

#else
    loc_C       = (uint64_t)(SPM_BASE+C_OFFSET);
    loc_inp     = (uint64_t)(SPM_BASE+INP_OFFSET);
    loc_sol     = (uint64_t)(SPM_BASE+SOL_OFFSET);

    std::memcpy((void *)(SPM_BASE+C_OFFSET),    (void *)C,      sizeof(TYPE)*C_SIZE);
    std::memcpy((void *)(SPM_BASE+INP_OFFSET),  (void *)inp,    sizeof(TYPE)*SIZE);
//    std::memcpy((void *)(SPM_BASE+SOL_OFFSET), (void *)sol,     sizeof(TYPE)*ROW*COL);
    
#endif

    printf("%d\n", acc);
    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
	
#ifdef SPM
    std::memcpy((void *)((void *)sol,   SPM_BASE+SOL_OFFSET), sizeof(TYPE)*SIZE);
#endif
    acc = 0x00;
#ifdef CHECK
	if(!checkData(&sts)) {
	    for (i = 0; i < SIZE; i++) {
	        if(sts.sol[i] != sts.check[i])
	            printf("out[%2d]=%d expected[%d]=%d\n", i, sts.sol[i], i, sts.check[i]);
	    }
	}
#endif
	*(char *)0x7fffffff = 1; //Kill the simulation
}
