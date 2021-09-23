#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "bench.h"
#include "../../../common/m5ops.h"

#define BASE            0x80c00000
#define FRCX_OFFSET     0
#define FRCY_OFFSET     (FRCX_OFFSET  + sizeof(TYPE)*nAtoms)
#define FRCZ_OFFSET     (FRCY_OFFSET  + sizeof(TYPE)*nAtoms)
#define POSX_OFFSET     (FRCZ_OFFSET  + sizeof(TYPE)*nAtoms)
#define POSY_OFFSET     (POSX_OFFSET  + sizeof(TYPE)*nAtoms)
#define POSZ_OFFSET     (POSY_OFFSET  + sizeof(TYPE)*nAtoms)
#define NL_OFFSET       (POSZ_OFFSET  + sizeof(TYPE)*nAtoms)
#define CHKX_OFFSET     (NL_OFFSET    + sizeof(int32_t)*nAtoms*maxNeighbors)
#define CHKY_OFFSET     (CHKX_OFFSET  + sizeof(TYPE)*nAtoms)
#define CHKZ_OFFSET     (CHKY_OFFSET  + sizeof(TYPE)*nAtoms)

md_struct mds;

volatile uint8_t  * top            = (uint8_t  *) (TOP + 0);
volatile uint32_t * loc_force_x    = (uint32_t *) (TOP + 1);
volatile uint32_t * loc_force_y    = (uint32_t *) (TOP + 9);
volatile uint32_t * loc_force_z    = (uint32_t *) (TOP + 17);
volatile uint32_t * loc_position_x = (uint32_t *) (TOP + 25);
volatile uint32_t * loc_position_y = (uint32_t *) (TOP + 33);
volatile uint32_t * loc_position_z = (uint32_t *) (TOP + 41);
volatile uint32_t * loc_NL         = (uint32_t *) (TOP + 49);

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

    volatile int count = 0;
    stage = 0;

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

    *loc_force_x     = (uint32_t)(void *)force_x;
    *loc_force_y     = (uint32_t)(void *)force_y;
    *loc_force_z     = (uint32_t)(void *)force_z;
    *loc_position_x  = (uint32_t)(void *)position_x;
    *loc_position_y  = (uint32_t)(void *)position_y;
    *loc_position_z  = (uint32_t)(void *)position_z;
    *loc_NL          = (uint32_t)(void *)NL;

    *top = 0x01;
    while (stage < 1) count++;

    printf("Job complete\n");

#ifdef CHECK
    bool fail = false;
    for (int i = 0; i < nAtoms; i++) {
        if ((std::abs(force_x[i]-check_x[i])/check_x[i] > EPSILON) ||
            (std::abs(force_y[i]-check_y[i])/check_y[i] > EPSILON) ||
            (std::abs(force_z[i]-check_z[i])/check_z[i] > EPSILON)) {
            fail = true;
            break;
        }
    }
    if(fail)
        printf("Check Failed\n");
    else
        printf("Check Passed\n");
#endif
    m5_dump_stats();
    m5_exit();
}
