#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "vadd.h"

vadd_struct vas;

#define MMR0        0x2ff10000
#define MMR1        0x2ff20000
#define BUFF_OFF    32

#define FLAGS_OFF               0
#define CONFIG_OFF              FLAGS_OFF+1
#define RD_ADDR_OFF             CONFIG_OFF+3
#define WR_ADDR_OFF             RD_ADDR_OFF+8
#define RD_FRAME_SIZE_OFF       WR_ADDR_OFF+8
#define NUM_RD_FRAMES_OFF       RD_FRAME_SIZE_OFF+4
#define RD_FRAME_BUFF_SIZE_OFF  NUM_RD_FRAMES_OFF+1
#define WR_FRAME_SIZE_OFF       RD_FRAME_BUFF_SIZE_OFF+1
#define NUM_WR_FRAMES_OFF       WR_FRAME_SIZE_OFF+4
#define WR_FRAME_BUFF_SIZE_OFF  NUM_WR_FRAMES_OFF+1
#define BUFFER_ACCESS_OFF       WR_FRAME_BUFF_SIZE_OFF+1

volatile uint8_t  * FLAGS0 = (uint8_t *)(MMR0);
volatile uint16_t * CONFIG0 = (uint16_t *)(MMR0+CONFIG_OFF);
volatile uint64_t * RD_ADDR0 = (uint64_t *)(MMR0+RD_ADDR_OFF);
volatile uint64_t * WR_ADDR0 = (uint64_t *)(MMR0+WR_ADDR_OFF);
volatile uint32_t * RD_FRAME_SIZE0 = (uint32_t *)(MMR0+RD_FRAME_SIZE_OFF);
volatile uint8_t  * NUM_RD_FRAMES0 = (uint8_t *)(MMR0+NUM_RD_FRAMES_OFF);
volatile uint8_t  * RD_FRAME_BUFF_SIZE0 = (uint8_t *)(MMR0+RD_FRAME_BUFF_SIZE_OFF);
volatile uint32_t * WR_FRAME_SIZE0 = (uint32_t *)(MMR0+WR_FRAME_SIZE_OFF);
volatile uint8_t  * NUM_WR_FRAMES0 = (uint8_t *)(MMR0+NUM_WR_FRAMES_OFF);
volatile uint8_t  * WR_FRAME_BUFF_SIZE0 = (uint8_t *)(MMR0+WR_FRAME_BUFF_SIZE_OFF);

volatile uint8_t  * FLAGS1 = (uint8_t *)(MMR1);
volatile uint16_t * CONFIG1 = (uint16_t *)(MMR1+CONFIG_OFF);
volatile uint64_t * RD_ADDR1 = (uint64_t *)(MMR1+RD_ADDR_OFF);
volatile uint64_t * WR_ADDR1 = (uint64_t *)(MMR1+WR_ADDR_OFF);
volatile uint32_t * RD_FRAME_SIZE1 = (uint32_t *)(MMR1+RD_FRAME_SIZE_OFF);
volatile uint8_t  * NUM_RD_FRAMES1 = (uint8_t *)(MMR1+NUM_RD_FRAMES_OFF);
volatile uint8_t  * RD_FRAME_BUFF_SIZE1 = (uint8_t *)(MMR1+RD_FRAME_BUFF_SIZE_OFF);
volatile uint32_t * WR_FRAME_SIZE1 = (uint32_t *)(MMR1+WR_FRAME_SIZE_OFF);
volatile uint8_t  * NUM_WR_FRAMES1 = (uint8_t *)(MMR1+NUM_WR_FRAMES_OFF);
volatile uint8_t  * WR_FRAME_BUFF_SIZE1 = (uint8_t *)(MMR1+WR_FRAME_BUFF_SIZE_OFF);

void dmaRead(int dmaNum, void * src, uint32_t frameSize, uint8_t numFrames, uint8_t frameBuffSize, uint16_t intFrames) {
    if (dmaNum == 0) {
        *RD_ADDR0 = (size_t)src;
        *RD_FRAME_SIZE0 = frameSize;
        *NUM_RD_FRAMES0 = numFrames;
        *RD_FRAME_BUFF_SIZE0 = frameBuffSize;
        *CONFIG0 = intFrames;
        *FLAGS0 |= 0x01;
    } else {
        *RD_ADDR1 = (size_t)src;
        *RD_FRAME_SIZE1 = frameSize;
        *NUM_RD_FRAMES1 = numFrames;
        *RD_FRAME_BUFF_SIZE1 = frameBuffSize;
        *CONFIG1 = intFrames;
        *FLAGS1 |= 0x01;
    }
}

void dmaWrite(int dmaNum, void * dst, uint32_t frameSize, uint8_t numFrames, uint8_t frameBuffSize, uint16_t intFrames) {
    if (dmaNum == 0) {
        *WR_ADDR0 = (size_t)dst;
        *WR_FRAME_SIZE0 = frameSize;
        *NUM_WR_FRAMES0 = numFrames;
        *WR_FRAME_BUFF_SIZE0 = frameBuffSize;
        *CONFIG0 = (intFrames<<8);
        *FLAGS0 |= 0x02;
    } else {
        *WR_ADDR1 = (size_t)dst;
        *WR_FRAME_SIZE1 = frameSize;
        *NUM_WR_FRAMES1 = numFrames;
        *WR_FRAME_BUFF_SIZE1 = frameBuffSize;
        *CONFIG1 = (intFrames<<8);
        *FLAGS1 |= 0x02;
    }
}

int main(void) {
	uint64_t base = 0x80c00000;

	TYPE *a = (TYPE *)(base+0);
	TYPE *b = (TYPE *)(base+sizeof(TYPE)*LENGTH);
	TYPE *c = (TYPE *)(base+2*sizeof(TYPE)*LENGTH);
	TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*LENGTH);

	common_val = 0;
    vas.a = a;
    vas.b = b;
    vas.c = c;
    vas.check = check;
    vas.length = LENGTH;

    printf("Generating data\n");
    genData(&vas);
    printf("Data generated\n");

    dmaRead(0, a, LENGTH, sizeof(TYPE), sizeof(TYPE), 0);
    dmaRead(1, b, LENGTH, sizeof(TYPE), sizeof(TYPE), 0);
    dmaWrite(0, c, LENGTH, sizeof(TYPE), sizeof(TYPE), 1);

    val_a = (uint64_t)(MMR0+BUFF_OFF);
    val_b = (uint64_t)(MMR1+BUFF_OFF);
    val_c = (uint64_t)(MMR0+BUFF_OFF);

    int i;
    printf("%d\n", acc);

    acc = 0x01;
    printf("%d\n", acc);

	while(acc != 0x4) {
        printf("%d\n", acc);
	}

    while ((*FLAGS0&0x08) == 0x08);

    acc = 0x00;
	if(!checkData(&vas)) {
	    for (i = 0; i < LENGTH; i++) {
	        printf("C[%2d]=%f\n", i, vas.c[i]);
	    }
	}
	printf("%d", acc);
	*(char *)0x2FFFFFFF = 1;
}
