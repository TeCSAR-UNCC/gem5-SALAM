#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "hotspot.h"

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define RESULT_OFFSET   0
#define TEMP_OFFSET     (RESULT_OFFSET  + sizeof(double)*GRID_COLS*GRID_ROWS)
#define POWER_OFFSET    (TEMP_OFFSET    + sizeof(double)*GRID_COLS*GRID_ROWS)
#define CHK_OFFSET      (POWER_OFFSET   + sizeof(double)*GRID_COLS*GRID_ROWS)

hotspot_struct hss;

int main(void) {

	double * result     = (double *)(BASE+RESULT_OFFSET);
	double * temp       = (double *)(BASE+TEMP_OFFSET);
	double * power      = (double *)(BASE+POWER_OFFSET);
	double * check      = (double *)(BASE+CHK_OFFSET);

	common_val = 0;

    hss.result  = result;
    hss.temp    = temp;
    hss.power   = power;
    hss.check   = check;

    printf("Generating data\n");
    genData(&hss);
    printf("Data generated\n");

#ifndef SPM
    loc_result      = (uint64_t)(BASE+RESULT_OFFSET);
    loc_temp        = (uint64_t)(BASE+TEMP_OFFSET);
    loc_power       = (uint64_t)(BASE+POWER_OFFSET);
#else
    loc_result      = (uint64_t)(SPM_BASE+RESULT_OFFSET);
    loc_temp        = (uint64_t)(SPM_BASE+TEMP_OFFSET);
    loc_power       = (uint64_t)(SPM_BASE+POWER_OFFSET);
    
    std::memcpy((void *)(SPM_BASE+TEMP_OFFSET),     (void *)temp,   sizeof(double)*GRID_COLS*GRID_ROWS);
    std::memcpy((void *)(SPM_BASE+POWER_OFFSET),    (void *)power,  sizeof(double)*GRID_COLS*GRID_ROWS);
#endif
    int i, total = 0;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void *)result, (void *)(SPM_BASE+RESULT_OFFSET),   sizeof(double)*GRID_COLS*GRID_ROWS);
#endif
    acc = 0x00;
#ifdef CHECK
	if(!checkData(&hss)) {
	
	    printf("failed");
        for (i = 0; i < GRID_COLS*GRID_ROWS; i++) {
            //printf("Check[%d]=%lf\n", i, result[i]);
            double error = (result[i]-check[i])/result[i]*100;
            printf("Error[%d] %f\n", i, error);
        }
        printf("failed");
	}
#endif
	*(char *)0x7FFFFFFF = 1;//Kill the simulation
}
