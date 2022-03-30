#include "../search_clstr_hw_defines.h"

typedef unsigned array_t[256];
typedef struct rootCacheNode{
	int start;
	int end;
	int start_address;
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

void insert(unsigned* a, unsigned* b, cache_t* c) {
	if(c->occupancy < 10){
		c->entry[c->occupancy].start = a[1];
		c->entry[c->occupancy].end = b[1];
		c->entry[c->occupancy].start_address = a[0];
		c->occupancy++;
	}
	
	
}

void search(unsigned* key, unsigned * res, cache_t* c){
	// for(int i = 0; i < c->occupancy; c++){
	// 	if(key[0] <= c->entry[i].end && key[0] >= c->entry[i].start){
	// 		res[0] = 60;
	// 		break;
	// 	}
			
	// 	// res[0] = c->entry[2].end;
	// }
	res[0] = -1;
	int i = 0;
	while(i < c->occupancy){
		if(key[0] > c->entry[i].start && key[0] < c->entry[i].end){
			res[0] = c->entry[i].start_address;
		}
		i++;
	}
	
	// res[0] = c->entry[2].end;
}

void search_stream(query_t * q, unsigned* res, cache_t* c){
	
	int result = 90;
	int i = 0;
	while(i < c->occupancy){
		if(q->start > c->entry[i].start && q->start < c->entry[i].end){
			result = c->entry[i].start_address;
		}
		i++;
	}
	*res = result;
	
}

void top() {
	query_t* strIn = (query_t*)0x10020600;
	unsigned * strOut = (unsigned *)cache_search_out;
	unsigned* status = (unsigned *)0x100206f0;
	void * a = (void *)0x80c10000;
	unsigned * b = (unsigned *)0x80c10008;
	query_t temp;

	// unsigned * d = (unsigned *)0x80c10010;
	cache_t * c = (cache_t *)TESTA;
	int i = 0;
	while(1);
	// while(1){
		// if(*status != 0){
			// temp = *strIn;
			// search_stream(strIn, strOut, c);
			// *strOut = temp.start;
			// return;
		// }
		

		// i++;
	// }?
	
	// if(b[0] == 2){
	// 	// c->occupancy = 0;
	// 	insert(a,b,c);
	// }
	// else{

	// 	search(a, b, c);
	// }

	return;
}