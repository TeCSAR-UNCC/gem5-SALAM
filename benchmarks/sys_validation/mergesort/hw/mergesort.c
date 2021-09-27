#include "hw_defines.h"

void merge(int start, int m, int stop){
    
    uint8_t * arrBase = (uint8_t *)MAIN;
    uint8_t * tempBase = (uint8_t *)(TEMP);
    TYPE    * a     = (TYPE    *)arrBase;
    TYPE    * temp     = (TYPE    *)tempBase;
    
    int i, j, k;

    merge_label1 : for(i=start; i<=m; i++){
        temp[i] = a[i];
    }

    merge_label2 : for(j=m+1; j<=stop; j++){
        temp[m+1+stop-j] = a[j];
    }

    i = start;
    j = stop;


    merge_label3 : for(k=start; k<=stop; k++){
        TYPE tmp_j = temp[j];
        TYPE tmp_i = temp[i];
        if(tmp_j < tmp_i) {
            a[k] = tmp_j;
            j--;
        } else {
            a[k] = tmp_i;
            i++;
        }
    }
}

void top() {
    int start, stop;
    int i, m, from, mid, to;

    uint8_t * arrBase = (uint8_t *)MAIN;
    volatile TYPE    * a     = (TYPE    *)arrBase;
    
    start = 0;
    stop = SIZE;

    mergesort_label1 : for(m=1; m<stop-start; m+=m) {
        mergesort_label2 : for(i=start; i<stop; i+=m+m) {
            from = i;
            mid = i+m-1;
            to = i+m+m-1;
            if(to < stop){
                merge(from, mid, to);
            }
            else{
                merge(from, mid, stop);
            }
        }
    }
}