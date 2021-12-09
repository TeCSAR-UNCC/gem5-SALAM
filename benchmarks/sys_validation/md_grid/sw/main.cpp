#include <cstdio>
#include <cstring>

#include "bench.h"
#include "../../../common/m5ops.h"

#define BASE            0x80c00000

#define NP_OFFSET       0
#define FRC_OFFSET      (NP_OFFSET  + sizeof(int32_t)*blockSide*blockSide*blockSide)
#define POS_OFFSET      (FRC_OFFSET + sizeof(dvector_t)*(blockSide*blockSide*blockSide*densityFactor))
#define CHK_OFFSET      (POS_OFFSET + sizeof(dvector_t)*(blockSide*blockSide*blockSide*densityFactor))

md_struct mds;

volatile uint8_t  * top   = (uint8_t  *)(TOP + 0x00);
volatile uint32_t * val_a = (uint32_t *)(TOP + 0x01);
volatile uint32_t * val_b = (uint32_t *)(TOP + 0x09);
volatile uint32_t * val_c = (uint32_t *)(TOP + 0x11);

int main(void) {
	int32_t *n_points    = (int32_t *)(BASE+NP_OFFSET);
	TYPE *force          = (TYPE    *)(BASE+FRC_OFFSET);
	TYPE *position       = (TYPE    *)(BASE+POS_OFFSET);
	TYPE *check          = (TYPE    *)(BASE+CHK_OFFSET);
    int i, j, k, d;
    volatile int count = 0;
	stage = 0;

    mds.n_points = n_points;
    mds.force = force;
    mds.position = position;
    mds.check = check;

    printf("Generating data\n");
    genData(&mds);
    printf("Data generated\n");

    *val_a = (uint32_t)(void *)n_points;
    *val_b = (uint32_t)(void *)force;
    *val_c = (uint32_t)(void *)position;
    // printf("%d\n", *top);
    *top = 0x01;
    while (*top != 0x04) count++;

    printf("Job complete\n");
#ifdef CHECK
	if(!checkData(&mds)) {
	    for(i = 0; i < blockSide3*densityFactor; i++) {
	        int dind = i % densityFactor;
	        int kind = (i - dind) / densityFactor % blockSide;
	        int jind = (i - dind - densityFactor * kind) / densityFactor / blockSide % blockSide;
	        int iind = (i - dind - densityFactor * kind - densityFactor * blockSide * jind) / densityFactor / blockSide / blockSide;
            printf("f[%d][%d][%d][%d]\n", iind, jind, kind, dind);
            printf("xe:%.10f\n", force[3*i]);
            printf("xc:%.10f\n", check[3*i]);
            printf("ye:%.10f\n", force[3*i+1]);
            printf("yc:%.10f\n", check[3*i+1]);
            printf("ze:%.10f\n", force[3*i+2]);
            printf("zc:%.10f\n", check[3*i+2]);
            printf("\n");
	    }
	}
#endif
	m5_dump_stats();
	m5_exit();
}
