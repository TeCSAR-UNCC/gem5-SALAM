#include "../defines.h"
#include "data.h"
#include <cmath>

#define common_val      *(unsigned *)0x80b00000
#define acc             *(char *)0x2f000000
#define loc_force_x     *(int *)0x2f000001
#define loc_force_y     *(int *)0x2f000009
#define loc_force_z     *(int *)0x2f000011
#define loc_position_x  *(int *)0x2f000019
#define loc_position_y  *(int *)0x2f000021
#define loc_position_z  *(int *)0x2f000029
#define loc_NL          *(int *)0x2f000031

#define EPSILON 1.0e-2

typedef struct {
    TYPE * force_x;
    TYPE * force_y;
    TYPE * force_z;
    TYPE * position_x;
    TYPE * position_y;
    TYPE * position_z;
    int32_t * NL;
    TYPE * check_x;
    TYPE * check_y;
    TYPE * check_z;
} md_struct;

int checkData(md_struct * mds) {
    int i;
	/*
	for (int i = 0; i < nAtoms; i++) {
		printf("i:%d \n X:%f:%f \n Y:%f:%f \n Z:%f:%f \n", i,mds->force_x[i], mds->check_x[i], mds->force_y[i], mds->check_y[i], mds->force_z[i], mds->check_z[i]); 
	}
	return 0;
	*/
	
    for (int i = 0; i < nAtoms; i++) {
        if (    (std::abs(mds->force_x[i]-mds->check_x[i])/mds->check_x[i] > EPSILON) || 
                (std::abs(mds->force_y[i]-mds->check_y[i])/mds->check_y[i] > EPSILON) || 
                (std::abs(mds->force_z[i]-mds->check_z[i])/mds->check_z[i] > EPSILON)) {
            printf("Check Failed\n");
            return 0;
        }
    }
    printf("Check Passed\n");
    return 1;
    
}

void genData(md_struct * mds) {
    int i;
    for(i = 0; i < nAtoms; i++) {
        mds->check_x[i]     = c_x[i];
        mds->check_y[i]     = c_y[i];
        mds->check_z[i]     = c_z[i];
        mds->position_x[i]  = p_x[i];
        mds->position_y[i]  = p_y[i];
        mds->position_z[i]  = p_z[i];
    }
    for(i = 0; i < nAtoms*maxNeighbors; i++) {
        mds->NL[i] = n_l[i];
    }
}
