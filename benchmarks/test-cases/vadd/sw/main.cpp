#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

volatile uint8_t * top  = (uint8_t *)TOP;

int main(void) {
    unsigned * a    = (unsigned *)TESTA;
    unsigned * b    = (unsigned *)TESTB;
    unsigned * c    = (unsigned *)TESTC;


    volatile int count = 0;
    int i;
    for(i = 0; i < 256; i++){
        a[i] = i + 1;
        b[i] = i + 1;
    }

    

    *top = 0x01;
    while (stage < 1) count++;
    for(i = 0; i < 256; i++){
        printf("A Check: %d %d %d \n",a[i], b[i], c[i]);
    }
    // printf("A Check: %d %s\n", achk, achk==32640 ? "PASSED" : "FAILED");
    // printf("B Check: %d %s\n", bchk, bchk==32640 ? "PASSED" : "FAILED");
    // printf("C Check: %d %s\n", cchk, cchk==32640 ? "PASSED" : "FAILED");
    // printf("D Check: %d %s\n", dchk, dchk==32640 ? "PASSED" : "FAILED");
    // printf("E Check: %d %s\n", echk, echk==32640 ? "PASSED" : "FAILED");

    m5_dump_stats();
    m5_exit();
}


