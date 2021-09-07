#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

nw_struct nws;

#define BASE        0x80c00000
#define SEQA_OFF    0
#define SEQB_OFF    SEQA_OFF   + ALEN
#define ALIA_OFF    SEQB_OFF   + BLEN
#define ALIB_OFF    ALIA_OFF   + (ALEN+BLEN)
#define CHECKA_OFF  ALIB_OFF   + (ALEN+BLEN)
#define CHECKB_OFF  CHECKA_OFF + (ALEN+BLEN)

volatile uint8_t * top        = (uint8_t *)(TOP + 0x00);
volatile int32_t * val_seqa   = (int32_t *)(TOP + 0x01);
volatile int32_t * val_seqb   = (int32_t *)(TOP + 0x09);
volatile int32_t * val_aligna = (int32_t *)(TOP + 0x11);
volatile int32_t * val_alignb = (int32_t *)(TOP + 0x19);

int main(void) {
	char * seqA     = (char *)(BASE+SEQA_OFF);
	char * seqB     = (char *)(BASE+SEQB_OFF);
	char * alignedA = (char *)(BASE+ALIA_OFF);
	char * alignedB = (char *)(BASE+ALIB_OFF);
	char * checkA   = (char *)(BASE+CHECKA_OFF);
	char * checkB   = (char *)(BASE+CHECKB_OFF);

    volatile int count = 0;

	stage = 0;
    nws.seqA = seqA;
    nws.seqB = seqB;
    nws.alignedA = alignedA;
    nws.alignedB = alignedB;
    nws.checkA = checkA;
    nws.checkB = checkB;

    printf("Generating data\n");
    genData(&nws);
    printf("Data generated\n");

    *val_seqa   = (uint32_t)(void *)seqA;
    *val_seqb   = (uint32_t)(void *)seqB;
    *val_aligna = (uint32_t)(void *)alignedA;
    *val_alignb = (uint32_t)(void *)alignedB;
    *top = 0x01;
    while (stage < 1) count++;

    printf("Job complete\n");

#ifdef CHECK
    bool fail = false;
    for (int i = 0; i < (ALEN+BLEN); i++) {
        if (alignedA[i] != checkA[i]) {
            printf("Check Failed\n");
            fail = true;
            break;
        }
        if (alignedB[i] != checkB[i]) {
            printf("Check Failed\n");
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