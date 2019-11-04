#include "max_pool.h"

//Max pooling using a 2x2 window and stride of 2
void max_pool(double in[FM_HEIGHT][FM_WIDTH], double out[FM_HEIGHT/2][FM_WIDTH/2]) {
    int xpos,ypos,i,j,max_val;

	#pragma clang loop unroll(disable)
    for (ypos = 1; ypos < FM_HEIGHT-1; ypos+=2) {
		#pragma clang loop unroll(disable)
        for (xpos = 1; xpos < FM_WIDTH-1; xpos+=2) {
            max_val = 0;
            for (i = ypos; i < ypos+2; i++) {
                for (j = xpos; j < xpos+2; j++) {
                    max_val = MAX(max_val,in[i][j]);
                }
            }
            out[ypos/2][xpos/2] = max_val;
        }
    }
}
