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

} node_t;

typedef struct rootCache{
	node_t entry[10];
	int occupancy;
} cache_t;

//SEQUENTIAL

void top(unsigned * a, unsigned* b, unsigned *c) {


	unsigned * f    = (unsigned *)0x80c10000;
    unsigned * g    = (unsigned *)0x80c10008;
    cache_t * h     = (cache_t *)TESTA;

    int count;

    // volatile int count = 0;
    // h->occupancy = 0;
    f[1] = 12;
    g[1] = 19;
    g[0] = 2;
    

    // *top = 0x01;


    *root = 0x01;
    while ((*root & 4) != 4) count++;

	f[1] = 12;
    g[1] = 19;
    g[0] = 2;
    

    // *top = 0x01;


    *root = 0x01;
    while ((*root & 4) != 4) count++;

	f[0] = 10;
    g[1] = 19;
    g[0] = 1;
    

    // *top = 0x01;


    *root = 0x01;
    while ((*root & 4) != 4) count++;

	*b = g[0];
	
	// bTree* cursor = (bTree*)*c;
	// uint64_t key = *a;
	
	

	// while(!cursor->l){
	// 	for(int i = 0; i < cursor->n; i++){
	// 		if(key<cursor->d[i])
	// 		{
	// 			cursor = cursor->child_ptr[i];
				
	// 			break;
	// 		}
	// 		if(key == cursor->d[i])
	// 		{
	// 			*b=1;
	// 			cursor = cursor->child_ptr[i];
	// 			break;
	// 		}
	// 		if(i == cursor->n - 1){
	// 			cursor = cursor->child_ptr[i+1];
	// 			break;
	// 		}
	// 	}
	// }

	// for(int i = 0; i < cursor->n; i++){
		
	// 	if(key == cursor->d[i])
	// 	{
	// 		*b=1;
	// 		break;
	// 	}
	// }





	return;

  
}