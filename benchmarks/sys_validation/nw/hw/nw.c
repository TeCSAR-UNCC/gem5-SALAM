#include "hw_defines.h"

#define MATCH_SCORE 1
#define MISMATCH_SCORE -1
#define GAP_SCORE -1

#define ALIGN '\\'
#define SKIPA '^'
#define SKIPB '<'

#define MAX(A,B) ( ((A)>(B))?(A):(B) )

void needwun(){

    volatile uint8_t * seqA     = (uint8_t *)SEQA;
    volatile uint8_t * seqB     = (uint8_t *)SEQB;
    volatile uint8_t * alignedA = (uint8_t *)ALIGNEDA;
    volatile uint8_t * alignedB = (uint8_t *)ALIGNEDB;
    volatile int32_t * m        = (int32_t *)M;
    volatile uint8_t * ptr      = (uint8_t *)PTR;

    int score, up_left, up, left, max;
    int row, row_up, r;
    int a_idx, b_idx;
    int a_str_idx, b_str_idx;

    init_row:
    for(a_idx=0; a_idx<(ALEN+1); a_idx++){
        m[a_idx] = a_idx * GAP_SCORE;
    }
    init_col:
    for(b_idx=0; b_idx<(BLEN+1); b_idx++){
        m[b_idx*(ALEN+1)] = b_idx * GAP_SCORE;
    }

    // Matrix filling loop
    fill_out:
    for(b_idx=1; b_idx<(BLEN+1); b_idx++){
        fill_in:
        for(a_idx=1; a_idx<(ALEN+1); a_idx++){
            if(seqA[a_idx-1] == seqB[b_idx-1]){
                score = MATCH_SCORE;
            } else {
                score = MISMATCH_SCORE;
            }

            row_up = (b_idx-1)*(ALEN+1);
            row = (b_idx)*(ALEN+1);

            up_left = m[row_up + (a_idx-1)] + score;
            up      = m[row_up + (a_idx  )] + GAP_SCORE;
            left    = m[row    + (a_idx-1)] + GAP_SCORE;

            max = MAX(up_left, MAX(up, left));

            m[row + a_idx] = max;
            if(max == left){
                ptr[row + a_idx] = SKIPB;
            } else if(max == up){
                ptr[row + a_idx] = SKIPA;
            } else{
                ptr[row + a_idx] = ALIGN;
            }
        }
    }

    // TraceBack (n.b. aligned sequences are backwards to avoid string appending)
    a_idx = ALEN;
    b_idx = BLEN;
    a_str_idx = 0;
    b_str_idx = 0;

    trace:
    while(a_idx>0 || b_idx>0) {
        r = b_idx*(ALEN+1);
        if (ptr[r + a_idx] == ALIGN){
            alignedA[a_str_idx++] = seqA[a_idx-1];
            alignedB[b_str_idx++] = seqB[b_idx-1];
            a_idx--;
            b_idx--;
        }
        else if (ptr[r + a_idx] == SKIPB){
            alignedA[a_str_idx++] = seqA[a_idx-1];
            alignedB[b_str_idx++] = '-';
            a_idx--;
        }
        else{ // SKIPA
            alignedA[a_str_idx++] = '-';
            alignedB[b_str_idx++] = seqB[b_idx-1];
            b_idx--;
        }
    }

    // Pad the result
    pad_a:
    for( ; a_str_idx<ALEN+BLEN; a_str_idx++ ) {
      alignedA[a_str_idx] = '_';
    }
    pad_b:
    for( ; b_str_idx<ALEN+BLEN; b_str_idx++ ) {
      alignedB[b_str_idx] = '_';
    }
}
