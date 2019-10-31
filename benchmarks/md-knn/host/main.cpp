#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "md.h"
#include "../../common/dma.h"
#include "../../common/m5ops.h"

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define FRCX_OFFSET     0
#define FRCY_OFFSET     (FRCX_OFFSET  + sizeof(TYPE)*nAtoms)
#define FRCZ_OFFSET     (FRCY_OFFSET  + sizeof(TYPE)*nAtoms)
#define POSX_OFFSET     (FRCZ_OFFSET  + sizeof(TYPE)*nAtoms)
#define POSY_OFFSET     (POSX_OFFSET  + sizeof(TYPE)*nAtoms)
#define POSZ_OFFSET     (POSY_OFFSET  + sizeof(TYPE)*nAtoms)
#define NL_OFFSET       (POSZ_OFFSET  + sizeof(TYPE)*nAtoms)
#define CHKX_OFFSET     (NL_OFFSET  + sizeof(int32_t)*nAtoms*maxNeighbors)
#define CHKY_OFFSET     (CHKX_OFFSET  + sizeof(TYPE)*nAtoms)
#define CHKZ_OFFSET     (CHKY_OFFSET  + sizeof(TYPE)*nAtoms)

md_struct mds;

int main(void) {


	TYPE *force_x       = (TYPE     *)(BASE+FRCX_OFFSET);
	TYPE *force_y       = (TYPE     *)(BASE+FRCY_OFFSET);
	TYPE *force_z       = (TYPE     *)(BASE+FRCZ_OFFSET);
	TYPE *position_x    = (TYPE     *)(BASE+POSX_OFFSET);
	TYPE *position_y    = (TYPE     *)(BASE+POSY_OFFSET);
	TYPE *position_z    = (TYPE     *)(BASE+POSZ_OFFSET);
	int32_t *NL         = (int32_t  *)(BASE+NL_OFFSET);
	TYPE *check_x       = (TYPE     *)(BASE+CHKX_OFFSET);
	TYPE *check_y       = (TYPE     *)(BASE+CHKY_OFFSET);
	TYPE *check_z       = (TYPE     *)(BASE+CHKZ_OFFSET);
#ifdef SPM
	TYPE *spmfx			= (TYPE		*)(SPM_BASE+FRCX_OFFSET);
	TYPE *spmfy			= (TYPE		*)(SPM_BASE+FRCY_OFFSET);
	TYPE *spmfz			= (TYPE		*)(SPM_BASE+FRCZ_OFFSET);
	TYPE *spmpx			= (TYPE		*)(SPM_BASE+POSX_OFFSET);
	TYPE *spmpy			= (TYPE		*)(SPM_BASE+POSY_OFFSET);
	TYPE *spmpz			= (TYPE		*)(SPM_BASE+POSZ_OFFSET);
	int32_t *spmnl		= (int32_t	*)(SPM_BASE+NL_OFFSET);
//	TYPE *check_x       = (TYPE     *)(SPM_BASE+CHKX_OFFSET);
//	TYPE *check_y       = (TYPE     *)(SPM_BASE+CHKY_OFFSET);
//	TYPE *check_z       = (TYPE     *)(SPM_BASE+CHKZ_OFFSET);
#endif
	
	common_val = 0;

//#ifndef SPM
    mds.force_x     = force_x;
    mds.force_y     = force_y;
    mds.force_z     = force_z;
    mds.position_x  = position_x;
    mds.position_y  = position_y;
    mds.position_z  = position_z;
    mds.NL          = NL;
    mds.check_x     = check_x;
    mds.check_y     = check_y;
    mds.check_z     = check_z;
/*
#else
    mds.force_x     = spmfx;
    mds.force_y     = spmfy;
    mds.force_z     = spmfz;
    mds.position_x  = spmpx;
    mds.position_y  = spmpy;
    mds.position_z  = spmpz;
    mds.NL          = spmnl;
    mds.check_x     = check_x;
    mds.check_y     = check_y;
    mds.check_z     = check_z;
#endif
*/
    printf("Generating data\n");
    genData(&mds);
    printf("Data generated\n");

#ifndef SPM
    loc_force_x     = (uint64_t)(BASE+FRCX_OFFSET);
    loc_force_y     = (uint64_t)(BASE+FRCY_OFFSET);
    loc_force_z     = (uint64_t)(BASE+FRCZ_OFFSET);
    loc_position_x  = (uint64_t)(BASE+POSX_OFFSET);
    loc_position_y  = (uint64_t)(BASE+POSY_OFFSET);
    loc_position_z  = (uint64_t)(BASE+POSZ_OFFSET);
    loc_NL          = (uint64_t)(BASE+NL_OFFSET);
#else
    loc_force_x     = (uint64_t)(SPM_BASE+FRCX_OFFSET);
    loc_force_y     = (uint64_t)(SPM_BASE+FRCY_OFFSET);
    loc_force_z     = (uint64_t)(SPM_BASE+FRCZ_OFFSET);
    loc_position_x  = (uint64_t)(SPM_BASE+POSX_OFFSET);
    loc_position_y  = (uint64_t)(SPM_BASE+POSY_OFFSET);
    loc_position_z  = (uint64_t)(SPM_BASE+POSZ_OFFSET);
    loc_NL          = (uint64_t)(SPM_BASE+NL_OFFSET);


    dmacpy(spmpx, position_x,	sizeof(TYPE)*nAtoms);
    while(!pollDma());
    resetDma();
    dmacpy(spmpy, position_y,   sizeof(TYPE)*nAtoms);
    while(!pollDma());
    resetDma();
    dmacpy(spmpz, position_z,   sizeof(TYPE)*nAtoms);
    while(!pollDma());
    resetDma();
    dmacpy(spmnl, NL,           sizeof(int32_t)*nAtoms*maxNeighbors);
    while(!pollDma());
    resetDma();


#endif
    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM

    dmacpy(force_x,	spmfx,	sizeof(TYPE)*nAtoms);
    while(!pollDma());
    resetDma();
    dmacpy(force_y,	spmfy,	sizeof(TYPE)*nAtoms);
    while(!pollDma());
    resetDma();
    dmacpy(force_z,	spmfz,	sizeof(TYPE)*nAtoms);
    while(!pollDma());

#endif
    acc = 0x00;
#ifdef CHECK
    
    if(!checkData(&mds)) {
     /*   int checkvals = 0;
        int errors = 0;
        for (int i = 0; i < nAtoms; i++) {
            bool xfail = (std::abs(force_x[i]-check_x[i])/check_x[i]) > EPSILON;
            bool yfail = (std::abs(force_y[i]-check_y[i])/check_y[i]) > EPSILON;
            bool zfail = (std::abs(force_z[i]-check_z[i])/check_z[i]) > EPSILON;
            double xerror = (std::abs(force_x[i]-check_x[i])/check_x[i]);
            double yerror = (std::abs(force_y[i]-check_y[i])/check_y[i]);
            double zerror = (std::abs(force_z[i]-check_z[i])/check_z[i]);
            double xemag = std::abs(force_x[i]-check_x[i]);
            double yemag = std::abs(force_y[i]-check_y[i]);
            double zemag = std::abs(force_z[i]-check_z[i]);
            checkvals+=3;
            if(xfail || yfail || zfail) {
                printf("I:%3d            \n", i);
                if(xfail) {
                    printf("X:             \n");
                    printf("%.3f%%\n %.13f\n", xerror*100, xemag);
                    errors++;
                }
                if(yfail) {
                    printf("Y:             \n");
                    printf("%.3f%%\n %.13f\n", yerror*100, yemag);
                    errors++;
                }
                if(zfail) {
                    printf("Z:             \n");
                    printf("%.3f%%\n %.13f\n", zerror*100, zemag);
                    errors++;
                }
            }
        }
        printf("                               \n");
        printf("Errors: %d \n Total Checks: %d \n", errors, checkvals);
    */
    }
    
#endif
	*(char *)0x7fffffff = 1; //Kill the simulation
	//m5_exit();
}
