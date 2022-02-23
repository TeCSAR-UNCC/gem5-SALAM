#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

volatile uint8_t * top  = (uint8_t *)TOP;

int main(void) {
    unsigned * arr    = (unsigned *)TESTA;
    unsigned * arrle    = (unsigned *)TESTB;
    unsigned * arrge    = (unsigned *)TESTC;
    unsigned * vshn    = (unsigned *)TESTD;
    unsigned * ky    = (unsigned *)TESTH;
     unsigned * ghj    = (unsigned *)TESTL;

    volatile int count = 0;
    int i;
    for(i = 0; i < 6; i++){
        arr[i] = i*10+10;
        arrle[i]=0;
        arrge[i]=0;
        vshn[i]=0;
    }

    

    *top = 0x01;
    while (stage < 1) count++;

    printf("A Check: %d %d %d \n",arr[0], ky, ghj);
    // printf("A Check: %d %s\n", achk, achk==32640 ? "PASSED" : "FAILED");
    // printf("B Check: %d %s\n", bchk, bchk==32640 ? "PASSED" : "FAILED");
    // printf("C Check: %d %s\n", cchk, cchk==32640 ? "PASSED" : "FAILED");
    // printf("D Check: %d %s\n", dchk, dchk==32640 ? "PASSED" : "FAILED");
    // printf("E Check: %d %s\n", echk, echk==32640 ? "PASSED" : "FAILED");

    m5_dump_stats();
    m5_exit();
}


