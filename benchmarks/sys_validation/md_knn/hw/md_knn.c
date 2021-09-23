/*
Implemenataion based on:
A. Danalis, G. Marin, C. McCurdy, J. S. Meredith, P. C. Roth, K. Spafford, V. Tipparaju, and J. S. Vetter.
The scalable heterogeneous computing (shoc) benchmark suite.
In Proceedings of the 3rd Workshop on General-Purpose Computation on Graphics Processing Units, 2010.
*/

#include "../defines.h"

void md_kernel() {

    uint8_t * force_x_base    = (uint8_t *)FORCEXADDR;
    uint8_t * force_y_base    = (uint8_t *)FORCEYADDR;
    uint8_t * force_z_base    = (uint8_t *)FORCEZADDR;
    uint8_t * position_x_base = (uint8_t *)POSITIONXADDR;
    uint8_t * position_y_base = (uint8_t *)POSITIONYADDR;
    uint8_t * position_z_base = (uint8_t *)POSITIONZADDR;
    uint8_t * nl_base         = (uint8_t *)NLADDR;
    TYPE    * force_x         = (TYPE    *)force_x_base;
    TYPE    * force_y         = (TYPE    *)force_y_base;
    TYPE    * force_z         = (TYPE    *)force_z_base;
    TYPE    * position_x      = (TYPE    *)position_x_base;
    TYPE    * position_y      = (TYPE    *)position_y_base;
    TYPE    * position_z      = (TYPE    *)position_z_base;
    int32_t * NL              = (int32_t *)nl_base;

    TYPE delx, dely, delz, r2inv;
    TYPE r6inv, potential, force, j_x, j_y, j_z;
    TYPE i_x, i_y, i_z, fx, fy, fz;
    int32_t i, j, jidx;

    loop_i :
    for (i = 0; i < nAtoms; i++) {
        i_x = position_x[i];
        i_y = position_y[i];
        i_z = position_z[i];
        fx = 0;
        fy = 0;
        fz = 0;

        loop_j :
        for( j = 0; j < maxNeighbors; j++) {
            // Get neighbor
            jidx = NL[i*maxNeighbors + j];
            // Look up x,y,z positions
            j_x = position_x[jidx];
            j_y = position_y[jidx];
            j_z = position_z[jidx];
            // Calc distance
            delx = i_x - j_x;
            dely = i_y - j_y;
            delz = i_z - j_z;
            r2inv = 1.0/( delx*delx + dely*dely + delz*delz );
            // Assume no cutoff and aways account for all nodes in area
            r6inv = r2inv * r2inv * r2inv;
            potential = r6inv*(lj1*r6inv - lj2);
            // Sum changes in force
            force = r2inv*potential;
            fx += delx * force;
            fy += dely * force;
            fz += delz * force;
        }
        //Update forces after all neighbors accounted for.
        force_x[i] = fx;
        force_y[i] = fy;
        force_z[i] = fz;
    }
}
