#include "conv.h"

void conv(double in[FM_HEIGHT][FM_WIDTH], double kern[KERN_HEIGHT][KERN_WIDTH], double out[FM_HEIGHT][FM_WIDTH]) {

    // find center position of kernel (half of kernel size)
    int kCenterX = KERN_WIDTH / 2;
    int kCenterY = KERN_HEIGHT / 2;

    int i,j,m,n;

    for (i = 1; i < FM_HEIGHT-1; ++i)              // rows
    {
        #pragma clang loop unroll_count(8)
        for (j = 1; j < FM_WIDTH-1; ++j)          // columns
        {
			double sum = 0;
			for (m = 0; m < KERN_HEIGHT; ++m)     // kernel rows
            {
                int mm = KERN_HEIGHT - 1 - m;      // row index
                for (n = 0; n < KERN_WIDTH; ++n) // kernel columns
                {
                    int nn = KERN_WIDTH - 1 - n;  // column index

                    // index of input signal, used for checking boundary
                    int ii = i + (m - kCenterY);
                    int jj = j + (n - kCenterX);

                    // ignore input samples which are out of bound
                    //if (ii >= 0 && ii < FM_HEIGHT && jj >= 0 && jj < FM_WIDTH)
                        sum += in[ii][jj] * kern[mm][nn];
                }
            }
            out[i][j]=sum;
        }
    }
}
