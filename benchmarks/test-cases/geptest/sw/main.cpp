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
    unsigned * d    = (unsigned *)TESTD;
    unsigned * e    = (unsigned *)TESTE;

    volatile int count = 0;

    unsigned achk = 0;
    unsigned bchk = 0;
    unsigned cchk = 0;
    unsigned dchk = 0;
    unsigned echk = 0;

    *top = 0x01;
    while (stage < 1) count++;

    for (count=0; count<255; count++) {
        achk += a[count];
        bchk += b[count];
        cchk += c[count];
        dchk += d[count];
        echk += e[count];
    }
    printf("A Check: %s\n", achk==32640 ? "PASSED" : "FAILED");
    printf("B Check: %s\n", bchk==32640 ? "PASSED" : "FAILED");
    printf("C Check: %s\n", cchk==32640 ? "PASSED" : "FAILED");
    printf("D Check: %s\n", dchk==32640 ? "PASSED" : "FAILED");
    printf("E Check: %s\n", echk==32640 ? "PASSED" : "FAILED");

    m5_exit();
}
