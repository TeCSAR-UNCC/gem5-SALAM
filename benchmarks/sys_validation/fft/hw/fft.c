#include "hw_defines.h"

void fft() {

    volatile uint8_t * realbase     = (uint8_t *)REALADDR;
    volatile uint8_t * imgbase      = (uint8_t *)IMGADDR;
    volatile uint8_t * realtwidbase = (uint8_t *)REALTWIDADDR;
    volatile uint8_t * imgtwidbase  = (uint8_t *)IMGTWIDADDR;
    volatile double  * real         = (double  *)realbase;
    volatile double  * img          = (double  *)imgbase;
    volatile double  * real_twid    = (double  *)realtwidbase;
    volatile double  * img_twid     = (double  *)imgtwidbase;

    int even, odd, span, log, rootindex;
    double temp;

    log = 0;

    outer:
    for(span=FFT_SIZE>>1; span; span>>=1, log++){
        inner:
        for(odd=span; odd<FFT_SIZE; odd++){
            odd |= span;
            even = odd ^ span;

            temp = real[even] + real[odd];
            real[odd] = real[even] - real[odd];
            real[even] = temp;

            temp = img[even] + img[odd];
            img[odd] = img[even] - img[odd];
            img[even] = temp;

            rootindex = (even<<log) & (FFT_SIZE - 1);
            if(rootindex){
                temp = real_twid[rootindex] * real[odd] -
                    img_twid[rootindex]  * img[odd];
                img[odd] = real_twid[rootindex]*img[odd] +
                    img_twid[rootindex]*real[odd];
                real[odd] = temp;
            }
        }
    }
}
