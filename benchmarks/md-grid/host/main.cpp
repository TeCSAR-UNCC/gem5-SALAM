#include <cstdio>
#include "md.h"

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define NP_OFFSET       0
#define FRC_OFFSET      (NP_OFFSET  + sizeof(dvector_t)*3*blockSide)
#define POS_OFFSET      (FRC_OFFSET + sizeof(dvector_t)*(3*blockSide+densityFactor))
#define CHK_OFFSET      (POS_OFFSET + sizeof(dvector_t)*(3*blockSide+densityFactor))

md_struct mds;

int main(void) {
	int32_t ***n_points    = (int32_t       ***)(BASE+NP_OFFSET);
	dvector_t ****force    = (dvector_t    ****)(BASE+FRC_OFFSET);
	dvector_t ****position = (dvector_t    ****)(BASE+POS_OFFSET);
	dvector_t ****check    = (dvector_t    ****)(BASE+CHK_OFFSET);
    int i, j, k, d;

	common_val = 0;

    mds.n_points = n_points;
    mds.force = force;
    mds.position = position;
    mds.check = check;

    printf("Generating data\n");
    genData(&mds);
    printf("Data generated\n");

#ifndef SPM
    loc_n_points    = (uint64_t)(BASE+NP_OFFSET);
    loc_force       = (uint64_t)(BASE+FRC_OFFSET);
    loc_position    = (uint64_t)(BASE+POS_OFFSET);
#else
    loc_n_points    = (uint64_t)(SPM_BASE+NP_OFFSET);
    loc_force       = (uint64_t)(SPM_BASE+FRC_OFFSET);
    loc_position    = (uint64_t)(SPM_BASE+POS_OFFSET);

    std::memcpy((void *)(SPM_BASE+NP_OFFSET), (void ***)n_points,      sizeof(dvector_t)*3*blockSide);
//    std::memcpy((void *)(SPM_BASE+SOL_OFFSET), (void *)sol,     sizeof(TYPE)*ROW*COL);
    std::memcpy((void *)(SPM_BASE+POS_OFFSET), (void ****)position,  sizeof(dvector_t)*3*(blockSide+densityFactor));
#endif
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void ****)force, (void *)(SPM_BASE+FRC_OFFSET), sizeof(dvector_t)*3*(blockSide+densityFactor));
#endif
    acc = 0x00;
	if(!checkData(&mds)) {
        for(i=0; i<blockSide; i++) {
            for(j=0; j<blockSide; j++) {
                for(k=0; k<blockSide; k++) {
                    for(d=0; d<densityFactor; d++) {
                        if((((mds.force[i][j][k][d].x - mds.check[i][j][k][d].x)) > EPSILON) || ((mds.force[i][j][k][d].x - mds.check[i][j][k][d].x) < -EPSILON))
            	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, mds.force[i][j][k][d].x, i, mds.check[i][j][k][d].x);
                        if((((mds.force[i][j][k][d].y - mds.check[i][j][k][d].y)) > EPSILON) || ((mds.force[i][j][k][d].y - mds.check[i][j][k][d].y) < -EPSILON))
            	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, mds.force[i][j][k][d].y, i, mds.check[i][j][k][d].y);
                        if(((mds.force[i][j][k][d].z - mds.check[i][j][k][d].z) > EPSILON) || ((mds.force[i][j][k][d].z - mds.check[i][j][k][d].z) < -EPSILON))
            	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, mds.force[i][j][k][d].z, i, mds.check[i][j][k][d].z);
                    }
                }
            }
        }
	}
	*(char *)0x7fffffff = 1; //Kill the simulation
}
