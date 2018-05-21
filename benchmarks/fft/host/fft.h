#include "../defines.h"
#include "data.h"

#define common_val        *(unsigned *)0x80b00000
#define acc               *(char *)0x2f000000
#define loc_real          *(int *)0x2f000001
#define loc_img           *(int *)0x2f000009
#define loc_real_twid     *(int *)0x2f000011
#define loc_img_twid      *(int *)0x2f000019

typedef struct {
    double * real;
    double * img;
    double * real_twid;
    double * img_twid;
    double * real_check;
    double * img_check;
} fft_struct;

int checkData(fft_struct * ffts) {
    int i;
    for (i = 0; i < FFT_SIZE; i++) {
        if(((ffts->real[i]-ffts->real_check[i]) > EPSILON) || ((ffts->real[i]-ffts->real_check[i]) < -EPSILON)) {
            printf("Check Failed\n");
            return 0;
        }
        if(((ffts->img[i]-ffts->img_check[i]) > EPSILON) || ((ffts->img[i]-ffts->img_check[i]) < -EPSILON)) {
            printf("Check Failed\n");
            return 0;
        }
    }
    printf("Check Passed\n");
    return 1;
}

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
