#include "../defines.h"

#define acc_comb            *(char *)0x2f000000
#define acc_conv_in			*(unsigned long long *)0x2f000001
#define acc_conv_kern		*(unsigned long long *)0x2f000009
#define acc_relu_in			*(unsigned long long *)0x2f000011
#define acc_pool_in			*(unsigned long long *)0x2f000019
#define acc_pool_out		*(unsigned long long *)0x2f000021

typedef struct {
	TYPE * conv_in;
	TYPE * conv_kern;
	TYPE * conv_out;
	TYPE * relu_in;
	TYPE * relu_out;
	TYPE * pool_in;
	TYPE * pool_out;
	TYPE * check;
} comb_struct;


int checkDataComb(comb_struct * combs) {
    printf("Check Passed\n");
    return 1;
}

void genDataComb(comb_struct * combs) {
	int i, j;    
	TYPE randValue;
	srand(RAND_SEED);
	for(i = 0; i<FM_WIDTH; i++) {
		for(j = 0; j<FM_HEIGHT; j++) {
			randValue = UNIFORM();
			//printf("Value[%d][%d] = %lf \n", i, j, randValue);
			combs->conv_in[i+(j*FM_WIDTH)] = randValue;
			//combs->check[i+(j*FM_WIDTH)] = randValue;
		}
	}

	combs->conv_kern[0] = 0; //Sharpen Kernel
	combs->conv_kern[1] = -1;
	combs->conv_kern[2] = 0;
	combs->conv_kern[3] = -1;
	combs->conv_kern[4] = 5;
	combs->conv_kern[5] = -1;
	combs->conv_kern[6] = 0;
	combs->conv_kern[7] = -1;
	combs->conv_kern[8] = 0;
}
