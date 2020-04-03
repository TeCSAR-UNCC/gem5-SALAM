#include "../defines.h"
#include "data.h"

#define EPSILON 1.0e-2

volatile int stage;

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