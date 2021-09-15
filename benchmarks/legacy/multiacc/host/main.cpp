#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "main.h"
#include "../../common/dma.h"
#include "../../common/m5ops.h"

gemm_struct ges;
fir_struct fes;

int main(void) {
	uint64_t GEMM_BASE = 0x80C00000;
	uint64_t FIR_BASE  = 0x80D00000;
    uint64_t SPM0_BASE = 0x2F100000;
    uint64_t SPM1_BASE = 0x2F200000;

    volatile char     * gemm            = (char *)(0x2f000000);
    volatile uint64_t * val_a           = (uint64_t *)(0x2f000001);
    volatile uint64_t * val_b           = (uint64_t *)(0x2f000009);
    volatile uint64_t * val_c           = (uint64_t *)(0x2f000011);

    volatile char     * fir             = (char *)(0x2f000019);
    volatile uint64_t * loc_in          = (uint64_t *)(0x2f00001A);
    volatile uint64_t * loc_coefficient = (uint64_t *)(0x2f000022);
    volatile uint64_t * loc_previous    = (uint64_t *)(0x2f00002A);
    volatile uint64_t * loc_temp        = (uint64_t *)(0x2f000032);

	TYPE *a =     	   (TYPE *)(GEMM_BASE+0);
	TYPE *b =     	   (TYPE *)(GEMM_BASE+sizeof(TYPE)*ROW*COL);
	TYPE *c =     	   (TYPE *)(GEMM_BASE+2*sizeof(TYPE)*ROW*COL);
	TYPE *gemm_check = (TYPE *)(GEMM_BASE+3*sizeof(TYPE)*ROW*COL);
	TYPE *spm_a = 	   (TYPE *)(SPM0_BASE+0);
	TYPE *spm_b =      (TYPE *)(SPM0_BASE+sizeof(TYPE)*ROW*COL);
	TYPE *spm_c =      (TYPE *)(SPM0_BASE+2*sizeof(TYPE)*ROW*COL);

    ges.a = a;
    ges.b = b;
    ges.c = c;
    ges.row_size = ROW;
    ges.col_size = COL;

    printf("Generating GEMM data\n");
    genGemmData(&ges);
    printf("Data GEMM generated\n");

	#define IN_OFFSET       0
    #define COEF_OFFSET     (IN_OFFSET      + sizeof(int)*INPUTSIZE)
    #define PREV_OFFSET     (COEF_OFFSET    + sizeof(int)*TAPS)
    #define TEMP_OFFSET     (PREV_OFFSET    + sizeof(int)*TAPS)
    #define CHK_OFFSET      (TEMP_OFFSET    + sizeof(int)*INPUTSIZE)

	int *in                 = (int *)(FIR_BASE+IN_OFFSET);
	int *coefficient        = (int *)(FIR_BASE+COEF_OFFSET);
	int *previous           = (int *)(FIR_BASE+PREV_OFFSET);
	int *temp               = (int *)(FIR_BASE+TEMP_OFFSET);
	int *check              = (int *)(FIR_BASE+CHK_OFFSET);
	int *spm_in             = (int *)(SPM1_BASE+IN_OFFSET);
	int *spm_coefficient    = (int *)(SPM1_BASE+COEF_OFFSET);
	int *spm_previous       = (int *)(SPM1_BASE+PREV_OFFSET);
	int *spm_temp           = (int *)(SPM1_BASE+TEMP_OFFSET);

    fes.in = in;
    fes.coefficient = coefficient;
    fes.previous = previous;
    fes.temp = temp;
    fes.check = check;

    printf("Generating FIR data\n");
    genFirData(&fes);
    printf("Data FIR generated\n");

#ifndef SPM
    *val_a           = (uint64_t)(GEMM_BASE+0*ROW*COL*sizeof(TYPE));
    *val_b           = (uint64_t)(GEMM_BASE+1*ROW*COL*sizeof(TYPE));
    *val_c           = (uint64_t)(GEMM_BASE+2*ROW*COL*sizeof(TYPE));

    *loc_in          = (uint64_t)(FIR_BASE+IN_OFFSET);
    *loc_coefficient = (uint64_t)(FIR_BASE+COEF_OFFSET);
    *loc_previous    = (uint64_t)(FIR_BASE+PREV_OFFSET);
    *loc_temp        = (uint64_t)(FIR_BASE+TEMP_OFFSET);
#else
    *val_a           = (uint64_t)(SPM0_BASE+0*ROW*COL*sizeof(TYPE));
    *val_b           = (uint64_t)(SPM0_BASE+1*ROW*COL*sizeof(TYPE));
    *val_c           = (uint64_t)(SPM0_BASE+2*ROW*COL*sizeof(TYPE));

    *loc_in          = (uint64_t)(SPM1_BASE+IN_OFFSET);
    *loc_coefficient = (uint64_t)(SPM1_BASE+COEF_OFFSET);
    *loc_previous    = (uint64_t)(SPM1_BASE+PREV_OFFSET);
    *loc_temp        = (uint64_t)(SPM1_BASE+TEMP_OFFSET);

    dmacpy(spm_a, a, sizeof(TYPE)*ROW*COL);
    while(!pollDma());
    resetDma();
    dmacpy(spm_b, b, sizeof(TYPE)*ROW*COL);
    while(!pollDma());
    resetDma();
#endif
    printf("Launching GEMM:%d\n", *gemm);

    *gemm = 0x01;
    printf("GEMM Launched:%d\n", *gemm);

#ifdef SPM
    dmacpy(spm_in, in, sizeof(int)*INPUTSIZE);
    while(!pollDma());
    resetDma();
    dmacpy(spm_coefficient, coefficient, sizeof(int)*TAPS);
    while(!pollDma());
    resetDma();
    dmacpy(spm_previous, previous, sizeof(int)*TAPS);
    while(!pollDma());
    resetDma();
#endif
    printf("Launching FIR:%d\n", *fir);
    *fir = 0x01;
    printf("FIR Launched:%d\n", *fir);

    while(*fir != 0x4 || *gemm != 0x4) {
        printf("FIR:%d GEMM:%d\n", *fir, *gemm);
	}

#ifdef SPM
    dmacpy(temp, spm_temp, sizeof(int)*INPUTSIZE);
    while(!pollDma());
#endif
    *fir = 0x00;

    if (checkFirData(&fes)==0)
        printf("FIR Check Failed\n");
    else
        printf("FIR Check Passed\n");

	
#ifdef SPM
    dmacpy(c, spm_c, sizeof(TYPE)*ROW*COL);
    while(!pollDma());
#endif
    *gemm = 0x00;
    bool fail = false;
	int i, j, k, k_col, i_col;
	TYPE sum = 0;
	TYPE mult = 0;

	for(i=0;i<ROW;i++) {
        for(j=0;j<COL;j++) {
            i_col = i * COL;
            sum = 0;
            for(k=0;k<ROW;k++) {
                k_col = k * COL;
                mult = a[i_col + k] * b[k_col + j];
                sum += mult;
            }
            gemm_check[i_col + j] = sum;
        }
    }

    for(i=0; i<ROW*COL; i++) {
        printf("Expected:%f Actual:%f\n", check[i], c[i]);
        if(c[i] != gemm_check[i])
            fail = true;
    }

    if(fail)
        printf("GEMM Check Failed\n");
    else
        printf("GEMM Check Passed\n");

	m5_exit();
}
