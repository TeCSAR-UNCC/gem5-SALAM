#include "relu.h"

void relu(double in[FM_HEIGHT][FM_WIDTH], double out[FM_HEIGHT][FM_WIDTH]) {
    int i,j;

    for (i = 0; i < FM_HEIGHT; i++) {
        for (j = 0; j < FM_WIDTH; j++) {
            out[i][j] = MAX(in[i][j],0);
        }
    }
}
