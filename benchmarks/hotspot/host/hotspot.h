#ifndef __DEFINES_H__
#include "../defines.h"
#endif
#include "data.h"
#include <cmath>

#define common_val      *(unsigned *)0x80b00000
#define acc             *(char *)0x2f000000
#define loc_result      *(int *)0x2f000001
#define loc_temp        *(int *)0x2f000009
#define loc_power       *(int *)0x2f000011

typedef struct {
    double * result;
    double * temp;
    double * power; 
    double * check;
} hotspot_struct;

int checkData(hotspot_struct * hss) {
    int i;
    double error;

    for (i = 0; i < GRID_COLS*GRID_ROWS; i++) {
		error = (hss->result[i]-hss->check[i])/hss->result[i]*100;
		// printf("Error[%d] = %f%\n",i, error);
        if(error >= 0.1) {
            printf("Check Failed\n");
            return 0;
        }
    }
    
    printf("Check Passed\n");
    return 1;
}

void genData(hotspot_struct * hss) {
    int i;
    
    for (i = 0; i < GRID_COLS*GRID_ROWS; i++) {
        hss->temp[i]    = temp_in[i];
        hss->power[i]   = power_in[i];
        hss->check[i]   = check_in[i];
    }
}
