#include "../defines.h"
#include "../nw_clstr_hw_defines.h"

#include "data.h"

volatile int stage;

typedef struct {
    char * seqA;
    char * seqB;
    char * alignedA;
    char * alignedB;
    char * checkA;
    char * checkB;
} nw_struct;

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