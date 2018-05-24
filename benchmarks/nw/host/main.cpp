#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "nw.h"

nw_struct nws;

#define BASE        0x80c00000
#define SPM_BASE    0x2f100000
#define SEQA_OFF    0
#define SEQB_OFF    ALEN
#define ALIA_OFF    ALEN+BLEN
#define ALIB_OFF    2*(ALEN+BLEN)
#define M_OFF       3*(ALEN+BLEN)
#define PTR_OFF     3*(ALEN+BLEN)+4*((ALEN+1)*(BLEN+1))
#define CHECKA_OFF  3*(ALEN+BLEN)+5*((ALEN+1)*(BLEN+1))
#define CHECKB_OFF  4*(ALEN+BLEN)+5*((ALEN+1)*(BLEN+1))

int main(void) {
	char * seqA     = (char *)(BASE+SEQA_OFF);
	char * seqB     = (char *)(BASE+SEQB_OFF);
	char * alignedA = (char *)(BASE+ALIA_OFF);
	char * alignedB = (char *)(BASE+ALIB_OFF);
	int  * M        = (int  *)(BASE+M_OFF);
	char * ptr      = (char *)(BASE+PTR_OFF);
	char * checkA   = (char *)(BASE+CHECKA_OFF);
	char * checkB   = (char *)(BASE+CHECKB_OFF);

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

    std::memcpy((void *)(SPM_BASE+SEQA_OFF), (void *)seqA, ALEN);
    std::memcpy((void *)(SPM_BASE+SEQB_OFF), (void *)seqB, BLEN);
#endif
    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}
#ifdef SPM
    std::memcpy((void *)alignedA, (void *)(SPM_BASE+ALIA_OFF), (ALEN+BLEN));
    std::memcpy((void *)alignedB, (void *)(SPM_BASE+ALIB_OFF), (ALEN+BLEN));
#endif
    acc = 0x00;
	if(!checkData(&nws)) {
	    for (i = 0; i < (ALEN+BLEN); i++) {
	        if (alignedA[i] != checkA[i])
	            printf("alignedA[%2d]=\'%s\' Expected=\'%s\'\n", i, alignedA[i], checkA[i]);
            if (alignedB[i] != checkB[i])
	            printf("alignedB[%2d]=\'%s\' Expected=\'%s\'\n", i, alignedB[i], checkB[i]);
	    }
	}
	*(char *)0x7fffffff = 1;
}
