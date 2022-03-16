#include "../search_clstr_hw_defines.h"

typedef unsigned array_t[256];
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

void insert(unsigned* a, unsigned* b, cache_t* c) {
	if(c->occupancy < 10){
		c->entry[c->occupancy].start = a[1];
		c->entry[c->occupancy].end = b[1];
		c->entry[c->occupancy].start_address = 12;
		c->occupancy++;
	}
	
}

void search(unsigned* key, unsigned * res, cache_t* c){
	for(int i = 0; i < c->occupancy; c++){
		if(key[0] < c->entry[i].end && key[0] > c->entry[i].start)
			res[0] = c->entry[i].start_address;
		res[0] = c->occupancy;
	}
}

void top() {
	unsigned* status = (unsigned *)0x100206f0;
	void * a = (void *)0x80c10000;
	unsigned * b = (unsigned *)0x80c10008;
	cache_t * c = (cache_t *)TESTA;
	if(*status != 0){
		return;
	}
	if(b[0] == 2){
		// c->occupancy = 0;
		insert(a,b,c);
	}
	else{

		search(a, b, c);
	}

	return;
}