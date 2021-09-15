#include "../defines.h"

#define acc_conv        	*(char *)0x2f000000
#define loc_conv_in        	*(unsigned long long *)0x2f000001
#define loc_conv_kern       *(unsigned long long *)0x2f000009
#define loc_conv_out     	*(unsigned long long *)0x2f000011


typedef struct {
	TYPE * in;
	TYPE * kern;
	TYPE * out;
	TYPE * check;
} conv_struct;


int checkData(conv_struct * convs) {
    printf("Check Passed\n");
    return 1;
}

void genData(conv_struct * convs) {
	int i, j;    
	TYPE randValue;
	srand(RAND_SEED);
	for(i = 0; i<FM_WIDTH; i++) {
		for(j = 0; j<FM_HEIGHT; j++) {
			randValue = UNIFORM();
			//printf("Value[%d][%d] = %lf \n", i, j, randValue); 
			convs->in[i+(j*FM_WIDTH)] = randValue;
			//convs->check[i+(j*FM_WIDTH)] = randValue;
		}
	}

	convs->kern[0] = 0; //Sharpen Kernel
	convs->kern[1] = -1;
	convs->kern[2] = 0;
	convs->kern[3] = -1;
	convs->kern[4] = 5;
	convs->kern[5] = -1;
	convs->kern[6] = 0;
	convs->kern[7] = -1;
	convs->kern[8] = 0;
}
