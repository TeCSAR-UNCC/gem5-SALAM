#include "../search_clstr_hw_defines.h"
#include <stdint.h>

typedef struct BTree{
   int d[7];
   struct BTree * child_ptr[8];
   int l;
   int n;
} bTree;

volatile uint8_t * root  = (uint8_t *)ROOTCACHE;
typedef struct rootCacheNode{
	int start;
	int end;
	int key;
	int utility;

} node_t;

typedef struct rootCache{
	node_t entry[10];
	int occupancy;
} cache_t;


typedef struct rc_query{
	unsigned start;
	unsigned end;
	unsigned start_address;
	unsigned flag;
} query_t;

//SEQUENTIAL

void top(unsigned * a, unsigned* b, unsigned *c) {
	
	unsigned* inStatus = (unsigned *)0x100206f0;
	unsigned* outStatus = (unsigned *)0x100207f0;
	query_t* strOut = (query_t*)0x10020600;
	unsigned* strIn = (unsigned *)cache_search_out;
	unsigned * key_rc    = (unsigned *)0x80c10000;
    unsigned * g    = (unsigned *)0x80c10008;
	// unsigned * m    = (unsigned *)0x80c10010;
    cache_t * h     = (cache_t *)TESTA;
	int count;
	unsigned temp = 0;
	uint64_t key = *a;

	bTree* cursor;
	bTree* ref;
	query_t q;
	q.start = *a;
	q.end = *a;
	q.start_address = *a;
	q.flag = *a;

	

	*strOut = q;

	*b = *strIn;

    
	

	return;

  
}