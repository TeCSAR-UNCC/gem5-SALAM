#include "defines.h"
#include "data.h"
#include <cmath>

#define EPSILON 1.0e-2
#define blockSide3 blockSide*blockSide*blockSide
#define blockSide2 blockSide*blockSide

typedef struct {
    int32_t * n_points;
    TYPE * force;
    TYPE * position;
    TYPE * check;
//    int blockSide;
//    int densityFactor;
} md_struct;

int checkData(md_struct * mds) {
    int i;
	double result;
    for (int i = 0; i < blockSide3*densityFactor*3; i++) {
    	result = std::abs((mds->force[i]-mds->check[i]))/(mds->check[i]);
        if (result > EPSILON) {
            printf("Check Failed\n");
            return 0;
        }
    }
    printf("Check Passed\n");
    return 1;
}

void genData(md_struct * mds) {
    int i;
#ifndef TEST
    for (i = 0; i < blockSide3; i++) {
        mds->n_points[i] = n_p[i];
    }
    for(i = 0; i < blockSide3*densityFactor; i++) {
        mds->position[3*i] = pos[3*i];
        mds->position[3*i+1] = pos[3*i+1];
        mds->position[3*i+2] = pos[3*i+2];
        mds->check[3*i] = chk[3*i];
        mds->check[3*i+1] = chk[3*i+1];
        mds->check[3*i+2] = chk[3*i+2];
        mds->force[3*i] = 0.0;
        mds->force[3*i+1] = 0.0;
        mds->force[3*i+2] = 0.0;
    }
#else
    for (i = 0; i < blockSide3; i++) {
        mds->n_points[i] = 1;
    }
    for(i = 0; i < blockSide3*densityFactor; i++) {
        mds->position[3*i] = 3.0*i;
        mds->position[3*i+1] = 3.0*i;
        mds->position[3*i+2] = 3.0*i;
        mds->check[3*i] = chk[3*i];
        mds->check[3*i+1] = chk[3*i+1];
        mds->check[3*i+2] = chk[3*i+2];
        mds->force[3*i] = 0.0;
        mds->force[3*i+1] = 0.0;
        mds->force[3*i+2] = 0.0;
    }
#endif
}

