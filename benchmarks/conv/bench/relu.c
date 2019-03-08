#include "relu.h"

void relu(double in[FM_HEIGHT][FM_WIDTH], double out[FM_HEIGHT][FM_WIDTH]) {
    int i,j;

	#pragma clang loop unroll(disable)    
	for (i = 1; i < FM_HEIGHT-1; i++) {
		#pragma clang loop unroll(disable)
        for (j = 1; j < FM_WIDTH-1; j++) {
			out[i][j] = MAX(in[i][j],0);
        }
    }
}
