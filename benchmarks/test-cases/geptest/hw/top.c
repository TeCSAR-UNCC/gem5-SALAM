#include "../geptest_clstr_hw_defines.h"

typedef struct base_struct {
	unsigned first;
	unsigned second;
} base_struct_t;

typedef unsigned array1d_t[256];
typedef unsigned array4d_t[4][4][4][4];
typedef base_struct_t struct_array[8][4][4];

void top() {
	int i,j,k,l,count;

	unsigned 		* a = (unsigned *)TESTA;
	base_struct_t 	* b = (base_struct_t *)TESTB;
	array1d_t 		* c = (array1d_t *)TESTC;
	array4d_t 		* d = (array4d_t *)TESTD;
	struct_array 	* e = (struct_array *)TESTE;

	for (i=0; i<256; i++) {
		a[i] = i;
	}

	count = 0;
	for (i=0; i<128; i++) {
		b[i].first = count;
		count++;
		b[i].second = count;
		count++;
	}

	for (i=0; i<256; i++) {
		*c[i] = i;
	}

	count = 0;
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			for (k=0; k<4; k++) {
				for (l=0; l<4; l++) {
					*d[i][j][k][l] = count;
					count++;
				}
			}
		}
	}

	count = 0;
	for (i=0; i<8; i++) {
		for (j=0; j<4; j++) {
			for (k=0; k<4; k++) {
				(*e[i][j][k]).first = count;
				count++;
				(*e[i][j][k]).second = count;
				count++;
			}
		}
	}

	return;
}