#include "stdio.h"
#include "queue_dma.h"

int launchCpy() {
    //*FLAGS &= 0xFB;
    if (valid[rdQueuePtr] == 1) {
/*        printf("Launching Xfer %d\n", rdQueuePtr);*/
/*        printf("SRC:%x DST:%x LEN:%d\n", jobs[rdQueuePtr].src, jobs[rdQueuePtr].dst, jobs[rdQueuePtr].len);*/
        *SRC = jobs[rdQueuePtr].src;
        *DST = jobs[rdQueuePtr].dst;
        *LEN = jobs[rdQueuePtr].len;
        *FLAGS |= 0x01;
        valid[rdQueuePtr] = 0;
        inQueue--;
/*        printf("Xfer %d launched\n", rdQueuePtr);*/
        if (rdQueuePtr<(QUEUE_SIZE-1))
            rdQueuePtr++;
        else
            rdQueuePtr = 0;
        return 0;
    } else {
        //printf("No more xfers\n");
        *FLAGS=0;
        return -1;
    }
}

extern int dma_signal;

int dmacpy(void * dst, void * src, int len) {
    if (valid[wrQueuePtr] == 0) {
        dma_signal = 0;
/*        printf("Queueing Xfer %d\n", wrQueuePtr);*/
        jobs[wrQueuePtr].src = (size_t)src;
        jobs[wrQueuePtr].dst = (size_t)dst;
        jobs[wrQueuePtr].len = len;
        valid[wrQueuePtr] = 1;
        inQueue++;
        if (wrQueuePtr<(QUEUE_SIZE-1))
            wrQueuePtr++;
        else
            wrQueuePtr = 0;
        if ((*FLAGS&0x02) != 0x02)
            launchCpy();
        return 0;
    } else {
/*        printf("ERROR: Cannot Queue Xfer %d\n", wrQueuePtr);*/
        return -1;
    }
}

int pollDma() {
    return ((*FLAGS&0x04)==0x04);
}
void resetDma() {
    *FLAGS = 0;
}
