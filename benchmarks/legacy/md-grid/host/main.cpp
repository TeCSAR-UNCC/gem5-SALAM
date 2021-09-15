#include <cstdio>
#include <cstring>

#include "md.h"

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define NP_OFFSET       0
#define FRC_OFFSET      (NP_OFFSET  + sizeof(int32_t)*blockSide*blockSide*blockSide)
#define POS_OFFSET      (FRC_OFFSET + sizeof(dvector_t)*(blockSide*blockSide*blockSide*densityFactor))
#define CHK_OFFSET      (POS_OFFSET + sizeof(dvector_t)*(blockSide*blockSide*blockSide*densityFactor))

md_struct mds;

int main(void) {
	int32_t *n_points    = (int32_t       *)(BASE+NP_OFFSET);
	TYPE *force    = (TYPE    *)(BASE+FRC_OFFSET);
	TYPE *position = (TYPE    *)(BASE+POS_OFFSET);
	TYPE *check    = (TYPE    *)(BASE+CHK_OFFSET);
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
    
    memcpy( (void *)(SPM_BASE+NP_OFFSET), (void ***)n_points, (sizeof(ivector_t)*3*blockSide));
	memcpy((void *)(SPM_BASE+FRC_OFFSET), (void *)force,     sizeof(dvector_t)*(blockSide*blockSide*blockSide*densityFactor));
    memcpy((void *)(SPM_BASE+POS_OFFSET), (void ****)position,  sizeof(dvector_t)*3*(blockSide+densityFactor));
#endif
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    memcpy((void ****)force, (void *)(SPM_BASE+FRC_OFFSET), sizeof(dvector_t)*3*(blockSide+densityFactor));
#endif
    acc = 0x00;
#ifdef CHECK
	if(!checkData(&mds)) {
	    for(i = 0; i < blockSide3*densityFactor; i++) {
	        int dind = i % densityFactor;
	        int kind = (i - dind) / densityFactor % blockSide;
	        int jind = (i - dind - densityFactor * kind) / densityFactor / blockSide % blockSide;
	        int iind = (i - dind - densityFactor * kind - densityFactor * blockSide * jind) / densityFactor / blockSide / blockSide;
	        double xerror = std::abs(force[3*i]-check[3*i])/check[3*i];
	        double yerror = std::abs(force[3*i+1]-check[3*i+1])/check[3*i+1];
	        double zerror = std::abs(force[3*i+2]-check[3*i+2])/check[3*i+2];
	        bool xfail = xerror > EPSILON;
	        bool yfail = yerror > EPSILON;
	        bool zfail = zerror > EPSILON;
	        if (xfail || yfail || zfail) {
	            printf("f[%d][%d][%d][%d]     \n", iind, jind, kind, dind);
	            if (xfail) {
                    printf("X                 \n");
                }
                if (yfail) {
                    printf("Y                 \n");
                }
                if (zfail) {
                    printf("Z                 \n");
                }
                printf("\nError\n");
                if (xfail) {
                    printf("x:%.3f%%       \n",xerror*100);
                    printf("%.14f\n", force[3*i]-check[3*i]);
                }
                if (yfail) {
                    printf("y:%.3f%%       \n",yerror*100);
                    printf("%.14f\n", force[3*i+1]-check[3*i+1]);
                }
                if (zfail) {
                    printf("z:%.3f%%       \n",zerror*100);
                    printf("%.14f\n", force[3*i+2]-check[3*i+2]);
                }
                printf("\n");
            }
	    }
//        for(i=0; i<blockSide; i++) {
//            for(j=0; j<blockSide; j++) {
//                for(k=0; k<blockSide; k++) {
//                    for(d=0; d<densityFactor; d++) {
//                        if((((mds.force[i][j][k][d].x - mds.check[i][j][k][d].x)) > EPSILON) || ((mds.force[i][j][k][d].x - mds.check[i][j][k][d].x) < -EPSILON))
//            	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, mds.force[i][j][k][d].x, i, mds.check[i][j][k][d].x);
//                        if((((mds.force[i][j][k][d].y - mds.check[i][j][k][d].y)) > EPSILON) || ((mds.force[i][j][k][d].y - mds.check[i][j][k][d].y) < -EPSILON))
//            	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, mds.force[i][j][k][d].y, i, mds.check[i][j][k][d].y);
//                        if(((mds.force[i][j][k][d].z - mds.check[i][j][k][d].z) > EPSILON) || ((mds.force[i][j][k][d].z - mds.check[i][j][k][d].z) < -EPSILON))
//            	            printf("out[%2d]=%10f expected[%d]=%10f\n", i, mds.force[i][j][k][d].z, i, mds.check[i][j][k][d].z);
//                    }
//                }
//            }
//        }
	}
#endif
	*(char *)0x7fffffff = 1; //Kill the simulation
}
