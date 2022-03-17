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


	unsigned * f    = (unsigned *)0x80c10000;
    unsigned * g    = (unsigned *)0x80c10008;
	// unsigned * m    = (unsigned *)0x80c10010;
    cache_t * h     = (cache_t *)TESTA;

    int count;

    // volatile int count = 0;
    // h->occupancy = 0;
    

	// *b = g[0];

	int rlg=0;
	int flg=0;
	int blg=0;
	
	bTree* cursor = (bTree*)*c;
	uint64_t key = *a;
	
	

	while(!cursor->l){
		for(int i = 0; i < cursor->n; i++){
			if(key<cursor->d[i])
			{
				rlg=1;
				blg=i;
				cursor = cursor->child_ptr[i];
				
				break;
			}
			if(key == cursor->d[i])
			{
				*b=1;
				flg=1;
				if(i-1>0)
				{
					f[1] = cursor->d[i-1];
					g[1] = key;
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					
				}
				else
				{
					if(i+1<cursor->n)
					{

					
					f[1] = key;
					g[1] = cursor->d[i+1];
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					}
					else{
					f[1] = key;
					g[1] = cursor->d[i];
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					}

				}
				rlg=1;
				blg=i;

				cursor = cursor->child_ptr[i];
				break;
			}
			if(i == cursor->n - 1){
				rlg=2;
				blg=i;
				cursor = cursor->child_ptr[i+1];
				break;
			}
		}
		
			if(blg-1>0)
				{
					f[1] = cursor->d[blg-1];
					g[1] = cursor->d[blg];
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					
				}
				else
				{
					if(blg+1<cursor->n)
					{

					
					f[1] = key;
					g[1] = cursor->d[blg+1];
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					}
					else{
					f[1] = key;
					g[1] = cursor->d[blg];
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					}
		

		}
			if(rlg==1)
			cursor=cursor->child_ptr[blg];
			else if(rlg==2)
			cursor=cursor->child_ptr[blg+1];
			else
			cursor=cursor->child_ptr[cursor->n]; 

			rlg=0;
			blg=0;
			flg=0;
	
	}
		for(int i = 0; i < cursor->n; i++){
		
		if(key == cursor->d[i])
		{
			flg=1;
			if(i-1>0)
				{
					f[1] = cursor->d[i-1];
					g[1] = key;
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;
					
				}
				else
				{

					f[1] = key;
					g[1] = cursor->d[i+1];
					f[0] = cursor;
					// m[0] = (unsigned)&key; 
					g[0] = 2;
					*root = 0x01;
					while ((*root & 4) != 4) count++;

				}
		}
	}

	f[0] = 5;
    g[1] = 5;
    g[0] = 1;
    

    *root = 0x01;
    while ((*root & 4) != 4) count++;

	*b = g[0];


	return;

  
}