#include "../defines.h"
#include "data.h"

volatile int stage;

typedef struct {
    double * real;
    double * img;
    double * real_twid;
    double * img_twid;
    double * real_check;
    double * img_check;
} fft_struct;

void genData(fft_struct * ffts) {
    int i;
    for (i = 0; i < FFT_SIZE; i++) {
        ffts->real[i] = re[i];
        ffts->img[i] = co[i];
        ffts->real_check[i] = re_chk[i];
        ffts->img_check[i] = co_chk[i];
    }
    for (i = 0; i < FFT_SIZE/2; i++) {
        ffts->real_twid[i] = re_twid[i];
        ffts->img_twid[i] = co_twid[i];
    }
}
