#include "../defines.h"
#include <cmath>

#define common_val      *(unsigned *)0x80b00000
#define acc             *(char *)0x2f000000
#define loc_in          *(int *)0x2f000001
#define loc_coefficient *(int *)0x2f000009
#define loc_previous    *(int *)0x2f000011
#define loc_temp        *(int *)0x2f000019

typedef struct {
    int * in;
    int * coefficient;
    int * previous;
    int * temp;    
    int * check;
} fir_struct;

int checkData(fir_struct * fir) {
    int i, total = 0;
    
    for (i = 0; i < INPUTSIZE; i++) {
        total += fir->temp[i];
        printf("Temp[%2d] %5d\n", i, fir->temp[i]);
    }
    
    printf("Total %5d, Check %5d\n", total, fir->check[0]);
    if(total != fir->check[0]) {
        printf("Check Failed\n");
        return 0;
    }
    printf("Check Passed\n");
    return 1;
}

void genData(fir_struct * fir) {
    int i;
    
    for (i = 0; i < INPUTSIZE; i++) {
        fir->in[i] = i+1;
    }
    
    for (i = 0; i < TAPS; i++) {
        fir->previous[i] = 0;
        fir->coefficient[i] = 10;
//        printf("P[%2d] = %5d\n", i, fir->previous[i]);
        printf("C[%2d] = %5d\n", i, fir->coefficient[i]);
    }
    
    fir->check[0] = EXPECTED_TOTAL;
}
