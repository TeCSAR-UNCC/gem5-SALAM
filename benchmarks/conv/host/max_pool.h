#include "../defines.h"

#define acc_pool            *(char *)0x2f000202
#define loc_pool_in        	*(int *)0x2f00022c
#define loc_pool_out     	*(int *)0x2f000234


typedef struct {
	TYPE * in;
	TYPE * out;
} max_pool_struct;


