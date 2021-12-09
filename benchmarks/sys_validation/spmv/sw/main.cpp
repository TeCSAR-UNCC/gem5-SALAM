#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "spmv.h"
#include "../../../common/m5ops.h"

spmv_struct sps;

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#ifdef TEST
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

volatile uint8_t  * acc      = (uint8_t  *)(TOP);
volatile uint32_t * loc_val  = (uint32_t *)(TOP+1);
volatile uint32_t * loc_cols = (uint32_t *)(TOP+9);
volatile uint32_t * loc_rows = (uint32_t *)(TOP+17);
volatile uint32_t * loc_vec  = (uint32_t *)(TOP+25);
volatile uint32_t * loc_out  = (uint32_t *)(TOP+33);

int main(void) {
	TYPE *val          = (TYPE *)(BASE+VAL_OFFSET);
	int *cols          = (int  *)(BASE+COLS_OFFSET);
	int *rowDelimiters = (int  *)(BASE+ROWD_OFFSET);
	TYPE *vec          = (TYPE *)(BASE+VEC_OFFSET);
	TYPE *out          = (TYPE *)(BASE+OUT_OFFSET);
	TYPE *check        = (TYPE *)(BASE+CHK_OFFSET);
    volatile int count = 0;

    sps.val = val;
    sps.cols = cols;
    sps.rowDelimiters = rowDelimiters;
    sps.vec = vec;
    sps.out = out;
    sps.check = check;

    printf("Generating data\n");
    genData(&sps);
    printf("Data generated\n");

    *loc_val  = (uint64_t)(BASE+VAL_OFFSET);
    *loc_cols = (uint64_t)(BASE+COLS_OFFSET);
    *loc_rows = (uint64_t)(BASE+ROWD_OFFSET);
    *loc_vec  = (uint64_t)(BASE+VEC_OFFSET);
    *loc_out  = (uint64_t)(BASE+OUT_OFFSET);

    *acc = DEV_INIT;
	while(*acc != DEV_INTR) count++;
    printf("Job complete\n");
#ifdef CHECK
	if(!checkData(&sps)) {
	    for (int i = 0; i < N; i++) {
	        if(((sps.out[i]-sps.check[i]) > EPSILON) || ((sps.out[i]-sps.check[i]) < -EPSILON))
	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, sps.out[i], i, sps.check[i]);
	    }
	}
#endif
	m5_dump_stats();
  	m5_exit();
}
