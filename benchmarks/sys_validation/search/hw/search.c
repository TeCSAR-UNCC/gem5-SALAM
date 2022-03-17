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

//SEQUENTIAL

void top(unsigned * a, unsigned* b, unsigned *c) {


	unsigned * key_rc    = (unsigned *)0x80c10000;
    unsigned * g    = (unsigned *)0x80c10008;
	// unsigned * m    = (unsigned *)0x80c10010;
    cache_t * h     = (cache_t *)TESTA;
	int count;

	bTree* cursor;
	bTree* ref;
	key_rc[0] = *a;
    g[1] = *a;
    g[0] = 1;
    

    *root = 0x01;
    while ((*root & 4) != 4) count++;

	// *b=g[0];
	if(g[0]>-1)
	{
		cursor=(bTree*)g[0];
	}
	else{
		cursor=(bTree*)*c;
	}

	ref=g[0];
    

    // volatile int count = 0;
    // h->occupancy = 0;
    

	// *b = g[0];

	int flg=0;
	int index=0;
	
	// bTree* cursor = (bTree*)*c;
	uint64_t key = *a;
	
	

	while(!cursor->l){
		for(int i = 0; i < cursor->n; i++){
			if(key<cursor->d[i]){
				flg=1;
				index=i;
				// cursor = cursor->child_ptr[i];
				
				break;
			}
			if(key == cursor->d[i]){

				*b=1;
				flg=1;
				if(i-1>0){
					key_rc[1] = cursor->d[i-1];
					g[1] = key;
					key_rc[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					
				}
				else{
					if(i+1<cursor->n){
						key_rc[1] = key;
						g[1] = cursor->d[i+1];
						key_rc[0] = cursor;
						// m[0] = (unsigned)&key; 
						g[0] = 2;
						*root = 0x01;
						while ((*root & 4) != 4) count++;
					}
					else{
						key_rc[1] = key;
						g[1] = cursor->d[i];
						key_rc[0] = cursor;
						// m[0] = (unsigned)&key; 
						g[0] = 2;
						*root = 0x01;
						while ((*root & 4) != 4) count++;
					}

				}
				flg=1;
				index=i;

				// cursor = cursor->child_ptr[i];
				break;
			}
			if(i == cursor->n - 1){
				flg=2;
				index=i;
				// cursor = cursor->child_ptr[i+1];
				break;
			}
		}
		
		if(index-1>0){
			key_rc[1] = cursor->d[index-1];
			g[1] = cursor->d[index];
			key_rc[0] = cursor;
			// m[0] = (unsigned)&key; 
			g[0] = 2;
			*root = 0x01;
			while ((*root & 4) != 4) count++;
			
		}
		else{
		
			if(index+1<cursor->n){
				key_rc[1] = key;
				g[1] = cursor->d[index+1];
				key_rc[0] = cursor;
				// m[0] = (unsigned)&key; 
				g[0] = 2;
				*root = 0x01;
				while ((*root & 4) != 4) count++;
			}
			else{
				key_rc[1] = key;
				g[1] = cursor->d[index];
				key_rc[0] = cursor;
				// m[0] = (unsigned)&key; 
				g[0] = 2;
				*root = 0x01;
				while ((*root & 4) != 4) count++;
			}


		}
		if(flg==1)
			cursor=cursor->child_ptr[index];
		else if(flg==2)
			cursor=cursor->child_ptr[index+1];
		else
			cursor=cursor->child_ptr[cursor->n]; 

		flg=0;
		index=0;
	
	}
	for(int i = 0; i < cursor->n; i++){
		
		if(key == cursor->d[i]){
			*b=1;
			flg=1;
			if(i-1>0){
				key_rc[1] = cursor->d[i-1];
				g[1] = key;
				key_rc[0] = cursor;
				// m[0] = (unsigned)&key; 
				g[0] = 2;
				*root = 0x01;
				while ((*root & 4) != 4) count++;
				
			}
			else{
				key_rc[1] = key;
				g[1] = cursor->d[i+1];
				key_rc[0] = cursor;
				// m[0] = (unsigned)&key; 
				g[0] = 2;
				*root = 0x01;
				while ((*root & 4) != 4) count++;

			}
		}
	}

	
	// *b = ref;

	return;

  
}