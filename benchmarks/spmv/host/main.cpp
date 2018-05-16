#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "spmv.h"

spmv_struct sps;

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#ifndef TEST
//Ensure 8 byte alignment
#define VAL_OFFSET      0
#define COLS_OFFSET     8*NNZ
#define ROWD_OFFSET     8*NNZ + 4*NNZ
#define VEC_OFFSET      8*NNZ + 4*NNZ + 4*(N+2)
#define OUT_OFFSET      8*NNZ + 4*NNZ + 4*(N+2) + 8*N
#define CHK_OFFSET      8*NNZ + 4*NNZ + 4*(N+2) + 8*N + 8*N

#else
//Ensure 8 byte alignment
#define VAL_OFFSET      0
#define COLS_OFFSET     8*NNZ
#define ROWD_OFFSET     8*NNZ + 4*(NNZ+1)
#define VEC_OFFSET      8*NNZ + 4*(NNZ+1) + 4*(N+1)
#define OUT_OFFSET      8*NNZ + 4*(NNZ+1) + 4*(N+1) + 8*(N+1)
#define CHK_OFFSET      8*NNZ + 4*(NNZ+1) + 4*(N+1) + 8*(N+1) + 8*(N+1)

#endif

int main(void) {
	TYPE *val          = (TYPE *)(BASE+VAL_OFFSET);
	int *cols          = (int  *)(BASE+COLS_OFFSET);
	int *rowDelimiters = (int  *)(BASE+ROWD_OFFSET);
	TYPE *vec          = (TYPE *)(BASE+VEC_OFFSET);
	TYPE *out          = (TYPE *)(BASE+OUT_OFFSET);
	TYPE *check        = (TYPE *)(BASE+CHK_OFFSET);

	common_val = 0;
    sps.val = val;
    sps.cols = cols;
    sps.rowDelimiters = rowDelimiters;
    sps.vec = vec;
    sps.out = out;
    sps.check = check;

    printf("Generating data\n");
    genData(&sps);
    printf("Data generated\n");
#ifndef SPM
    loc_val  = (uint64_t)(BASE+VAL_OFFSET);
    loc_cols = (uint64_t)(BASE+COLS_OFFSET);
    loc_rows = (uint64_t)(BASE+ROWD_OFFSET);
    loc_vec  = (uint64_t)(BASE+VEC_OFFSET);
    loc_out  = (uint64_t)(BASE+OUT_OFFSET);
#else
    loc_val  = (uint64_t)(SPM_BASE+VAL_OFFSET);
    loc_cols = (uint64_t)(SPM_BASE+COLS_OFFSET);
    loc_rows = (uint64_t)(SPM_BASE+ROWD_OFFSET);
    loc_vec  = (uint64_t)(SPM_BASE+VEC_OFFSET);
    loc_out  = (uint64_t)(SPM_BASE+OUT_OFFSET);

    std::memcpy((void *)(SPM_BASE+VAL_OFFSET),  (void *)val,           sizeof(TYPE)*NNZ);
    std::memcpy((void *)(SPM_BASE+COLS_OFFSET), (void *)cols,          sizeof(int)*NNZ);
    std::memcpy((void *)(SPM_BASE+ROWD_OFFSET), (void *)rowDelimiters, sizeof(int)*(N+1));
    std::memcpy((void *)(SPM_BASE+VEC_OFFSET),  (void *)vec,           sizeof(TYPE)*N);
#endif
    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void *)out, (void *)(SPM_BASE+OUT_OFFSET), sizeof(TYPE)*N);
#endif
    acc = 0x00;
	if(!checkData(&sps)) {
	    for (i = 0; i < N; i++) {
	        if(((sps.out[i]-sps.check[i]) > EPSILON) || ((sps.out[i]-sps.check[i]) < -EPSILON))
	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, sps.out[i], i, sps.check[i]);
	    }
	}
	*(char *)0x7fffffff = 1; //Kill the simulation
}
