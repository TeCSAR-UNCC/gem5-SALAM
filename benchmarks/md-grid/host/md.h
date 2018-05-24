#include "../defines.h"

#define common_val      *(unsigned *)0x80b00000
#define acc             *(char *)0x2f000000
#define loc_n_points    *(int *)0x2f000001
#define loc_force       *(int *)0x2f000009
#define loc_position    *(int *)0x2f000011

#define EPSILON 1.0e-6

typedef struct {
    int32_t *** n_points;
    dvector_t **** force;
    dvector_t **** position;
    dvector_t **** check;
//    int blockSide;
//    int densityFactor;
} md_struct;

int checkData(md_struct * mds) {
    int i, j, k, d;

    for(i=0; i<blockSide; i++) {
        for(j=0; j<blockSide; j++) {
            for(k=0; k<blockSide; k++) {
                for(d=0; d<densityFactor; d++) {
                    if (mds->force[i][j][k][d].x!= mds->check[i][j][k][d].x) {
                        printf("Check Failed\n");
                        return 0;
                    }
                    if (mds->force[i][j][k][d].y!= mds->check[i][j][k][d].y) {
                        printf("Check Failed\n");
                        return 0;
                    }
                    if (mds->force[i][j][k][d].z!= mds->check[i][j][k][d].z) {
                        printf("Check Failed\n");
                        return 0;
                    }
                }
            }
        }
    }
    
    printf("Check Passed\n");
    return 1;
}

void genData(md_struct * mds) {
    ivector_t b0, b1; // b0 is the current block, b1 is b0 or a neighboring block
    dvector_t p, q; // p is a point in b0, q is a point in either b0 or b1
    int32_t p_idx, q_idx, i, j, k, l;
    TYPE dx, dy, dz, r2inv, r6inv, potential, f;
    
    for(i=0; i<blockSide; i++) {
        for(j=0; j<blockSide; j++) {
            for(k=0; k<blockSide; k++) {
                mds->n_points[i][j][k] = blockSide*2*i + blockSide*j + k;
//                printf("Creating npoints: %d\n", mds->n_points[i][j][k]);
            }
        }
    }
//    printf("Created npoints\n");
    
    for(i=0; i<blockSide; i++) {
        for(j=0; j<blockSide; j++) {
            for(k=0; k<blockSide; k++) {
                for(l=0; l<densityFactor; l++) {
                    mds->position[i][j][k][l].x = blockSide*3*i + blockSide*2*j + blockSide*k + l + 3;
                    mds->position[i][j][k][l].y = blockSide*3*i + blockSide*2*j + blockSide*(blockSide-k) + l + 4;
                    mds->position[i][j][k][l].z = blockSide*3*i + blockSide*2*(blockSide-j) + blockSide*(blockSide-k) + l +3;
//                    printf("Creating position [%d][%d][%d][%d]: %d %d %d\n", i, j, k, l, mds->position[i][j][k][l].x, mds->position[i][j][k][l].y, mds->position[i][j][k][l].z);
                }
            }
        }
    }
//        printf("Created position all\n");
      // Iterate over the grid, block by block
    for( b0.x=0; b0.x<blockSide; b0.x++ ) {
    for( b0.y=0; b0.y<blockSide; b0.y++ ) {
    for( b0.z=0; b0.z<blockSide; b0.z++ ) {

      // Iterate over the 3x3x3 (modulo boundary conditions) cube of blocks around b0
        for( b1.x=MAX(0,b0.x-1); b1.x<MIN(blockSide,b0.x+2); b1.x++ ) {
        for( b1.y=MAX(0,b0.y-1); b1.y<MIN(blockSide,b0.y+2); b1.y++ ) {
        for( b1.z=MAX(0,b0.z-1); b1.z<MIN(blockSide,b0.z+2); b1.z++ ) {

            // For all points in b0
            dvector_t *base_q = mds->position[b1.x][b1.y][b1.z];
            int q_idx_range = mds->n_points[b1.x][b1.y][b1.z];
            for( p_idx=0; p_idx<mds->n_points[b0.x][b0.y][b0.z]; p_idx++ ) {
                p = mds->position[b0.x][b0.y][b0.z][p_idx];
                TYPE sum_x = mds->force[b0.x][b0.y][b0.z][p_idx].x;
                TYPE sum_y = mds->force[b0.x][b0.y][b0.z][p_idx].y;
                TYPE sum_z = mds->force[b0.x][b0.y][b0.z][p_idx].z;

                // For all points in b1
                for( q_idx=0; q_idx< q_idx_range ; q_idx++ ) {
                    q = *(base_q + q_idx);

                    // Don't compute our own
                    if( q.x!=p.x || q.y!=p.y || q.z!=p.z ) {
                        // Compute the LJ-potential
                        dx = p.x - q.x;
                        dy = p.y - q.y;
                        dz = p.z - q.z;
                        r2inv = 1.0/( dx*dx + dy*dy + dz*dz );
                        r6inv = r2inv*r2inv*r2inv;
                        potential = r6inv*(lj1*r6inv - lj2);
                        // Update forces
                        f = r2inv*potential;
                        sum_x += f*dx;
                        sum_y += f*dy;
                        sum_z += f*dz;
                    }
                } // loop_q
                mds->force[b0.x][b0.y][b0.z][p_idx].x = sum_x ;
                mds->force[b0.x][b0.y][b0.z][p_idx].y = sum_y ;
                mds->force[b0.x][b0.y][b0.z][p_idx].z = sum_z ;
            } // loop_p
        }}} // loop_grid1_*
    }}} // loop_grid0_*
}

