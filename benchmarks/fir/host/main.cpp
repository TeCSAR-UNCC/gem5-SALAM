#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "fir.h"

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define IN_OFFSET       0
#define COEF_OFFSET     (IN_OFFSET      + sizeof(int)*INPUTSIZE)
#define PREV_OFFSET     (COEF_OFFSET    + sizeof(int)*TAPS)
#define TEMP_OFFSET     (PREV_OFFSET    + sizeof(int)*TAPS)
#define CHK_OFFSET      (TEMP_OFFSET    + sizeof(int)*INPUTSIZE)

fir_struct fir;

int main(void) {

	int *in             = (int *)(BASE+IN_OFFSET);
	int *coefficient    = (int *)(BASE+COEF_OFFSET);
	int *previous       = (int *)(BASE+PREV_OFFSET);
	int *temp           = (int *)(BASE+TEMP_OFFSET);
	int *check          = (int *)(BASE+CHK_OFFSET);

	common_val = 0;

    fir.in          = in;
    fir.coefficient = coefficient;
    fir.previous    = previous;
    fir.temp        = temp;
    fir.check       = check;

    printf("Generating data\n");
    genData(&fir);
    printf("Data generated\n");

#ifndef SPM
    loc_in          = (uint64_t)(BASE+IN_OFFSET);
    loc_coefficient = (uint64_t)(BASE+COEF_OFFSET);
    loc_previous    = (uint64_t)(BASE+PREV_OFFSET);
    loc_temp        = (uint64_t)(BASE+TEMP_OFFSET);
#else
    loc_in          = (uint64_t)(BASE+IN_OFFSET);
    loc_coefficient = (uint64_t)(BASE+COEF_OFFSET);
    loc_previous    = (uint64_t)(BASE+PREV_OFFSET);
    loc_temp        = (uint64_t)(BASE+TEMP_OFFSET);
    
    std::memcpy((void *)(SPM_BASE+IN_OFFSET),   (void *)in,             sizeof(int)*INPUTSIZE);
    std::memcpy((void *)(SPM_BASE+COEF_OFFSET), (void *)coefficient,    sizeof(int)*TAPS);
    std::memcpy((void *)(SPM_BASE+PREV_OFFSET), (void *)previous,       sizeof(int)*TAPS);
#endif
    int i, total = 0;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void *)temp, (void *)(SPM_BASE+TEMP_OFFSET),           sizeof(int)*INPUTSIZE);
#endif
    acc = 0x00;
#ifdef CHECK
	if(!checkData(&fir)) {
        for (i = 0; i < INPUTSIZE-1; i++) {
            total += temp[i];
        }
        printf("T = %d\tC=%d\n", total, check[0]);
	}
#endif
	*(char *)0x7FFFFFFF = 1;//Kill the simulation
}
