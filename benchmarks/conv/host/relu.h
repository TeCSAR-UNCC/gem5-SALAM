#include "../defines.h"

#define acc_relu            *(char *)0x2f000100
#define loc_relu_in        	*(unsigned long long *)0x2f000101
#define loc_relu_out     	*(unsigned long long *)0x2f000109


typedef struct {
	TYPE * in;
	TYPE * out;
} relu_struct;


