#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int *)0x2f000001
#define val_b      *(int *)0x2f000009
#define val_c      *(int *)0x2f000011
#define rcIndex     (r*ges->row_size + c)

typedef struct {
  int x, y, z;
} dvector_t;

typedef struct {
    int * n_points;
    dvector_t * force;
    dvector_t * position;
    dvector_t * check;
    int blockSide;
    int densityFactor;
} md_struct;
  
//void checkData(vadd_struct * vas) {
//    int i;
//    for (i = 0; i < vas->length; i++) {
//        if (vas->c[i] != vas->check[i]) {
//            printf("Check Failed\n");
//            return;
//        }
//    }
//    printf("Check Passed\n");
//}

void checkData( md_struct * mds ) {
    int i, j, k, d;

    for(i=0; i<blockSide; i++) {
        for(j=0; j<blockSide; j++) {
            for(k=0; k<blockSide; k++) {
                for(d=0; d<densityFactor; d++) {
                    if (mds->force[i][j][k][d].x!= mds->check[i][j][k][d].x) {
                        printf("Check Failed\n");
                        return;
                    }
                    if (mds->force[i][j][k][d].y!= mds->check[i][j][k][d].y) {
                        printf("Check Failed\n");
                        return;
                    }
                    if (mds->force[i][j][k][d].z!= mds->check[i][j][k][d].z) {
                        printf("Check Failed\n");
                        return;
                    }
                }
            }
        }
    }
    
    printf("Check Passed\n");
}

//void genData(vadd_struct * vas) {
//    int i;
//    for (i = 0; i < vas->length; i++) {
//        vas->a[i] = i;
//        vas->b[i] = vas->length - 1- i;
//        vas->check[i] = vas->a[i] + vas->b[i];
//    }
//}

void genData(gemm_struct * ges) {
    int r, c, k, mult, sum;
    
    for( r=0; r < ges->row_size; r++ ) {
        for( c=0; c < ges->col_size; c++ ) {
            ges->a[rcIndex] = rcIndex;
            ges->b[rcIndex] = (ges->row_size * ges->col_size) - 1 - rcIndex;
        }
    }
        
    ges->check[0] = 0;
    for( r=0; r < ges->row_size; r++ ) {
        for( c=0; c < ges->col_size; c++ ) {
            sum = ges->check[rcIndex];
            for(k=0; k < ges->row_size; k++) {
                mult = ges->a[(r*ges->row_size + k)] * ges->b[(k*ges->col_size + c)];
                sum += mult;
            }
            ges->check[rcIndex] = sum;
        }
    }
}

{
      // Iterate over the grid, block by block
    for( b0.x=0; b0.x<blockSide; b0.x++ ) {
    for( b0.y=0; b0.y<blockSide; b0.y++ ) {
    for( b0.z=0; b0.z<blockSide; b0.z++ ) {

      // Iterate over the 3x3x3 (modulo boundary conditions) cube of blocks around b0
        for( b1.x=MAX(0,b0.x-1); b1.x<MIN(blockSide,b0.x+2); b1.x++ ) {
        for( b1.y=MAX(0,b0.y-1); b1.y<MIN(blockSide,b0.y+2); b1.y++ ) {
        for( b1.z=MAX(0,b0.z-1); b1.z<MIN(blockSide,b0.z+2); b1.z++ ) {

            // For all points in b0
            dvector_t *base_q = position[b1.x][b1.y][b1.z];
            int q_idx_range = n_points[b1.x][b1.y][b1.z];
            loop_p: for( p_idx=0; p_idx<n_points[b0.x][b0.y][b0.z]; p_idx++ ) {
            p = position[b0.x][b0.y][b0.z][p_idx];
            TYPE sum_x = force[b0.x][b0.y][b0.z][p_idx].x;
            TYPE sum_y = force[b0.x][b0.y][b0.z][p_idx].y;
            TYPE sum_z = force[b0.x][b0.y][b0.z][p_idx].z;

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
            force[b0.x][b0.y][b0.z][p_idx].x = sum_x ;
            force[b0.x][b0.y][b0.z][p_idx].y = sum_y ;
            force[b0.x][b0.y][b0.z][p_idx].z = sum_z ;
            } // loop_p
        }}} // loop_grid1_*
    }}} // loop_grid0_*
}

