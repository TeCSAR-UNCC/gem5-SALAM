#include "../defines.h"

#define acc_pool            *(char *)0x2f000200
#define loc_pool_in        	*(unsigned long long *)0x2f000201
#define loc_pool_out     	*(unsigned long long *)0x2f000209


typedef struct {
	TYPE * in;
	TYPE * out;
} max_pool_struct;


