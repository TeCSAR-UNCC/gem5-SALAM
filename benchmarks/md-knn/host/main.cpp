#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "md.h"

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

	common_val = 0;

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

    printf("Generating data\n");
    genData(&mds);
    printf("Data generated\n");
    
#ifndef SPM
    loc_force_x     = (uint64_t)(BASE+FRCX_OFFSET);
    loc_force_y     = (uint64_t)(BASE+FRCY_OFFSET);
    loc_force_z     = (uint64_t)(BASE+FRCY_OFFSET);
    loc_position_x  = (uint64_t)(BASE+POSX_OFFSET);
    loc_position_y  = (uint64_t)(BASE+POSY_OFFSET);
    loc_position_z  = (uint64_t)(BASE+POSZ_OFFSET);
    loc_NL          = (uint64_t)(BASE+NL_OFFSET);
#else
    loc_force_x     = (uint64_t)(BASE+FRCX_OFFSET);
    loc_force_y     = (uint64_t)(BASE+FRCY_OFFSET);
    loc_force_z     = (uint64_t)(BASE+FRCY_OFFSET);
    loc_position_x  = (uint64_t)(BASE+POSX_OFFSET);
    loc_position_y  = (uint64_t)(BASE+POSY_OFFSET);
    loc_position_z  = (uint64_t)(BASE+POSZ_OFFSET);
    loc_NL          = (uint64_t)(BASE+NL_OFFSET);

    std::memcpy((void *)(SPM_BASE+POSX_OFFSET), (void ***)position_x,   sizeof(TYPE)*nAtoms);
    std::memcpy((void *)(SPM_BASE+POSY_OFFSET), (void ***)position_y,   sizeof(TYPE)*nAtoms);
    std::memcpy((void *)(SPM_BASE+POSZ_OFFSET), (void ***)position_z,   sizeof(TYPE)*nAtoms);
    std::memcpy((void *)(SPM_BASE+NL_OFFSET),   (void ***)NL,           sizeof(int32_t)*nAtoms*maxNeighbors);
#endif
    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void *)(SPM_BASE+FRCX_OFFSET), (void ***)force_x,      sizeof(TYPE)*nAtoms);
    std::memcpy((void *)(SPM_BASE+FRCY_OFFSET), (void ***)force_y,      sizeof(TYPE)*nAtoms);
    std::memcpy((void *)(SPM_BASE+FRCZ_OFFSET), (void ***)force_z,      sizeof(TYPE)*nAtoms);
#endif
    acc = 0x00;
    if(!checkData(&mds)) {
        /* TBD */
    }
	*(char *)0x7fffffff = 1; //Kill the simulation
}
