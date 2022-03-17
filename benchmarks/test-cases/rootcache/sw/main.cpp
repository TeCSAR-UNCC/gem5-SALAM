#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

volatile uint8_t * top  = (uint8_t *)TOP;
typedef struct rootCacheNode{
	int start;
	int end;
	int key;

} node_t;

typedef struct rootCache{
	node_t entry[10];
	int occupancy;
} cache_t;


int main(void) {
    unsigned * a    = (unsigned *)0x80c00000;
    unsigned * b    = (unsigned *)0x80c00008;
    cache_t * c    = (cache_t *)TESTA;


    volatile int count = 0;
    c->occupancy = 0;
    int i;
    a[1] = 12;
    b[1] = 19;
    b[0] = 2;
    

    // *top = 0x01;


    *top = 0x01;
    while (stage < 1) count++;

    printf("Rootcache Check: %d \n",b[0]);

    a[0] = 17;
    b[1] = 19;
    b[0] = 1;
    

    *top = 0x01;
    while (stage < 1) count++;

    printf("Rootcache Check: %d \n",b[0]);
    // printf("A Check: %d %s\n", achk, achk==32640 ? "PASSED" : "FAILED");
    // printf("B Check: %d %s\n", bchk, bchk==32640 ? "PASSED" : "FAILED");
    // printf("C Check: %d %s\n", cchk, cchk==32640 ? "PASSED" : "FAILED");
    // printf("D Check: %d %s\n", dchk, dchk==32640 ? "PASSED" : "FAILED");
    // printf("E Check: %d %s\n", echk, echk==32640 ? "PASSED" : "FAILED");

    m5_dump_stats();
    m5_exit();
}


