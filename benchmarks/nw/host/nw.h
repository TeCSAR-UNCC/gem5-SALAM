#include "../defines.h"
#include "data.h"

#define common_val    *(unsigned *)0x80b00000
#define acc           *(char *)0x2f000000
#define val_seqa      *(int *)0x2f000001
#define val_seqb      *(int *)0x2f000009
#define val_aligna    *(int *)0x2f000011
#define val_alignb    *(int *)0x2f000019
#define val_M         *(int *)0x2f000021
#define val_ptr       *(int *)0x2f000029

#define CHECK

typedef struct {
    char * seqA;
    char * seqB;
    char * alignedA;
    char * alignedB;
    int  * M;
    char  * ptr;
    char * checkA;
    char * checkB;
} nw_struct;

int checkData(nw_struct * nws) {
    int i;
    for (i = 0; i < (ALEN+BLEN); i++) {
        if (nws->alignedA[i] != nws->checkA[i]) {
            printf("Check Failed\n");
            return 0;
        }
        if (nws->alignedB[i] != nws->checkB[i]) {
            printf("Check Failed\n");
            return 0;
        }
        printf("Aligned A Value: %d \t Check A Value: %d\n", nws->alignedA[i], nws->checkA[i]);
        printf("Aligned B Value: %d \t Check B Value: %d\n", nws->alignedB[i], nws->checkB[i]);
    }
    printf("Check Passed\n");
    return 1;
}

void genData(nw_struct * nws) {
    int i;
	for (i = 0; i < ALEN; i++) {
        nws->seqA[i] = a[i];
    }
    for (i = 0; i < BLEN; i++) {
        nws->seqB[i] = b[i];
    }
    for (i = 0; i < (ALEN+BLEN); i++) {
        nws->checkA[i] = alia[i];
        nws->checkB[i] = alib[i];
    }
}
