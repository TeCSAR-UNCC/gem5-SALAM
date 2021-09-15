#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "nw.h"
#include "../../common/dma.h"
#include "../../common/m5ops.h"

nw_struct nws;

#define BASE        0x80c00000
#define SPM_BASE    0x2f100000
#define SEQA_OFF    0
#define SEQB_OFF    SEQA_OFF + ALEN
#define ALIA_OFF    SEQB_OFF + BLEN
#define ALIB_OFF    ALIA_OFF + (ALEN+BLEN)
#define M_OFF       ALIB_OFF + (ALEN+BLEN)
#define PTR_OFF     M_OFF + 4*((ALEN+1)*(BLEN+1))
#define CHECKA_OFF  PTR_OFF + ((ALEN+1)*(BLEN+1))
#define CHECKB_OFF  CHECKA_OFF + (ALEN+BLEN)

int main(void) {
	char * seqA     = (char *)(BASE+SEQA_OFF);
	char * seqB     = (char *)(BASE+SEQB_OFF);
	char * alignedA = (char *)(BASE+ALIA_OFF);
	char * alignedB = (char *)(BASE+ALIB_OFF);
	int  * M        = (int  *)(BASE+M_OFF);
	char * ptr      = (char *)(BASE+PTR_OFF);
	char * checkA   = (char *)(BASE+CHECKA_OFF);
	char * checkB   = (char *)(BASE+CHECKB_OFF);
#ifdef SPM
	char *spmsA		= (char *)(SPM_BASE+SEQA_OFF);
	char *spmsB		= (char *)(SPM_BASE+SEQB_OFF);
	char *spmaA		= (char *)(SPM_BASE+ALIA_OFF);
	char *spmaB		= (char *)(SPM_BASE+ALIB_OFF);
#endif

	common_val = 0;
    nws.seqA = seqA;
    nws.seqB = seqB;
    nws.alignedA = alignedA;
    nws.alignedB = alignedB;
    nws.M = M;
    nws.ptr = ptr;
    nws.checkA = checkA;
    nws.checkB = checkB;

    printf("Generating data\n");
    genData(&nws);
    printf("Data generated\n");
#ifndef SPM
    val_seqa   = (uint64_t)(BASE+SEQA_OFF);
    val_seqb   = (uint64_t)(BASE+SEQB_OFF);
    val_aligna = (uint64_t)(BASE+ALIA_OFF);
    val_alignb = (uint64_t)(BASE+ALIB_OFF);
    val_M      = (uint64_t)(BASE+M_OFF);
    val_ptr    = (uint64_t)(BASE+PTR_OFF);
#else
    val_seqa   = (uint64_t)(SPM_BASE+SEQA_OFF);
    val_seqb   = (uint64_t)(SPM_BASE+SEQB_OFF);
    val_aligna = (uint64_t)(SPM_BASE+ALIA_OFF);
    val_alignb = (uint64_t)(SPM_BASE+ALIB_OFF);
    val_M      = (uint64_t)(SPM_BASE+M_OFF);
    val_ptr    = (uint64_t)(SPM_BASE+PTR_OFF);

    dmacpy(spmsA, seqA, ALEN);
    while(!pollDma());
    resetDma();
    dmacpy(spmsB, seqB, BLEN);
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
    dmacpy(alignedA, spmaA, (ALEN+BLEN));
    while(!pollDma());
    resetDma();
    dmacpy(alignedB, spmaB, (ALEN+BLEN));
    while(!pollDma());
#endif
    acc = 0x00;
#ifdef CHECK
	if(!checkData(&nws)) {
	    for (i = 0; i < (ALEN+BLEN); i++) {
	        //if (alignedA[i] != checkA[i])
	         //   printf("alignedA[%2d]=\'%c\' Expected=\'%c\'\n", i, alignedA[i], checkA[i]);
            	//if (alignedB[i] != checkB[i])
	         //   printf("alignedB[%2d]=\'%c\' Expected=\'%c\'\n", i, alignedB[i], checkB[i]);
			printf("%c", alignedA[i]);
	    }
	    printf("\n");
		for (i = 0; i < (ALEN+BLEN); i++) {
			printf("%c", checkA[i]);
		}
		printf("\n");
		for (i = 0; i < (ALEN+BLEN); i++) {
			printf("%c", alignedB[i]);
		}
		printf("\n");
		for (i = 0; i < (ALEN+BLEN); i++) {
			printf("%c", checkB[i]);
		}
	}
#endif
	// *(char *)0x7fffffff = 1;
	m5_exit();
}
