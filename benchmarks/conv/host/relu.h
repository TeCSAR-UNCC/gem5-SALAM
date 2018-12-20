#include "../defines.h"

#define acc_relu            *(char *)0x2f000101
#define loc_relu_in        	*(int *)0x2f00011c
#define loc_relu_out     	*(int *)0x2f000124


typedef struct {
	TYPE * in;
	TYPE * out;
} relu_struct;


