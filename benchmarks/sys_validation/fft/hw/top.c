#include "hw_defines.h"

void top(uint64_t REAL_addr,
		 uint64_t IMG_addr,
		 uint64_t REALTWID_addr,
		 uint64_t IMGTWID_addr) {

	//Define Device MMRs
	volatile uint8_t  * FFTFlags  = (uint8_t *)FFT;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Transfer Input Matrices
	//Transfer REAL
	*DmaRdAddr  = REAL_addr;
	*DmaWrAddr  = REAL;
	*DmaCopyLen = FFT_SIZE * sizeof(double);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer IMG
	*DmaRdAddr  = IMG_addr;
	*DmaWrAddr  = IMG;
	*DmaCopyLen = FFT_SIZE * sizeof(double);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer REAL
	*DmaRdAddr  = REALTWID_addr;
	*DmaWrAddr  = REALTWID;
	*DmaCopyLen = FFT_HALF * sizeof(double);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer IMG
	*DmaRdAddr  = IMGTWID_addr;
	*DmaWrAddr  = IMGTWID;
	*DmaCopyLen = FFT_HALF * sizeof(double);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*FFTFlags = DEV_INIT;
	//Poll function for finish
	while ((*FFTFlags & DEV_INTR) != DEV_INTR);

	//Transfer REAL back
	*DmaRdAddr  = REAL;
	*DmaWrAddr  = REAL_addr;
	*DmaCopyLen = FFT_SIZE * sizeof(double);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer IMG back
	*DmaRdAddr  = IMG;
	*DmaWrAddr  = IMG_addr;
	*DmaCopyLen = FFT_SIZE * sizeof(double);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}